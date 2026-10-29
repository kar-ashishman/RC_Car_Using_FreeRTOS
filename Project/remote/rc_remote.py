"""PS5 (DualSense) style RC car remote that sends button commands as UDP packets.

Run:  python rc_remote.py [--config path\\to\\config.json]
"""
from __future__ import annotations

import argparse
import html
import math
import sys
import time
from functools import partial
from pathlib import Path

from PySide6.QtCore import QEvent, QFileSystemWatcher, QObject, Qt, QTimer, QUrl, Signal
from PySide6.QtGui import QDesktopServices, QKeySequence
from PySide6.QtWidgets import (
    QApplication, QCheckBox, QHBoxLayout, QLabel, QLineEdit, QMainWindow, QMessageBox,
    QPlainTextEdit, QPushButton, QSpinBox, QSplitter, QVBoxLayout, QWidget,
)

from config_loader import (
    BUTTON_NAMES, STICK_DIRECTIONS, STICKS, ConfigError, RemoteConfig, encode_command, load_config,
)
from dualsense_widget import DualSenseWidget
from udp_link import LinkError, UdpLink

DEFAULT_CONFIG = Path(__file__).with_name("config.json")
STICK_ON, STICK_OFF = 0.5, 0.4  # hysteresis for turning stick drags into direction presses


def _key_int(key) -> int:
    return key.value if hasattr(key, "value") else int(key)


def qt_key_from_name(name: str) -> int | None:
    """Translate a config key name ("W", "Space", "Up", "F1", "Shift", ";") into a Qt key code."""
    name = name.strip()
    if not name:
        return None
    attr = "Key_" + (name.upper() if len(name) == 1 else name[:1].upper() + name[1:])
    key = getattr(Qt.Key, attr, None)
    if key is not None:
        return _key_int(key)
    seq = QKeySequence.fromString(name, QKeySequence.PortableText)
    if seq.isEmpty():
        return None
    value = _key_int(seq[0].key())
    return value if value not in (0, _key_int(Qt.Key.Key_unknown)) else None


def describe_bytes(data: bytes) -> str:
    if all(32 <= b < 127 or b in (9, 10, 13) for b in data):
        return data.decode("ascii").replace("\r", "\\r").replace("\n", "\\n").replace("\t", "\\t")
    return "hex " + data.hex(" ").upper()


class RemoteController(QObject):
    changed = Signal()
    log = Signal(str, str)              # level, message
    connection_changed = Signal(bool)

    def __init__(self, config_path: Path):
        super().__init__()
        self.config_path = Path(config_path)
        self.cfg: RemoteConfig | None = None
        self.link = UdpLink()
        self._last_error, self._last_error_time = "", 0.0
        self._active: dict[str, set[str]] = {}
        self._repeat: dict[str, QTimer] = {}
        self._key_map: dict[int, str] = {}
        self._stick_mouse: dict[str, tuple[float, float] | None] = dict.fromkeys(STICKS)
        self._analog_last: dict[str, tuple[int, int] | None] = dict.fromkeys(STICKS)
        self._analog_timers: dict[str, QTimer] = {}
        for stick in STICKS:
            t = QTimer(self)
            t.timeout.connect(partial(self._analog_tick, stick))
            self._analog_timers[stick] = t
        self._heartbeat = QTimer(self)
        self._heartbeat.timeout.connect(self._send_heartbeat)
        self._rx_timer = QTimer(self)
        self._rx_timer.setInterval(30)
        self._rx_timer.timeout.connect(self._poll_rx)

    # ------------------------------------------------------------------ config
    def load(self) -> bool:
        try:
            cfg = load_config(self.config_path)
        except ConfigError as e:
            self.log.emit("error", f"Config not loaded: {e}")
            return False

        key_map: dict[int, str] = {}
        for name, b in cfg.buttons.items():
            if not b.key.strip():
                continue
            code = qt_key_from_name(b.key)
            if code is None:
                cfg.warnings.append(f"buttons.{name}: unknown key name {b.key!r}")
                continue
            if code in key_map:
                cfg.warnings.append(f"Key {b.key!r} is mapped to both '{key_map[code]}' and '{name}'; "
                                    f"using '{name}'")
            key_map[code] = name

        self.release_all("config reload", send_stop=False)
        self.cfg = cfg
        self._key_map = key_map
        self._analog_last = dict.fromkeys(STICKS)
        for stick, timer in self._analog_timers.items():
            timer.stop()
            st = cfg.sticks[stick]
            if st.command:
                timer.start(st.interval_ms)
        self._heartbeat.stop()
        if cfg.behavior.heartbeat_command and cfg.behavior.heartbeat_interval_ms > 0:
            self._heartbeat.start(max(20, cfg.behavior.heartbeat_interval_ms))
        for w in cfg.warnings:
            self.log.emit("warn", w)
        self.log.emit("info", f"Loaded {cfg.path.name}: {len(key_map)} keys mapped")
        self.changed.emit()
        return True

    # ------------------------------------------------------------ widget API
    @property
    def link_open(self) -> bool:
        return self.link.is_open

    def all_buttons(self):
        return BUTTON_NAMES

    def is_active(self, name: str) -> bool:
        return bool(self._active.get(name))

    def key_label(self, name: str) -> str:
        b = self.cfg.buttons.get(name) if self.cfg else None
        return b.key.strip() if b else ""

    def tooltip_for(self, name: str) -> str | None:
        if not self.cfg:
            return None
        if name.endswith("_stick"):
            stick = name[:-6]
            rows = [f"<b>{stick.title()} stick</b> (drag; right-click = {'L3' if stick == 'left' else 'R3'})"]
            for d in STICK_DIRECTIONS:
                rows.append(self._binding_row(f"{stick}_stick_{d}", d))
            rows.append(self._binding_row("l3" if stick == "left" else "r3", "click"))
            st = self.cfg.sticks[stick]
            if st.command:
                rows.append(f"analog: <code>{html.escape(st.command)}</code> every {st.interval_ms} ms")
            return "<br>".join(rows)
        b = self.cfg.buttons.get(name)
        if not b:
            return None
        title = f"<b>{name}</b>" + (f" — {html.escape(b.description)}" if b.description else "")
        return title + "<br>" + self._binding_row(name, None)

    def _binding_row(self, name, label):
        b = self.cfg.buttons[name]
        parts = [f"key <b>{html.escape(b.key) or '—'}</b>",
                 f"press <code>{html.escape(b.press) or '—'}</code>"]
        if b.release:
            parts.append(f"release <code>{html.escape(b.release)}</code>")
        if b.repeat_ms:
            parts.append(f"repeat {b.repeat_ms} ms")
        return (f"{label}: " if label else "") + ", ".join(parts)

    # -------------------------------------------------------- press/release
    def press(self, name: str, source: str):
        srcs = self._active.setdefault(name, set())
        if source in srcs:
            return
        was_active = bool(srcs)
        srcs.add(source)
        if not was_active:
            self._on_press(name)
        self.changed.emit()

    def release(self, name: str, source: str):
        srcs = self._active.get(name)
        if not srcs or source not in srcs:
            return
        srcs.discard(source)
        if not srcs:
            self._on_release(name)
        self.changed.emit()

    def release_source(self, source: str):
        for name, srcs in list(self._active.items()):
            if source in srcs:
                self.release(name, source)

    def release_all(self, reason: str, send_stop: bool = True):
        released = False
        for name, srcs in list(self._active.items()):
            if srcs:
                srcs.clear()
                self._on_release(name)
                released = True
        self._stick_mouse = dict.fromkeys(STICKS)
        if self.cfg and send_stop and self.cfg.behavior.stop_command:
            self.send(self.cfg.behavior.stop_command, f"stop ({reason})")
        elif released:
            self.log.emit("info", f"Released all buttons ({reason})")
        self.changed.emit()

    def _on_press(self, name: str):
        b = self.cfg.buttons.get(name) if self.cfg else None
        if not b:
            return
        if b.press:
            self.send(b.press, name)
            if b.repeat_ms > 0:
                t = QTimer(self)
                t.timeout.connect(partial(self.send, b.press, f"{name} (repeat)"))
                t.start(b.repeat_ms)
                self._repeat[name] = t

    def _on_release(self, name: str):
        t = self._repeat.pop(name, None)
        if t:
            t.stop()
            t.deleteLater()
        b = self.cfg.buttons.get(name) if self.cfg else None
        if b and b.release:
            self.send(b.release, f"{name} release")

    def handle_key(self, event, pressed: bool) -> bool:
        name = self._key_map.get(_key_int(event.key()))
        if name is None:
            return False
        if not event.isAutoRepeat():
            (self.press if pressed else self.release)(name, "key")
        return True

    # ------------------------------------------------------------- sticks
    def _held_excluding_stick(self, name: str) -> bool:
        return bool(self._active.get(name, set()) - {"stick"})

    def stick_position(self, stick: str) -> tuple[float, float]:
        m = self._stick_mouse.get(stick)
        if m:
            return m
        x = float(self._held_excluding_stick(f"{stick}_stick_right")) - float(self._held_excluding_stick(f"{stick}_stick_left"))
        y = float(self._held_excluding_stick(f"{stick}_stick_down")) - float(self._held_excluding_stick(f"{stick}_stick_up"))
        if x and y:
            x, y = x * math.sqrt(0.5), y * math.sqrt(0.5)
        return x, y

    def set_stick_mouse(self, stick: str, x: float, y: float):
        self._stick_mouse[stick] = (x, y)
        for d, value in (("up", -y), ("down", y), ("left", -x), ("right", x)):
            name = f"{stick}_stick_{d}"
            held = "stick" in self._active.get(name, ())
            if value > STICK_ON or (held and value > STICK_OFF):
                self.press(name, "stick")
            else:
                self.release(name, "stick")
        self.changed.emit()

    def clear_stick_mouse(self, stick: str):
        self._stick_mouse[stick] = None
        for d in STICK_DIRECTIONS:
            self.release(f"{stick}_stick_{d}", "stick")
        self.changed.emit()

    def _analog_tick(self, stick: str):
        st = self.cfg.sticks[stick]
        x, y = self.stick_position(stick)
        ix = round(x * st.range)
        iy = round((-y if st.invert_y else y) * st.range)
        if self._analog_last[stick] == (ix, iy):
            return
        self._analog_last[stick] = (ix, iy)
        self.send(st.command, f"{stick} stick", x=ix, y=iy, ux=ix + st.range, uy=iy + st.range)

    # ---------------------------------------------------------------- UDP
    def send(self, template: str, origin: str, quiet: bool = False, **values):
        if not self.cfg:
            return
        try:
            data = encode_command(template, self.cfg.udp, **values)
        except (ValueError, KeyError, IndexError, UnicodeEncodeError) as e:
            self.log.emit("error", f"Bad command for {origin}: {template!r} ({e})")
            return
        if not data:
            return
        if self.link.is_open:
            try:
                self.link.write(data)
            except LinkError as e:
                # UDP has no connection to lose; a failed send (e.g. Wi-Fi dropped) is reported but
                # the link stays open so sending resumes once the network is back.
                now = time.monotonic()
                if str(e) != self._last_error or now - self._last_error_time > 2:
                    self.log.emit("error", f"Send failed for {origin}: {e}")
                self._last_error, self._last_error_time = str(e), now
                return
            if not quiet:
                self.log.emit("tx", f"{origin}: {describe_bytes(data)}")
        elif not quiet:
            self.log.emit("dry", f"{origin}: {describe_bytes(data)}   (not connected)")

    def connect(self, host: str, port: int) -> bool:
        try:
            self.link.open(host, port, self.cfg.udp)
        except LinkError as e:
            self.log.emit("error", f"Could not open UDP link: {e}")
            self.connection_changed.emit(False)
            return False
        self._last_error = ""
        self._rx_timer.start()
        self.log.emit("info", f"Sending to {self.link.description}")
        self.connection_changed.emit(True)
        self.changed.emit()
        return True

    def disconnect(self, send_stop: bool = True):
        if not self.link.is_open:
            return
        if send_stop and self.cfg and self.cfg.behavior.stop_command:
            self.send(self.cfg.behavior.stop_command, "stop (disconnect)")
        self._rx_timer.stop()
        self.link.close()
        self.log.emit("info", "Disconnected")
        self.connection_changed.emit(False)
        self.changed.emit()

    def _send_heartbeat(self):
        if self.link.is_open:
            self.send(self.cfg.behavior.heartbeat_command, "heartbeat", quiet=True)

    def _poll_rx(self):
        packets = self.link.read_available()
        if not self.cfg.udp.show_rx:
            return
        for data, (ip, port) in packets:
            data = data.rstrip(b"\r\n")
            try:
                text = data.decode(self.cfg.udp.encoding)
                if not text.isprintable():
                    raise ValueError
            except ValueError:
                text = "hex " + data.hex(" ").upper()
            self.log.emit("rx", f"{ip}:{port}: {text}")

    def shutdown(self):
        self.release_all("closing", send_stop=not self.link.is_open)
        self.disconnect(send_stop=True)


LOG_COLORS = {"tx": "#7ee787", "rx": "#79c0ff", "dry": "#8b949e", "warn": "#e3b341",
              "error": "#ff7b72", "info": "#e6edf3"}

STYLE = """
QMainWindow, QWidget#central { background: #0f1115; }
QLabel, QCheckBox { color: #c9d1d9; }
QPushButton { background: #21262d; color: #e6edf3; border: 1px solid #30363d; border-radius: 6px;
              padding: 5px 12px; }
QPushButton:hover { background: #30363d; }
QPushButton#connect[connected="true"] { background: #1f6feb; border-color: #388bfd; }
QComboBox, QSpinBox { background: #161b22; color: #e6edf3; border: 1px solid #30363d; border-radius: 6px;
            padding: 4px 8px; min-width: 70px; }
QLineEdit:disabled, QSpinBox:disabled { color: #8b949e; }
QComboBox QAbstractItemView { background: #161b22; color: #e6edf3; selection-background-color: #1f6feb; }
QPlainTextEdit, QLineEdit { background: #0d1117; color: #e6edf3; border: 1px solid #30363d;
            border-radius: 6px; font-family: Consolas, 'Cascadia Mono', monospace; }
QSplitter::handle { background: #0f1115; }
"""


class MainWindow(QMainWindow):
    def __init__(self, ctrl: RemoteController):
        super().__init__()
        self.ctrl = ctrl
        self.setWindowTitle("RC Remote — DualSense style")
        self.resize(1080, 820)

        central = QWidget(objectName="central")
        self.setCentralWidget(central)
        root = QVBoxLayout(central)

        bar = QHBoxLayout()
        self.host_edit = QLineEdit(placeholderText="car IP, e.g. 192.168.4.1")
        self.host_edit.setMinimumWidth(170)
        self.host_edit.setToolTip("IP address or hostname of the car (a broadcast address like "
                                  "192.168.4.255 also works). Press Enter or Esc to return to driving.")
        self.host_edit.returnPressed.connect(lambda: self.pad.setFocus())
        self.port_spin = QSpinBox(minimum=1, maximum=65535)
        self.port_spin.setToolTip("UDP port the car listens on")
        self.connect_btn = QPushButton("Connect", objectName="connect")
        self.connect_btn.clicked.connect(self.toggle_connection)
        reload_btn = QPushButton("Reload config")
        reload_btn.clicked.connect(self.reload_config)
        open_btn = QPushButton("Edit config")
        open_btn.clicked.connect(lambda: QDesktopServices.openUrl(QUrl.fromLocalFile(str(ctrl.config_path))))
        self.labels_box = QCheckBox("Key labels")
        self.status = QLabel()
        for w in (QLabel("Car IP"), self.host_edit, QLabel("UDP port"), self.port_spin,
                  self.connect_btn, reload_btn, open_btn, self.labels_box):
            if w not in (self.host_edit, self.port_spin):
                w.setFocusPolicy(Qt.NoFocus)
            bar.addWidget(w)
        bar.addStretch(1)
        bar.addWidget(self.status)
        root.addLayout(bar)

        self.pad = DualSenseWidget(ctrl)
        self.labels_box.toggled.connect(self._set_labels)

        log_panel = QWidget()
        lv = QVBoxLayout(log_panel)
        lv.setContentsMargins(0, 0, 0, 0)
        self.log_view = QPlainTextEdit(readOnly=True)
        self.log_view.setMaximumBlockCount(2000)
        self.log_view.setFocusPolicy(Qt.NoFocus)
        lv.addWidget(self.log_view)
        send_row = QHBoxLayout()
        self.manual = QLineEdit(placeholderText="Type a raw command and press Enter (key bindings pause "
                                                "while typing — Esc to resume)")
        self.manual.returnPressed.connect(self._send_manual)
        clear_btn = QPushButton("Clear log")
        clear_btn.setFocusPolicy(Qt.NoFocus)
        clear_btn.clicked.connect(self.log_view.clear)
        send_row.addWidget(self.manual, 1)
        send_row.addWidget(clear_btn)
        lv.addLayout(send_row)

        split = QSplitter(Qt.Vertical)
        split.addWidget(self.pad)
        split.addWidget(log_panel)
        split.setStretchFactor(0, 4)
        split.setStretchFactor(1, 1)
        split.setSizes([600, 180])
        root.addWidget(split, 1)

        ctrl.log.connect(self.append_log)
        ctrl.connection_changed.connect(self._update_status)

        self.watcher = QFileSystemWatcher([str(ctrl.config_path)])
        self._reload_timer = QTimer(self, singleShot=True, interval=300)
        self._reload_timer.timeout.connect(self.reload_config)
        self.watcher.fileChanged.connect(lambda _p: self._reload_timer.start())

        QApplication.instance().installEventFilter(self)
        QApplication.instance().focusChanged.connect(self._focus_changed)

    # ----------------------------------------------------------- setup
    def apply_config(self, initial: bool):
        cfg = self.ctrl.cfg
        if initial:
            self.labels_box.setChecked(cfg.behavior.show_key_labels)
            self._set_labels(cfg.behavior.show_key_labels)
        if not self.ctrl.link_open:
            self.host_edit.setText(cfg.udp.host)
            self.port_spin.setValue(cfg.udp.port)
        self._update_status(self.ctrl.link_open)
        if initial and cfg.udp.auto_connect and cfg.udp.host:
            self.toggle_connection()

    def reload_config(self):
        if str(self.ctrl.config_path) not in self.watcher.files() and self.ctrl.config_path.exists():
            self.watcher.addPath(str(self.ctrl.config_path))  # editors that replace the file drop the watch
        if self.ctrl.load():
            self.apply_config(initial=False)

    def _set_labels(self, on: bool):
        self.pad.show_labels = on
        self.pad.update()

    # ------------------------------------------------------- connection
    def toggle_connection(self):
        if self.ctrl.link_open:
            self.ctrl.disconnect()
            return
        host = self.host_edit.text().strip()
        if not host:
            self.append_log("warn", "Enter the car's IP address first")
            return
        if self.ctrl.connect(host, self.port_spin.value()):
            self.pad.setFocus()

    def _update_status(self, connected: bool):
        self.connect_btn.setText("Disconnect" if connected else "Connect")
        self.connect_btn.setProperty("connected", connected)
        self.connect_btn.style().unpolish(self.connect_btn)
        self.connect_btn.style().polish(self.connect_btn)
        self.host_edit.setEnabled(not connected)
        self.port_spin.setEnabled(not connected)
        if connected:
            self.status.setText(f"<span style='color:#3fb950'>●</span> UDP → {html.escape(self.ctrl.link.description)}")
        else:
            self.status.setText("<span style='color:#e3b341'>●</span> Not connected (dry run — commands are only logged)")

    # ------------------------------------------------------------ input
    def _typing(self, w=None) -> bool:
        w = QApplication.focusWidget() if w is None else w
        return w is not None and any(w is f or f.isAncestorOf(w)
                                     for f in (self.manual, self.host_edit, self.port_spin))

    def eventFilter(self, obj, ev):
        t = ev.type()
        if t in (QEvent.KeyPress, QEvent.KeyRelease) and QApplication.activeWindow() is self:
            if self._typing():
                if t == QEvent.KeyPress and ev.key() == Qt.Key_Escape:
                    self.pad.setFocus()
                    return True
                return False
            if self.ctrl.handle_key(ev, t == QEvent.KeyPress):
                return True
        return super().eventFilter(obj, ev)

    def _focus_changed(self, _old, new):
        if self._typing(new):
            self.ctrl.release_source("key")

    def changeEvent(self, ev):
        if ev.type() == QEvent.ActivationChange and not self.isActiveWindow() and self.ctrl.cfg:
            self.pad.cancel_mouse()
            self.ctrl.release_all("window lost focus", send_stop=self.ctrl.cfg.behavior.send_stop_on_focus_loss)
        super().changeEvent(ev)

    def _send_manual(self):
        text = self.manual.text()
        if text:
            self.ctrl.send(text, "manual")
            self.manual.clear()

    def append_log(self, level: str, message: str):
        stamp = time.strftime("%H:%M:%S")
        tag = {"tx": "TX ", "rx": "RX ", "dry": "-- ", "warn": "WARN", "error": "ERR ", "info": "    "}.get(level, "")
        color = LOG_COLORS.get(level, "#e6edf3")
        self.log_view.appendHtml(f"<span style='color:#6e7681'>{stamp}</span> "
                                 f"<span style='color:{color}'><b>{tag}</b> {html.escape(message)}</span>")

    def closeEvent(self, ev):
        QApplication.instance().removeEventFilter(self)
        self.ctrl.shutdown()
        super().closeEvent(ev)


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description="DualSense-style UDP remote for an RC car")
    parser.add_argument("--config", type=Path, default=DEFAULT_CONFIG, help="path to config JSON")
    args = parser.parse_args(argv)

    app = QApplication(sys.argv[:1])
    app.setStyleSheet(STYLE)
    ctrl = RemoteController(args.config.resolve())
    win = MainWindow(ctrl)
    if not ctrl.load():
        try:
            load_config(ctrl.config_path)
        except ConfigError as e:
            QMessageBox.critical(None, "RC Remote", f"Could not load configuration:\n\n{e}")
        return 1
    win.apply_config(initial=True)
    win.show()
    win.pad.setFocus()
    return app.exec()


if __name__ == "__main__":
    sys.exit(main())
