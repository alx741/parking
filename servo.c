#include "servo.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#define SERVO_0 200
#define SERVO_90 410
#define SERVO_180 520
#define SERVO_STEP 10
#define SERVO_STEP_DELAY 40

static int CURRENT_ANGLE = SERVO_0;
static int SERVO_CLOSED = true;

void servo_init()
{
    TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11); // NON Inverted PWM
    TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11) | (1 << CS10);
    ICR1 = 4999; // PWM = 50Hz
    DDRB |= (1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2);
    servo_reset();
}


void servo_reset()
{
    servo_set(SERVO_0);
    _delay_ms(500);
    servo_set(SERVO_90);
    _delay_ms(500);
    servo_set(SERVO_0);
}

void servo_set(angle a)
{
    // Enable servo
    PORTB |= (1 << PORTB0);

    int a_ = a;
    if (a <= SERVO_0)
    {
        a_ = SERVO_0;
    }
    else if (a >= SERVO_90)
    {
        a_ = SERVO_90;
    }
    CURRENT_ANGLE = a_;
    OCR1A = a_;

    // Disable servo
    _delay_ms(500);
    PORTB &= ~(1 << PORTB0);
}

void servo_open()
{
    if (SERVO_CLOSED)
    {
        while (CURRENT_ANGLE != SERVO_90)
        {
            servo_fstep();
        }
        SERVO_CLOSED = false;
    }
}

void servo_close()
{
    if (! SERVO_CLOSED)
    {
        while (CURRENT_ANGLE != SERVO_0)
        {
            servo_bstep();
        }
        SERVO_CLOSED = true;
    }
}

void servo_toggle()
{
    if (SERVO_CLOSED) { servo_open(); }
    else { servo_close(); }
}

void servo_fstep()
{
    servo_set(CURRENT_ANGLE + SERVO_STEP);
    _delay_ms(SERVO_STEP_DELAY);
}

void servo_bstep()
{
    servo_set(CURRENT_ANGLE - SERVO_STEP);
    _delay_ms(SERVO_STEP_DELAY);
}
