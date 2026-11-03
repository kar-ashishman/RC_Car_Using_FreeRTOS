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
    NONE,
    FORWARD,
    REVERSE,
    HORNON = 'HN',
    HORNOFF = 'HF',
    HORNDANCE = 'HD',
    IND_LEFT = 'LI',
    IND_RIGHT = 'RI',
    HAZARD = 'HZ',
    IND_OFF = 'ST'
};
