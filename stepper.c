#include "stepper.h"
#include <avr/io.h>
#include <util/delay.h>

#define X_WAIT 40

int STEP_STATE = 0x01;

void stepper_reset()
{
    rotate_right(30);
    _delay_ms(500);
    rotate_left(30);
    _delay_ms(500);
}

void stepper_init()
{
    DDRC = 0b00001111;
    PORTC = 0x00;
    stepper_reset();
}

void step_right()
{
    STEP_STATE = (STEP_STATE == 0x01)? 0x08 : (STEP_STATE >>= 1);
    PORTC = STEP_STATE;
    _delay_ms(X_WAIT);
}

void step_left()
{
    STEP_STATE = (STEP_STATE == 0x08)? 0x01 : (STEP_STATE <<= 1);
    PORTC = STEP_STATE;
    _delay_ms(X_WAIT);
}

void rotate_right(int angle)
{
    int steps = angle/2;
    for (int i=0; i < steps; i++) { step_right(); }
}

void rotate_left(int angle)
{
    int steps = angle/2;
    for (int i=0; i < steps; i++) { step_left(); }
}
