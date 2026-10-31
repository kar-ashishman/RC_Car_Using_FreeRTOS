# RC Remote (DualSense style)

A Python GUI remote that looks like a PS5 DualSense controller. Each button sends a
configurable command to the car as a **UDP packet** over Wi-Fi. Each button can be pressed from
the keyboard, by clicking it, or by dragging the analog sticks.

## Setup

```powershell
py -3.13 -m venv .venv
.\.venv\Scripts\python.exe -m pip install -r requirements.txt
.\.venv\Scripts\python.exe rc_remote.py                 # uses config.json next to the script
.\.venv\Scripts\python.exe rc_remote.py --config my_car.json
```

Enter the car's IP address and UDP port in the top bar and click **Connect**. UDP has no real
connection: **Connect** just opens a socket, and every command is then sent as one packet to that
address. Replies sent by the car to the remote's address and port are shown in the log as `RX`.
While the remote isn't connected, it runs in **dry-run** mode: commands are only written to the log,
and the light bar is amber instead of blue.

Keyboard bindings pause while you type in the IP, port or raw command boxes. Press Esc (or Enter
in the IP box) to go back to driving.

## Using the remote

| Action | How |
|---|---|
| Press a button | Mapped key, or left-click it |
| Analog sticks | Drag with the left mouse button, or use the mapped direction keys |
| L3 / R3 (stick click) | Mapped key, or right-click the stick |
| See a button's mapping | Hover over it |
| Send a raw command | Type it in the box under the log and press Enter (Esc returns to key control) |

Safety: if the window loses focus, the remote releases every held button, which sends their
`release` commands, and then sends `behavior.stop_command`. It also sends the stop command when it
disconnects or closes.

## Display (touchpad screen)

The touchpad acts as the remote's dashboard:

| Item | Shows |
|---|---|
| ◀ ▶ arrows (top corners) | Left/right indicator, blinking green. Both arrows blink for hazard lights. |
| Signal bars + text (top) | Link status: **ONLINE** (the car replied within `link_timeout_ms`), **NO REPLY** (link open but the car is silent), **OFFLINE** (not connected), **UDP OPEN** (`link_timeout_ms` is 0, so replies aren't expected). The light bars on either side use the same colour. |
| Big number + bar | Vehicle speed and unit. The bar fills up to `display.speed_max`. It shows `--` until a value is known, and turns grey when the car stops replying because the value is out of date. |
| Pill (bottom) | **BOOST ON** (orange) or **BOOST OFF** |

The display can be updated in two ways. Whichever update arrives last wins.

1. **Telemetry from the car (recommended).** The car sends UDP packets back to the remote, for
   example `SPD:42;IND:L;BST:1`. Each token is `KEY:VALUE` or `KEY=VALUE`. Tokens can be separated
   by `;`, `,` or spaces, can come in any order, and you can send any subset of them.
   * `SPD`: speed as a number, such as `42` or `12.5`
   * `IND`: indicator, one of `L`, `R`, `H` (hazard) or `OFF`. `LEFT`, `RIGHT`, `HAZARD`, `BOTH` and `0` also work.
   * `BST`: boost, `1`/`0`, `ON`/`OFF` or `TRUE`/`FALSE`

   You can rename the keys in `display.telemetry_keys`. Packets that carry telemetry aren't written
   to the log unless `log_telemetry` is true.
2. **Button display actions** (works without telemetry): add `display_press` and/or
   `display_release` to a button. See the table below.

The link status comes from *any* packet the car sends. The default config sends a `PING`
heartbeat every 500 ms. Have the car reply to it, ideally with its telemetry.

## config.json

The app reloads the file automatically when you save it. You can also click **Reload config**.
If the file has an error, the app keeps the previous config and shows the error in the log.

### `buttons`
Valid names are `dpad_up/down/left/right`, `triangle`, `circle`, `cross`, `square`, `l1`, `l2`,
`l3`, `r1`, `r2`, `r3`, `create`, `options`, `ps`, `touchpad`, `mute`, and
`left_stick_up/down/left/right` and `right_stick_up/down/left/right`.

| Field | Meaning |
|---|---|
| `key` | Keyboard key: `"W"`, `"Space"`, `"Up"`, `"Left"`, `"F1"`, `"Shift"`, `"Escape"`, `"1"`, `";"` … Leave empty for no key. |
| `press` | Command sent when the button is pressed |
| `release` | Command sent when the button is released (empty = nothing). Example: `"S"` to stop when you let go of throttle. |
| `repeat_ms` | If > 0, `press` is re-sent at this interval while the button is held |
| `description` | Text shown in the tooltip |
| `display_press` | Optional display change on press: `{"boost": true \| false \| "toggle"}`, `{"indicator": "left" \| "right" \| "hazard" \| "off"}` (choosing the indicator that's already on switches it off), `{"speed": 30}` (set) or `{"speed": "+5"}` (change). With `repeat_ms`, relative speed changes repeat too. |
| `display_release` | Same format, applied on release. For example, `{"boost": false}` |

### Command format
Each command is sent as **one UDP packet**.
* Plain text, such as `"F"`, gets `udp.line_ending` appended (nothing by default) and is encoded
  with `udp.encoding`. JSON escapes work: `"F\n"`, `"\u0002GO\u0003"`.
* To send raw bytes, use `"hex:AA 01 FF"`. Bytes can be separated by spaces or commas, and may use a
  `0x` prefix. Hex commands get no line ending.

### `analog_sticks` (optional proportional control)
`"command": "THR:{y}"` sends the stick position every `interval_ms`, but only when it changes.
Placeholders: `{x}` and `{y}` run from `-range` to `+range` (up is positive when `invert_y` is
true), and `{ux}` and `{uy}` run from `0` to `2*range`. Python format specs work, so
`"hex:A1 {ux:02X} {uy:02X}"` sends binary packets. Leave `command` empty to disable it.
Sticks always send their four direction commands, using the `*_stick_*` buttons.

### `udp`
| Field | Meaning |
|---|---|
| `host` | Car's IP address or hostname. A broadcast address such as `192.168.4.255` also works. |
| `port` | UDP port the car listens on |
| `local_port` | Local port to send from and receive replies on. `0` lets the OS pick one; set a fixed port if the car replies to a fixed port. |
| `line_ending` | Appended to text commands, for example `""` or `"\n"` |
| `encoding` | Text encoding (default `ascii`) |
| `auto_connect` | Open the link on startup |
| `show_rx` | Show packets received from the car in the log |

UDP doesn't guarantee delivery. Use `repeat_ms` or a heartbeat with a timeout on the car so a
lost "stop" packet can't leave the car driving.

### `behavior`
`stop_command`, `send_stop_on_focus_loss`, `heartbeat_command` with `heartbeat_interval_ms` (sent
silently while connected; useful for a fail-safe timeout on the car), and `show_key_labels`.

Keys starting with `_` (for example `"_comment"`) are ignored, so you can use them as comments.

### `display`
| Field | Meaning |
|---|---|
| `speed_unit` | Text shown under the speed, for example `km/h`, `m/s` or `RPM` |
| `speed_max` | Speed at which the bar is full. Speeds set by buttons are capped at this value. |
| `speed_decimals` | Number of decimal places shown (0–3) |
| `link_timeout_ms` | Status shows **NO REPLY** if the car has been silent this long. Set to `0` if the car never replies. |
| `blink_ms` | Indicator blink period |
| `log_telemetry` | Also write telemetry packets to the log |
| `telemetry_keys` | Names of the telemetry keys: `{"speed": "SPD", "indicator": "IND", "boost": "BST"}`. Set a key to `""` to ignore it. |

## Car side (ESP32 / ESP8266 Arduino example)
```cpp
#include <WiFi.h>          // ESP8266: <ESP8266WiFi.h>
#include <WiFiUdp.h>
WiFiUDP udp;
char buf[64];
int speedKmh = 0; char ind = 'O'; bool boost = false;   // update these from your sensors/logic
void setup() {
  WiFi.softAP("RC-CAR", "12345678");   // car becomes 192.168.4.1
  udp.begin(4210);
}
void loop() {
  int n = udp.parsePacket();
  if (n) {
    n = udp.read(buf, sizeof(buf) - 1); buf[n] = 0;
    String cmd(buf);
    if (cmd == "F") { /* forward */ } else if (cmd == "S") { /* stop */ }
    else if (cmd == "BOOST") boost = true; else if (cmd == "BOOST_OFF") boost = false;
    else if (cmd == "IND_L") ind = (ind == 'L') ? 'O' : 'L';
    else if (cmd == "IND_R") ind = (ind == 'R') ? 'O' : 'R';
    else if (cmd == "IND_H") ind = (ind == 'H') ? 'O' : 'H';
    // Reply to every packet (including the PING heartbeat) with telemetry for the display
    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    udp.printf("SPD:%d;IND:%s;BST:%d", speedKmh, ind == 'O' ? "OFF" : String(ind).c_str(), boost);
    udp.endPacket();
  }
}
```

## Files
* `rc_remote.py`: the main window, key handling, button logic, and UDP sending
* `dualsense_widget.py`: draws the controller and the touchpad display, and handles mouse input
* `config_loader.py`: loads and validates the config, and encodes commands
* `udp_link.py`: non-blocking UDP socket wrapper
