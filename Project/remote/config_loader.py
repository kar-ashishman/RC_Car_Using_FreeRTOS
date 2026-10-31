"""Loading, validation and command encoding for the RC remote configuration."""
from __future__ import annotations

import json
import re
from dataclasses import dataclass, field, fields
from pathlib import Path

STICKS = ("left", "right")
STICK_DIRECTIONS = ("up", "down", "left", "right")

BUTTON_NAMES = (
    "dpad_up", "dpad_down", "dpad_left", "dpad_right",
    "triangle", "circle", "cross", "square",
    "l1", "l2", "l3", "r1", "r2", "r3",
    "create", "options", "ps", "touchpad", "mute",
    *(f"{s}_stick_{d}" for s in STICKS for d in STICK_DIRECTIONS),
)


class ConfigError(Exception):
    pass


@dataclass
class UdpSettings:
    host: str = "192.168.4.1"
    port: int = 4210
    local_port: int = 0
    line_ending: str = ""
    encoding: str = "ascii"
    auto_connect: bool = False
    show_rx: bool = True


@dataclass
class Behavior:
    stop_command: str = ""
    send_stop_on_focus_loss: bool = True
    heartbeat_command: str = ""
    heartbeat_interval_ms: int = 0
    show_key_labels: bool = True


@dataclass
class AnalogStick:
    command: str = ""
    interval_ms: int = 50
    range: int = 100
    invert_y: bool = True


DISPLAY_FIELDS = ("speed", "indicator", "boost")
INDICATOR_STATES = ("off", "left", "right", "hazard")


@dataclass
class DisplaySettings:
    speed_unit: str = "km/h"
    speed_max: float = 100
    speed_decimals: int = 0
    link_timeout_ms: int = 1500
    blink_ms: int = 400
    log_telemetry: bool = False
    telemetry_keys: dict = field(default_factory=lambda: {"speed": "SPD", "indicator": "IND", "boost": "BST"})


@dataclass
class ButtonBinding:
    name: str = ""
    key: str = ""
    press: str = ""
    release: str = ""
    repeat_ms: int = 0
    description: str = ""
    display_press: dict = field(default_factory=dict)
    display_release: dict = field(default_factory=dict)


@dataclass
class RemoteConfig:
    path: Path
    udp: UdpSettings
    behavior: Behavior
    display: DisplaySettings
    sticks: dict[str, AnalogStick]
    buttons: dict[str, ButtonBinding]
    warnings: list[str] = field(default_factory=list)


def _coerce(value, default, where: str):
    target = type(default)
    if target is bool:
        if isinstance(value, bool):
            return value
        raise ConfigError(f"{where}: expected true/false, got {value!r}")
    if target in (int, float):
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ConfigError(f"{where}: expected a number, got {value!r}")
        return target(value) if target is float or float(value).is_integer() else value
    if target is str:
        if not isinstance(value, str):
            raise ConfigError(f"{where}: expected a string, got {value!r}")
        return value
    if target is dict and not isinstance(value, dict):
        raise ConfigError(f"{where}: expected an object, got {value!r}")
    return value


def _check_display_action(action: dict, where: str) -> dict:
    """Validate a button's display action, e.g. {"boost": true} or {"speed": "+5"}."""
    clean = {}
    for key, value in action.items():
        if key.startswith("_"):
            continue
        if key == "boost":
            if value != "toggle" and not isinstance(value, bool):
                raise ConfigError(f"{where}.boost: use true, false or \"toggle\"")
        elif key == "indicator":
            if not isinstance(value, str) or value.lower() not in INDICATOR_STATES:
                raise ConfigError(f"{where}.indicator: use one of {', '.join(INDICATOR_STATES)}")
            value = value.lower()
        elif key == "speed":
            ok = (isinstance(value, (int, float)) and not isinstance(value, bool)) or (
                isinstance(value, str) and re.fullmatch(r"[+-]\d+(\.\d+)?", value.strip()))
            if not ok:
                raise ConfigError(f"{where}.speed: use a number (set) or a string like \"+5\" / \"-5\" (change)")
            value = value.strip() if isinstance(value, str) else value
        else:
            raise ConfigError(f"{where}: unknown display field '{key}' (valid: {', '.join(DISPLAY_FIELDS)})")
        clean[key] = value
    return clean


def _fill(cls, data, section: str, warnings: list[str], **fixed):
    if data is None:
        data = {}
    if not isinstance(data, dict):
        raise ConfigError(f"'{section}' must be an object")
    obj = cls(**fixed)
    known = {f.name for f in fields(cls)} - set(fixed)
    for key, value in data.items():
        if key.startswith("_"):
            continue  # "_comment" style keys are allowed
        if key not in known:
            warnings.append(f"Unknown setting '{section}.{key}' ignored")
            continue
        setattr(obj, key, _coerce(value, getattr(obj, key), f"{section}.{key}"))
    return obj


def load_config(path: str | Path) -> RemoteConfig:
    path = Path(path)
    try:
        raw = json.loads(path.read_text(encoding="utf-8"))
    except FileNotFoundError:
        raise ConfigError(f"Config file not found: {path}") from None
    except json.JSONDecodeError as e:
        raise ConfigError(f"Invalid JSON in {path.name} (line {e.lineno}, col {e.colno}): {e.msg}") from None
    if not isinstance(raw, dict):
        raise ConfigError("Top level of the config must be an object")

    warnings: list[str] = []
    for key in raw:
        if key == "serial":
            warnings.append("Section 'serial' is no longer used; configure 'udp' instead")
        elif key not in ("udp", "behavior", "display", "analog_sticks", "buttons") and not key.startswith("_"):
            warnings.append(f"Unknown section '{key}' ignored")

    udp = _fill(UdpSettings, raw.get("udp"), "udp", warnings)
    udp.host = udp.host.strip()
    if not isinstance(udp.port, int) or not 1 <= udp.port <= 65535:
        raise ConfigError("udp.port must be a whole number from 1 to 65535")
    if not isinstance(udp.local_port, int) or not 0 <= udp.local_port <= 65535:
        raise ConfigError("udp.local_port must be a whole number from 0 to 65535 (0 = any)")
    try:
        "".encode(udp.encoding)
    except LookupError:
        raise ConfigError(f"udp.encoding: unknown encoding {udp.encoding!r}") from None

    behavior = _fill(Behavior, raw.get("behavior"), "behavior", warnings)

    display = _fill(DisplaySettings, raw.get("display"), "display", warnings)
    if display.speed_max <= 0:
        raise ConfigError("display.speed_max must be positive")
    if not isinstance(display.speed_decimals, int) or not 0 <= display.speed_decimals <= 3:
        raise ConfigError("display.speed_decimals must be 0, 1, 2 or 3")
    display.blink_ms = max(100, display.blink_ms)
    display.link_timeout_ms = max(0, display.link_timeout_ms)
    tkeys = DisplaySettings().telemetry_keys
    for key, value in display.telemetry_keys.items():
        if key.startswith("_"):
            continue
        if key not in DISPLAY_FIELDS:
            raise ConfigError(f"display.telemetry_keys: unknown field '{key}' (valid: {', '.join(DISPLAY_FIELDS)})")
        if not isinstance(value, str) or not re.fullmatch(r"[A-Za-z_]\w*|", value):
            raise ConfigError(f"display.telemetry_keys.{key}: must be a word like \"SPD\" (or \"\" to disable)")
        tkeys[key] = value
    display.telemetry_keys = tkeys

    sticks_raw = raw.get("analog_sticks") or {}
    if not isinstance(sticks_raw, dict):
        raise ConfigError("'analog_sticks' must be an object")
    sticks = {}
    for stick in STICKS:
        st = _fill(AnalogStick, sticks_raw.get(stick), f"analog_sticks.{stick}", warnings)
        if st.range <= 0:
            raise ConfigError(f"analog_sticks.{stick}.range must be positive")
        st.interval_ms = max(10, st.interval_ms)
        sticks[stick] = st
    for key in sticks_raw:
        if key not in STICKS and not key.startswith("_"):
            warnings.append(f"Unknown stick 'analog_sticks.{key}' ignored")

    buttons_raw = raw.get("buttons") or {}
    if not isinstance(buttons_raw, dict):
        raise ConfigError("'buttons' must be an object")
    buttons = {}
    for name in BUTTON_NAMES:
        b = _fill(ButtonBinding, buttons_raw.get(name), f"buttons.{name}", warnings, name=name)
        b.repeat_ms = max(0, b.repeat_ms)
        if 0 < b.repeat_ms < 20:
            b.repeat_ms = 20
        b.display_press = _check_display_action(b.display_press, f"buttons.{name}.display_press")
        b.display_release = _check_display_action(b.display_release, f"buttons.{name}.display_release")
        buttons[name] = b
    for key in buttons_raw:
        if key not in BUTTON_NAMES and not key.startswith("_"):
            warnings.append(f"Unknown button 'buttons.{key}' ignored (valid: {', '.join(BUTTON_NAMES)})")

    return RemoteConfig(path, udp, behavior, display, sticks, buttons, warnings)


def encode_command(template: str, udp: UdpSettings, **values) -> bytes:
    """Turn a configured command into the payload of one UDP packet.

    * Plain text gets ``udp.line_ending`` appended and is encoded with ``udp.encoding``.
    * ``hex:AA 01 FF`` sends raw bytes (no line ending).
    * When ``values`` are given (analog sticks) ``{x}``-style placeholders are filled in first.
    """
    if not template:
        return b""
    text = template.format(**values) if values else template
    if text[:4].lower() == "hex:":
        tokens = [t for t in re.split(r"[\s,]+", text[4:]) if t]
        tokens = [t[2:] if t.lower().startswith("0x") else t for t in tokens]
        return bytes.fromhex("".join(tokens))
    return (text + udp.line_ending).encode(udp.encoding)
