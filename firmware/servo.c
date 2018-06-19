#include "servo.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#define SERVO_0 410
#define SERVO_90 200
#define SERVO_CLOSE SERVO_0
#define SERVO_OPEN SERVO_90

static int CURRENT_A_ANGLE = SERVO_0;
static int CURRENT_B_ANGLE = SERVO_0;
static int SERVO_A_CLOSED = true;
static int SERVO_B_CLOSED = true;

void servo_fstep(int id);
void servo_bstep(int id);
void servo_set(int id, int a);

void servo_init()
{
    TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11); // NON Inverted PWM
    TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11) | (1 << CS10);
    ICR1 = 4999; // PWM = 50Hz
    DDRB |= (1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2);
}


void servo_reset()
{
    servo_set(1, SERVO_OPEN);
    servo_set(2, SERVO_OPEN);
    _delay_ms(300);
    servo_set(1, SERVO_CLOSE);
    servo_set(2, SERVO_CLOSE);
}

void servo_set(int id, int a)
{
    // Enable servos
    PORTB |= (1 << PORTB0);

    int a_ = a;
    if (a >= SERVO_0)
    {
        a_ = SERVO_0;
    }
    else if (a <= SERVO_90)
    {
        a_ = SERVO_90;
    }

    // Servo A
    if (id == 1)
    {
        CURRENT_A_ANGLE = a_;
        OCR1A = a_;
    }
    // Servo B
    else
    {
        CURRENT_B_ANGLE = a_;
        OCR1B = a_;
    }

    // Disable servos
    _delay_ms(600);
    PORTB &= ~(1 << PORTB0);
}

void servo_open(int id)
{
    // Servo A
    if (id == 1)
    {
        if (SERVO_A_CLOSED)
        {
            servo_set(1, SERVO_OPEN);
            /* while (CURRENT_A_ANGLE != SERVO_90) */
            /* { */
            /*     servo_fstep(1); */
            /* } */
            SERVO_A_CLOSED = false;
        }
    }
    // Servo B
    else
    {
        if (SERVO_B_CLOSED)
        {
            servo_set(2, SERVO_OPEN);
            /* while (CURRENT_B_ANGLE != SERVO_90) */
            /* { */
            /*     servo_fstep(2); */
            /* } */
            SERVO_B_CLOSED = false;
        }
    }
}

void servo_close(int id)
{
    // Servo A
    if (id == 1)
    {
        if (! SERVO_A_CLOSED)
        {
            servo_set(1, SERVO_CLOSE);
            /* while (CURRENT_A_ANGLE != SERVO_0) */
            /* { */
            /*     servo_bstep(1); */
            /* } */
            SERVO_A_CLOSED = true;
        }
    }
    // Servo B
    else
    {
        if (! SERVO_B_CLOSED)
        {
            servo_set(2, SERVO_CLOSE);
            /* while (CURRENT_B_ANGLE != SERVO_0) */
            /* { */
            /*     servo_bstep(2); */
            /* } */
            SERVO_B_CLOSED = true;
        }
    }
}

void servo_toggle(int id)
{
    // Servo A
    if (id == 1)
    {
        if (SERVO_A_CLOSED) { servo_open(1); }
        else { servo_close(1); }
    }
    // Servo B
    else
    {
        if (SERVO_B_CLOSED) { servo_open(2); }
        else { servo_close(2); }
    }
}

/* void servo_fstep(int id) */
/* { */
/*     // Servo A */
/*     if (id == 1) */
/*     { */
/*         servo_set(1, CURRENT_A_ANGLE + SERVO_STEP); */
/*         /1* _delay_ms(SERVO_STEP_DELAY); *1/ */
/*     } */
/*     // Servo B */
/*     else */
/*     { */
/*         servo_set(2, CURRENT_B_ANGLE + SERVO_STEP); */
/*         /1* _delay_ms(SERVO_STEP_DELAY); *1/ */
/*     } */
/* } */

/* void servo_bstep(int id) */
/* { */
/*     // Servo A */
/*     if (id == 1) */
/*     { */
/*         servo_set(1, CURRENT_A_ANGLE - SERVO_STEP); */
/*         /1* _delay_ms(SERVO_STEP_DELAY); *1/ */
/*     } */
/*     // Servo B */
/*     else */
/*     { */
/*         servo_set(2, CURRENT_B_ANGLE - SERVO_STEP); */
/*         /1* _delay_ms(SERVO_STEP_DELAY); *1/ */
/*     } */
/* } */
