#pragma once

typedef enum {ANGLE_START, ANGLE_MIDDLE, ANGLE_END} angle;

void servo_init();
void servo_reset();

void servo_open();
void servo_close();
void servo_toggle();
