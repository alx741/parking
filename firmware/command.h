#pragma once

#include <stdint.h>

#define COMMAND_PROMPT '>'
#define COMMAND_END ';'

typedef struct
{
    unsigned command : 8;
    /* unsigned value   : 1; */
    /* unsigned         : 3; */
} command_t;

/* Available commands */
#define NOP      0x00

// Actuators
#define STEP_LEFT      0x01
#define STEP_RIGHT     0x02
#define ROTATE_LEFT    0x03
#define ROTATE_RIGHT   0x04
#define LIGHTS_ON      0x05
#define LIGHTS_OFF     0x06
#define LIGHTS_TOGGLE  0x07
#define FRONT_ARM_UP   0x08
#define FRONT_ARM_DOWN 0x09
#define BACK_ARM_UP    0x0A
#define BACK_ARM_DOWN  0x0B

// Sensors
#define FRONT_ENTRY_SENSE 0x0C
#define BACK_ENTRY_SENSE  0x0D

// Configuration
#define ENABLE_AUTO_ENTRY_REACT  0x0E
#define DISABLE_AUTO_ENTRY_REACT 0x0F

// Reservation ligths
#define RESERVE_ON_1 0x10
#define RESERVE_ON_2 0x11
#define RESERVE_ON_3 0x12
#define RESERVE_OFF_1 0x13
#define RESERVE_OFF_2 0x14
#define RESERVE_OFF_3 0x15
