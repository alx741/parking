#include "servo.h"
#include <avr/io.h>
#include <util/delay.h>

#define SERVO_0 200
#define SERVO_90 410
#define SERVO_180 560
#define SERVO_STEP 10

#define SERVO_ANGLE_START SERVO_0
/* #define SERVO_ANGLE_MIDDLE SERVO_90 */
#define SERVO_ANGLE_END SERVO_90

const int CURRENT_ANGLE = SERVO_ANGLE_START;

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
    servo_set(ANGLE_START);
    _delay_ms(500);
    servo_set(ANGLE_MIDDLE);
    _delay_ms(500);
    servo_set(ANGLE_START);
}

void servo_set(angle a)
{
    if (a == ANGLE_START)
    {
        OCR1A = SERVO_ANGLE_START;
    }
    else if (a == ANGLE_MIDDLE)
    {
        OCR1A = SERVO_ANGLE_MIDDLE;
    }
    else if (a == ANGLE_END)
    {
        OCR1A = SERVO_ANGLE_END;
    }
}

void servo_fstep()
{
    OCR1A += SERVO_STEP;
    if (OCR1A >= SERVO_ANGLE_END)
    {
        servo_set(ANGLE_END);
    }
}

void servo_bstep()
{
    OCR1A -= SERVO_STEP;
    if (OCR1A <= SERVO_ANGLE_START)
    {
        servo_set(ANGLE_START);
    }
}
