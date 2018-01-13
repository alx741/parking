#include "stepper.h"
#include <avr/io.h>
#include <util/delay.h>

#define DELAY 40

int STEP_STATE = 0x01;

void stepper_init()
{
    DDRC = 0b00001111;
    PORTC = 0x00;
}

void step_left()
{
    STEP_STATE = (STEP_STATE == 0x01)? 0x08 : (STEP_STATE >>= 1);
    _delay_ms(DELAY);
    PORTC = STEP_STATE;
    /* PORTC = 0x00; */
}

void step_right()
{
    STEP_STATE = (STEP_STATE == 0x08)? 0x01 : (STEP_STATE <<= 1);
    _delay_ms(DELAY);
    PORTC = STEP_STATE;
    /* PORTC = 0x00; */
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
