#pragma once

typedef enum {ANGLE_START, ANGLE_MIDDLE, ANGLE_END} angle;

void servo_init();
void servo_reset();
void servo_set(angle a);
void servo_fstep();
void servo_bstep();
