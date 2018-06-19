#pragma once

#include <stdint.h>

#define COMMAND_PROMPT '>'
#define COMMAND_END ';'

typedef struct
{
    unsigned command : 4;
    unsigned value   : 1;
    unsigned         : 3;
} command_t;

/* Available commands */
#define NOP      0x0

// Actuators
#define STEP_LEFT      0x1
#define STEP_RIGHT     0x2
#define ROTATE_LEFT    0x3
#define ROTATE_RIGHT   0x4
#define LIGHTS_ON      0x5
#define LIGHTS_OFF     0x6
#define LIGHTS_TOGGLE  0x7
#define FRONT_ARM_UP   0x8
#define FRONT_ARM_DOWN 0x9
#define BACK_ARM_UP    0xA
#define BACK_ARM_DOWN  0xB

// Sensors
#define FRONT_ENTRY_SENSE 0xC
#define BACK_ENTRY_SENSE  0xD
