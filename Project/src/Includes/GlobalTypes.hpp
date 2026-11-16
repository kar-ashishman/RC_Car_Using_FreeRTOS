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
    STOP = 0xF100,
    FORWARD = 0xF101,
    REVERSE = 0xF201,
    STEER_LEFT = 0xF301,
    STEER_RIGHT = 0xF401,
    HORNON = 0xF501,
    HORNOFF = 0xF500,
    HORNDANCE = 0xF502,
    IND_LEFT = 0xF601,
    IND_RIGHT = 0xF602,
    HAZARD = 0xF603,
    IND_OFF = 0xF600,
    HEADLIGHTON = 0xF701,
    HEADLIGHTOFF = 0xF700,
    SYNC = 0xEE00
};
