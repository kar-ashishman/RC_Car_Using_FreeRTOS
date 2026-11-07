#pragma once

#include <stdint.h>

enum class Pinout {
    HORN_PIN = 2
};

enum class CoreId {
    CONTROL_CORE = 0,
    COMMUNICATION_CORE = 1
};

enum class Direction {
    BACKWARD = -1,
    STOP = 0,
    FORWARD = 1
};

enum class Indicator{
    NONE = 0,
    LEFT = 1,
    RIGHT = 2,
    HAZARD = 3
};

enum class Signals{
    NONE = 0x00,
    STOP = 0xF0,
    FORWARD = 0xF1,
    REVERSE = 0xF2,
    STEER_LEFT = 0xF3,
    STEER_RIGHT = 0xF4,
    HORNON = 0xF5,
    HORNOFF = 0xF6,
    HORNDANCE = 0xF7,
    IND_LEFT = 0xF8,
    IND_RIGHT = 0xF9,
    HAZARD = 0xFA,
    IND_OFF = 0xFB,
    HEADLIGHTON = 0xFC,
    HEADLIGHTOFF = 0xFD,
    SYNC = 0xEE
};
