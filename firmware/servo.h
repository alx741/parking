#pragma once

void servo_init();
void servo_reset();

void servo_set(int id, int a);

void servo_open(int id);
void servo_close(int id);
void servo_toggle(int id);
