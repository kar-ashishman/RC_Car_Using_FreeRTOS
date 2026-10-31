"""A DualSense-style controller drawn with QPainter.

The widget is purely visual/interactive; it forwards presses to a controller object that
provides: is_active(name), stick_position(stick), key_label(name), tooltip_for(name),
all_buttons(), link_status(), display (speed/indicator/boost), display_settings,
press(name, source), release(name, source), set_stick_mouse(stick, x, y),
clear_stick_mouse(stick) and a ``changed`` signal.

The touchpad doubles as the remote's display (indicators, speed, boost, link status).
"""
from __future__ import annotations

import math

from PySide6.QtCore import QEvent, QPointF, QRectF, QSize, Qt, QTimer
from PySide6.QtGui import (
    QBrush, QColor, QFont, QFontMetricsF, QLinearGradient, QPainter, QPainterPath, QPen,
    QPolygonF, QRadialGradient, QTransform,
)
from PySide6.QtWidgets import QToolTip, QWidget

BASE_W, BASE_H = 1000.0, 620.0

ACCENT = QColor("#2f7bff")
WHITE_TOP, WHITE_BOT = QColor("#fbfcfd"), QColor("#d6dae0")
OUTLINE = QColor("#a9afb9")
DARK_TOP, DARK_BOT = QColor("#2c2f36"), QColor("#101216")
SYMBOL_GREY = QColor("#b7bcc5")
SYMBOL_COLORS = {
    "triangle": QColor("#39d0a5"),
    "circle": QColor("#ff5a6e"),
    "cross": QColor("#6aa8ff"),
    "square": QColor("#f38bd6"),
}

STICK_CENTERS = {"left": QPointF(390, 322), "right": QPointF(610, 322)}
STICK_BASE_R = 50.0
KNOB_R = 36.0
KNOB_TRAVEL = 16.0   # how far the knob is drawn from centre at full deflection
DRAG_TRAVEL = 45.0   # mouse distance (base units) for full deflection

DPAD_CENTER = QPointF(228, 212)
FACE_CENTER = QPointF(772, 212)
FACE_OFFSET = 58.0
FACE_R = 26.0

KEY_GLYPHS = {"up": "↑", "down": "↓", "left": "←", "right": "→", "space": "Space"}

SCREEN = QRectF(364, 106, 272, 138)
STATUS_COLORS = {"online": "#3ddc84", "open": "#2f7bff", "waiting": "#ffb340", "offline": "#ff5a4f"}
STATUS_TEXT = {"online": "ONLINE", "open": "UDP OPEN", "waiting": "NO REPLY", "offline": "OFFLINE"}
STATUS_BARS = {"online": 4, "open": 3, "waiting": 1, "offline": 0}


def _mirror(path: QPainterPath) -> QPainterPath:
    return QTransform(-1, 0, 0, 1, BASE_W, 0).map(path)


class DualSenseWidget(QWidget):
    def __init__(self, controller, parent=None):
        super().__init__(parent)
        self.ctrl = controller
        self.show_labels = True
        self._drag_stick: str | None = None
        self._mouse_button: str | None = None
        self.setMinimumSize(560, 350)
        self.setFocusPolicy(Qt.StrongFocus)
        self._build_geometry()
        self._blink_on = True
        self._blink = QTimer(self)
        self._blink.timeout.connect(self._toggle_blink)
        controller.changed.connect(self._sync_blink)
        controller.changed.connect(self.update)

    def sizeHint(self):
        return QSize(int(BASE_W), int(BASE_H))

    # ------------------------------------------------------------------ geometry
    def _build_geometry(self):
        body = QPainterPath()
        body.addRoundedRect(QRectF(120, 92, 760, 255), 115, 115)
        mid = QPainterPath()
        mid.addEllipse(QPointF(500, 335), 215, 88)
        grip = QPainterPath()
        grip.addEllipse(QPointF(0, 0), 112, 190)
        left_grip = QTransform().translate(222, 398).rotate(19).map(grip)
        right_grip = QTransform().translate(778, 398).rotate(-19).map(grip)
        self.body = body.united(mid).united(left_grip).united(right_grip).simplified()

        c = QPainterPath()
        c.moveTo(330, 205)
        c.lineTo(670, 205)
        c.cubicTo(725, 265, 735, 360, 700, 405)
        c.quadTo(500, 445, 300, 405)
        c.cubicTo(265, 360, 275, 265, 330, 205)
        c.closeSubpath()
        self.center_panel = c.intersected(self.body)

        tp = QPainterPath()
        tp.moveTo(345, 96)
        tp.lineTo(655, 96)
        tp.lineTo(644, 238)
        tp.quadTo(642, 255, 624, 255)
        tp.lineTo(376, 255)
        tp.quadTo(358, 255, 356, 238)
        tp.closeSubpath()
        self.touchpad = tp.intersected(self.body)

        arrow = QPainterPath()
        arrow.addPolygon(QPolygonF([QPointF(378, 124), QPointF(393, 112), QPointF(393, 119.5),
                                    QPointF(408, 119.5), QPointF(408, 128.5), QPointF(393, 128.5),
                                    QPointF(393, 136)]))
        arrow.closeSubpath()
        self.ind_left = arrow
        self.ind_right = _mirror(arrow)

        l1 = QPainterPath()
        l1.addRoundedRect(QRectF(168, 66, 152, 36), 16, 16)
        l2 = QPainterPath()
        l2.moveTo(185, 74)
        l2.cubicTo(185, 30, 205, 18, 245, 18)
        l2.cubicTo(285, 18, 305, 30, 305, 74)
        l2.closeSubpath()
        self.shoulders = {"l2": l2, "r2": _mirror(l2), "l1": l1, "r1": _mirror(l1)}

        piece = QPainterPath()
        piece.addPolygon(QPolygonF([QPointF(-19, -70), QPointF(19, -70), QPointF(19, -32),
                                    QPointF(0, -14), QPointF(-19, -32)]))
        piece.closeSubpath()
        arrow = QPainterPath()
        arrow.addPolygon(QPolygonF([QPointF(0, -58), QPointF(-7, -47), QPointF(7, -47)]))
        arrow.closeSubpath()
        self.dpad, self.dpad_arrows = {}, {}
        for name, angle in (("dpad_up", 0), ("dpad_right", 90), ("dpad_down", 180), ("dpad_left", 270)):
            t = QTransform().translate(DPAD_CENTER.x(), DPAD_CENTER.y()).rotate(angle)
            self.dpad[name] = t.map(piece)
            self.dpad_arrows[name] = t.map(arrow)

        fx, fy = FACE_CENTER.x(), FACE_CENTER.y()
        self.face_centers = {
            "triangle": QPointF(fx, fy - FACE_OFFSET),
            "circle": QPointF(fx + FACE_OFFSET, fy),
            "cross": QPointF(fx, fy + FACE_OFFSET),
            "square": QPointF(fx - FACE_OFFSET, fy),
        }
        self.face = {}
        for name, pt in self.face_centers.items():
            p = QPainterPath()
            p.addEllipse(pt, FACE_R, FACE_R)
            self.face[name] = p

        self.small = {}
        create = QPainterPath()
        create.addRoundedRect(QRectF(316, 122, 13, 34), 6.5, 6.5)
        self.small["create"] = create
        self.small["options"] = _mirror(create)
        ps = QPainterPath()
        ps.addEllipse(QPointF(500, 322), 18, 18)
        self.small["ps"] = ps
        mute = QPainterPath()
        mute.addRoundedRect(QRectF(484, 360, 32, 11), 5.5, 5.5)
        self.small["mute"] = mute

        # Hit testing, top-most first. Shoulders exclude the parts hidden behind other shapes.
        hit = list(self.face.items()) + list(self.dpad.items()) + list(self.small.items())
        hit.append(("touchpad", self.touchpad))
        for side in ("l", "r"):
            one, two = self.shoulders[f"{side}1"], self.shoulders[f"{side}2"]
            hit.append((f"{side}1", one.subtracted(self.body)))
            hit.append((f"{side}2", two.subtracted(one).subtracted(self.body)))
        self._hit_order = hit

        d = DPAD_CENTER
        self.label_pos = {
            "dpad_up": QPointF(d.x(), d.y() - 86), "dpad_down": QPointF(d.x(), d.y() + 86),
            "dpad_left": QPointF(d.x() - 88, d.y()), "dpad_right": QPointF(d.x() + 88, d.y()),
            "triangle": QPointF(fx, fy - 102), "cross": QPointF(fx, fy + 102),
            "square": QPointF(fx - 102, fy), "circle": QPointF(fx + 102, fy),
            "l1": QPointF(244, 92), "l2": QPointF(245, 44),
            "r1": QPointF(BASE_W - 244, 92), "r2": QPointF(BASE_W - 245, 44),
            "create": QPointF(322, 174), "options": QPointF(BASE_W - 322, 174),
            "touchpad": QPointF(500, 264), "ps": QPointF(500, 290), "mute": QPointF(500, 392),
        }
        for stick, c in STICK_CENTERS.items():
            self.label_pos[f"{stick}_stick_up"] = QPointF(c.x(), c.y() - 64)
            self.label_pos[f"{stick}_stick_down"] = QPointF(c.x(), c.y() + 64)
            self.label_pos[f"{stick}_stick_left"] = QPointF(c.x() - 66, c.y())
            self.label_pos[f"{stick}_stick_right"] = QPointF(c.x() + 66, c.y())
            dx = -50 if stick == "left" else 50
            self.label_pos["l3" if stick == "left" else "r3"] = QPointF(c.x() + dx, c.y() + 50)

    # ---------------------------------------------------------------- transforms
    def _xform(self) -> QTransform:
        s = min(self.width() / BASE_W, self.height() / BASE_H)
        ox = (self.width() - BASE_W * s) / 2
        oy = (self.height() - BASE_H * s) / 2
        return QTransform(s, 0, 0, s, ox, oy)

    def _to_base(self, pos) -> QPointF:
        inv, _ok = self._xform().inverted()
        return inv.map(QPointF(pos))

    def _stick_at(self, p: QPointF) -> str | None:
        for stick, c in STICK_CENTERS.items():
            if math.hypot(p.x() - c.x(), p.y() - c.y()) <= STICK_BASE_R:
                return stick
        return None

    def _button_at(self, p: QPointF) -> str | None:
        for name, path in self._hit_order:
            if path.contains(p):
                return name
        return None

    # --------------------------------------------------------------------- mouse
    def mousePressEvent(self, e):
        p = self._to_base(e.position())
        stick = self._stick_at(p)
        if stick:
            if e.button() == Qt.RightButton:
                self._mouse_button = "l3" if stick == "left" else "r3"
                self.ctrl.press(self._mouse_button, "mouse")
            elif e.button() == Qt.LeftButton:
                self._drag_stick = stick
                self._update_drag(p)
            return
        if e.button() == Qt.LeftButton:
            name = self._button_at(p)
            if name:
                self._mouse_button = name
                self.ctrl.press(name, "mouse")

    def mouseMoveEvent(self, e):
        if self._drag_stick:
            self._update_drag(self._to_base(e.position()))

    def mouseReleaseEvent(self, e):
        if self._drag_stick and e.button() == Qt.LeftButton:
            stick, self._drag_stick = self._drag_stick, None
            self.ctrl.clear_stick_mouse(stick)
        if self._mouse_button:
            name, self._mouse_button = self._mouse_button, None
            self.ctrl.release(name, "mouse")

    def _update_drag(self, p: QPointF):
        c = STICK_CENTERS[self._drag_stick]
        x, y = (p.x() - c.x()) / DRAG_TRAVEL, (p.y() - c.y()) / DRAG_TRAVEL
        length = math.hypot(x, y)
        if length > 1:
            x, y = x / length, y / length
        self.ctrl.set_stick_mouse(self._drag_stick, x, y)

    def cancel_mouse(self):
        """Release anything held with the mouse (e.g. when the window loses focus)."""
        self._drag_stick = None
        self._mouse_button = None

    def event(self, e):
        if e.type() == QEvent.ToolTip:
            p = self._to_base(e.position() if hasattr(e, "position") else e.pos())
            stick = self._stick_at(p)
            text = self.ctrl.tooltip_for(f"{stick}_stick") if stick else None
            if not text:
                name = self._button_at(p)
                text = self.ctrl.tooltip_for(name) if name else None
            if text:
                QToolTip.showText(e.globalPos(), text, self)
            else:
                QToolTip.hideText()
                e.ignore()
            return True
        return super().event(e)

    # --------------------------------------------------------------------- paint
    def paintEvent(self, _e):
        p = QPainter(self)
        p.setRenderHint(QPainter.Antialiasing)
        p.setRenderHint(QPainter.TextAntialiasing)
        p.fillRect(self.rect(), QColor("#0f1115"))
        p.setTransform(self._xform())
        active = self.ctrl.is_active

        self._paint_shoulders(p, active)

        for i in range(7):
            p.fillPath(self.body.translated(0, 5 + i * 2), QColor(0, 0, 0, 16))
        g = QLinearGradient(0, 90, 0, 590)
        g.setColorAt(0, WHITE_TOP)
        g.setColorAt(0.55, QColor("#eceef2"))
        g.setColorAt(1, WHITE_BOT)
        p.fillPath(self.body, g)
        p.setPen(QPen(OUTLINE, 1.6))
        p.drawPath(self.body)

        g = QLinearGradient(0, 205, 0, 430)
        g.setColorAt(0, DARK_TOP)
        g.setColorAt(1, DARK_BOT)
        p.fillPath(self.center_panel, g)

        self._paint_touchpad(p, active("touchpad"))
        self._paint_dpad(p, active)
        self._paint_face(p, active)
        self._paint_small(p, active)
        for stick in STICK_CENTERS:
            self._paint_stick(p, stick, active)
        if self.show_labels:
            self._paint_labels(p, active)
        p.end()

    def _paint_shoulders(self, p, active):
        for name in ("l2", "r2", "l1", "r1"):
            path = self.shoulders[name]
            rect = path.boundingRect()
            g = QLinearGradient(0, rect.top(), 0, rect.bottom())
            if active(name):
                g.setColorAt(0, ACCENT.lighter(130))
                g.setColorAt(1, ACCENT.darker(130))
            else:
                g.setColorAt(0, QColor("#3a3e46"))
                g.setColorAt(1, QColor("#16181c"))
            p.fillPath(path, g)
            p.setPen(QPen(QColor("#0b0c0f"), 1.2))
            p.drawPath(path)
            p.setPen(QPen(QColor(255, 255, 255, 40), 1.2))
            p.drawLine(QPointF(rect.left() + 18, rect.top() + 4), QPointF(rect.right() - 18, rect.top() + 4))

    def _paint_touchpad(self, p, pressed):
        g = QLinearGradient(0, 96, 0, 255)
        g.setColorAt(0, QColor("#f4f5f8"))
        g.setColorAt(1, QColor("#dfe2e7"))
        p.fillPath(self.touchpad, g)
        if pressed:
            p.fillPath(self.touchpad, QColor(ACCENT.red(), ACCENT.green(), ACCENT.blue(), 70))
        p.setPen(QPen(QColor("#bfc4cc"), 1.4))
        p.drawPath(self.touchpad)
        self._paint_screen(p, pressed)

        color = QColor(STATUS_COLORS[self.ctrl.link_status()])
        if not any(self.ctrl.is_active(n) for n in self.ctrl.all_buttons()):
            color.setAlpha(170)
        for width, alpha in ((12, 40), (7, 90), (3, 255)):
            c = QColor(color)
            c.setAlpha(min(alpha, color.alpha()))
            p.setPen(QPen(c, width, Qt.SolidLine, Qt.RoundCap))
            p.drawLine(QPointF(340, 112), QPointF(349, 236))
            p.drawLine(QPointF(BASE_W - 340, 112), QPointF(BASE_W - 349, 236))

    # ------------------------------------------------------------- display
    def _sync_blink(self):
        on = self.ctrl.display.indicator != "off"
        interval = self.ctrl.display_settings.blink_ms
        if on and (not self._blink.isActive() or self._blink.interval() != interval):
            self._blink_on = True
            self._blink.start(interval)
        elif not on and self._blink.isActive():
            self._blink.stop()
            self._blink_on = True

    def _toggle_blink(self):
        self._blink_on = not self._blink_on
        self.update()

    def _paint_screen(self, p, pressed):
        state, ds, status = self.ctrl.display, self.ctrl.display_settings, self.ctrl.link_status()
        screen = QPainterPath()
        screen.addRoundedRect(SCREEN, 14, 14)
        g = QLinearGradient(0, SCREEN.top(), 0, SCREEN.bottom())
        g.setColorAt(0, QColor("#18202a"))
        g.setColorAt(1, QColor("#06080b"))
        p.fillPath(screen, g)
        p.save()
        p.setClipPath(screen)

        # Indicators
        for side, path in (("left", self.ind_left), ("right", self.ind_right)):
            lit = state.indicator in (side, "hazard") and self._blink_on
            if lit:
                p.setPen(QPen(QColor(61, 220, 132, 70), 9, Qt.SolidLine, Qt.RoundCap, Qt.RoundJoin))
                p.setBrush(Qt.NoBrush)
                p.drawPath(path)
                p.fillPath(path, QColor("#3ddc84"))
            else:
                p.setPen(QPen(QColor("#2c4636"), 1.2))
                p.setBrush(QColor("#13201a"))
                p.drawPath(path)

        # Connection status: signal bars + text
        color = QColor(STATUS_COLORS[status])
        text = STATUS_TEXT[status]
        f = QFont()
        f.setPixelSize(10)
        f.setBold(True)
        f.setLetterSpacing(QFont.AbsoluteSpacing, 0.6)
        p.setFont(f)
        tw = QFontMetricsF(f).horizontalAdvance(text)
        x = 500 - (22 + 5 + tw) / 2
        bars = STATUS_BARS[status]
        for i in range(4):
            h = 4 + i * 3
            p.fillRect(QRectF(x + i * 5.5, 131 - h, 3.5, h), color if i < bars else QColor("#2a313b"))
        if status == "offline":
            p.setPen(QPen(color, 1.6, Qt.SolidLine, Qt.RoundCap))
            p.drawLine(QPointF(x + 14, 117), QPointF(x + 21, 124))
            p.drawLine(QPointF(x + 21, 117), QPointF(x + 14, 124))
        p.setPen(color)
        p.drawText(QRectF(x + 27, 116, tw + 4, 16), Qt.AlignLeft | Qt.AlignVCenter, text)

        # Speed
        speed = state.speed
        f = QFont()
        f.setPixelSize(44)
        f.setBold(True)
        p.setFont(f)
        if status == "waiting" and speed is not None:
            p.setPen(QColor("#6b7280"))  # stale: car stopped replying
        else:
            p.setPen(QColor("#ff9a3c") if state.boost else QColor("#f2f5f8"))
        p.drawText(QRectF(400, 134, 200, 50), Qt.AlignCenter,
                   "--" if speed is None else f"{speed:.{ds.speed_decimals}f}")
        f = QFont()
        f.setPixelSize(11)
        f.setBold(True)
        p.setFont(f)
        p.setPen(QColor("#8b949e"))
        p.drawText(QRectF(400, 183, 200, 14), Qt.AlignCenter, ds.speed_unit)

        bar = QRectF(388, 203, 224, 6)
        p.setPen(Qt.NoPen)
        p.setBrush(QColor("#1f2630"))
        p.drawRoundedRect(bar, 3, 3)
        frac = 0.0 if speed is None else min(abs(speed) / ds.speed_max, 1.0)
        if frac > 0:
            g = QLinearGradient(bar.left(), 0, bar.right(), 0)
            g.setColorAt(0, QColor("#3ddc84"))
            g.setColorAt(0.65, QColor("#ffd23f"))
            g.setColorAt(1, QColor("#ff4d4d"))
            p.setBrush(g)
            p.drawRoundedRect(QRectF(bar.left(), bar.top(), max(bar.width() * frac, 6), bar.height()), 3, 3)

        # Boost
        pill = QRectF(450, 217, 100, 20)
        f = QFont()
        f.setPixelSize(11)
        f.setBold(True)
        f.setLetterSpacing(QFont.AbsoluteSpacing, 0.8)
        p.setFont(f)
        if state.boost:
            for grow, alpha in ((6, 40), (3, 80)):
                p.setPen(Qt.NoPen)
                p.setBrush(QColor(255, 106, 42, alpha))
                p.drawRoundedRect(pill.adjusted(-grow, -grow, grow, grow), 10 + grow, 10 + grow)
            g = QLinearGradient(pill.left(), 0, pill.right(), 0)
            g.setColorAt(0, QColor("#ff8a2a"))
            g.setColorAt(1, QColor("#ff3d3d"))
            p.setBrush(g)
            p.drawRoundedRect(pill, 10, 10)
            p.setPen(QColor("#ffffff"))
            p.drawText(pill, Qt.AlignCenter, "BOOST ON")
        else:
            p.setPen(QPen(QColor("#3a414c"), 1.2))
            p.setBrush(Qt.NoBrush)
            p.drawRoundedRect(pill, 10, 10)
            p.setPen(QColor("#5c6470"))
            p.drawText(pill, Qt.AlignCenter, "BOOST OFF")

        # Glass sheen
        sheen = QPainterPath()
        sheen.moveTo(SCREEN.left(), SCREEN.top())
        sheen.lineTo(SCREEN.right(), SCREEN.top())
        sheen.lineTo(SCREEN.right(), SCREEN.top() + 26)
        sheen.quadTo(500, SCREEN.top() + 52, SCREEN.left(), SCREEN.top() + 38)
        sheen.closeSubpath()
        p.fillPath(sheen, QColor(255, 255, 255, 10))
        if pressed:
            p.fillPath(screen, QColor(ACCENT.red(), ACCENT.green(), ACCENT.blue(), 40))
        p.restore()
        p.setPen(QPen(QColor("#8e959f"), 1.5))
        p.setBrush(Qt.NoBrush)
        p.drawPath(screen)

    def _paint_dpad(self, p, active):
        for name, path in self.dpad.items():
            on = active(name)
            rect = path.boundingRect()
            g = QLinearGradient(rect.topLeft(), rect.bottomRight())
            if on:
                g.setColorAt(0, ACCENT.lighter(125))
                g.setColorAt(1, ACCENT.darker(120))
            else:
                g.setColorAt(0, QColor("#3a3d44"))
                g.setColorAt(1, QColor("#1b1d21"))
            p.setPen(QPen(QColor("#121316"), 3, Qt.SolidLine, Qt.RoundCap, Qt.RoundJoin))
            p.setBrush(QBrush(g))
            p.drawPath(path)
            p.fillPath(self.dpad_arrows[name], QColor("#ffffff") if on else QColor("#6b7079"))

    def _paint_face(self, p, active):
        for name, path in self.face.items():
            c = self.face_centers[name]
            on = active(name)
            color = SYMBOL_COLORS[name]
            if on:
                glow = QRadialGradient(c, FACE_R + 16)
                glow.setColorAt(0.55, QColor(color.red(), color.green(), color.blue(), 150))
                glow.setColorAt(1, QColor(color.red(), color.green(), color.blue(), 0))
                p.setPen(Qt.NoPen)
                p.setBrush(glow)
                p.drawEllipse(c, FACE_R + 16, FACE_R + 16)
            g = QRadialGradient(c + QPointF(-8, -10), FACE_R * 1.6)
            g.setColorAt(0, QColor("#41454d") if not on else color.darker(150))
            g.setColorAt(1, QColor("#14161a"))
            p.setBrush(g)
            p.setPen(QPen(QColor("#0c0d10"), 1.5))
            p.drawPath(path)

            pen = QPen(QColor("#ffffff") if on else SYMBOL_GREY, 3.2, Qt.SolidLine, Qt.RoundCap, Qt.RoundJoin)
            p.setPen(pen)
            p.setBrush(Qt.NoBrush)
            s = 11.0
            if name == "triangle":
                p.drawPolygon(QPolygonF([c + QPointF(0, -s), c + QPointF(s * 0.95, s * 0.7),
                                         c + QPointF(-s * 0.95, s * 0.7)]))
            elif name == "circle":
                p.drawEllipse(c, s * 0.95, s * 0.95)
            elif name == "cross":
                p.drawLine(c + QPointF(-s * 0.8, -s * 0.8), c + QPointF(s * 0.8, s * 0.8))
                p.drawLine(c + QPointF(s * 0.8, -s * 0.8), c + QPointF(-s * 0.8, s * 0.8))
            else:
                p.drawRect(QRectF(c.x() - s * 0.8, c.y() - s * 0.8, s * 1.6, s * 1.6))

    def _paint_small(self, p, active):
        for name in ("create", "options"):
            path = self.small[name]
            p.setPen(QPen(QColor("#0f1013"), 1.2))
            p.setBrush(ACCENT if active(name) else QColor("#2a2d33"))
            p.drawPath(path)

        ps = self.small["ps"]
        on = active("ps")
        g = QRadialGradient(QPointF(494, 316), 26)
        g.setColorAt(0, ACCENT.lighter(120) if on else QColor("#3a3e46"))
        g.setColorAt(1, ACCENT.darker(140) if on else QColor("#121418"))
        p.setBrush(g)
        p.setPen(QPen(QColor("#4a4e57"), 1.2))
        p.drawPath(ps)
        f = QFont()
        f.setPixelSize(13)
        f.setBold(True)
        f.setItalic(True)
        p.setFont(f)
        p.setPen(QColor("#ffffff") if on else QColor("#c9cdd4"))
        p.drawText(ps.boundingRect(), Qt.AlignCenter, "PS")

        mute = self.small["mute"]
        on = active("mute")
        p.setPen(QPen(QColor("#4a4e57"), 1))
        p.setBrush(QColor("#ff8a2a") if on else QColor("#23262b"))
        p.drawPath(mute)

    def _paint_stick(self, p, stick, active):
        c = STICK_CENTERS[stick]
        g = QRadialGradient(c, STICK_BASE_R)
        g.setColorAt(0, QColor("#050607"))
        g.setColorAt(0.85, QColor("#141619"))
        g.setColorAt(1, QColor("#2a2d33"))
        p.setPen(QPen(QColor("#3a3d44"), 1.2))
        p.setBrush(g)
        p.drawEllipse(c, STICK_BASE_R, STICK_BASE_R)

        r = STICK_BASE_R - 4
        arc_rect = QRectF(c.x() - r, c.y() - r, 2 * r, 2 * r)
        for d, angle in (("right", 0), ("up", 90), ("left", 180), ("down", 270)):
            if active(f"{stick}_stick_{d}"):
                p.setPen(QPen(ACCENT, 5, Qt.SolidLine, Qt.RoundCap))
                p.drawArc(arc_rect, int((angle - 30) * 16), int(60 * 16))

        x, y = self.ctrl.stick_position(stick)
        k = c + QPointF(x * KNOB_TRAVEL, y * KNOB_TRAVEL)
        p.setPen(Qt.NoPen)
        p.setBrush(QColor(0, 0, 0, 110))
        p.drawEllipse(k + QPointF(0, 4), KNOB_R, KNOB_R)

        click = active("l3" if stick == "left" else "r3")
        g = QRadialGradient(k + QPointF(-10, -12), KNOB_R * 1.5)
        g.setColorAt(0, QColor("#4a4e57"))
        g.setColorAt(1, QColor("#16181c"))
        p.setBrush(g)
        p.setPen(QPen(ACCENT if click else QColor("#0c0d10"), 3 if click else 1.5))
        p.drawEllipse(k, KNOB_R, KNOB_R)

        dash = QPen(QColor("#2a2d33"), 3)
        dash.setDashPattern([1.2, 1.4])
        p.setPen(dash)
        p.setBrush(Qt.NoBrush)
        p.drawEllipse(k, KNOB_R - 4, KNOB_R - 4)

        g = QRadialGradient(k + QPointF(6, 8), 28)
        g.setColorAt(0, QColor("#30333a"))
        g.setColorAt(1, QColor("#141619"))
        p.setBrush(g)
        p.setPen(QPen(QColor("#4b4f58"), 1))
        p.drawEllipse(k, 25, 25)

    def _paint_labels(self, p, active):
        f = QFont()
        f.setPixelSize(12)
        f.setBold(True)
        p.setFont(f)
        fm = QFontMetricsF(f)
        for name, pos in self.label_pos.items():
            text = self.ctrl.key_label(name)
            if not text:
                continue
            text = KEY_GLYPHS.get(text.lower(), text)
            w = max(20.0, fm.horizontalAdvance(text) + 10)
            rect = QRectF(pos.x() - w / 2, pos.y() - 9, w, 18)
            p.setPen(QPen(QColor(255, 255, 255, 70), 1))
            p.setBrush(ACCENT if active(name) else QColor(18, 20, 24, 215))
            p.drawRoundedRect(rect, 5, 5)
            p.setPen(QColor("#ffffff"))
            p.drawText(rect, Qt.AlignCenter, text)
