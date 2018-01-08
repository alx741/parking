#include "stepper.h"
#include <avr/io.h>
#include <util/delay.h>

#define X_MICRO_STEPS 76
#define Y_MICRO_STEPS 60
#define X_WAIT 12
#define Y_WAIT 12

void x_micro_fstep();
void x_micro_bstep();
void y_micro_fstep();
void y_micro_bstep();
void reset();

int X_STEP_STATE = 1;
int Y_STEP_STATE = 1;

int X_MICRO_STATE = 0x01;
int Y_MICRO_STATE = 0x10;


void stepper_init()
{
    DDRC = 0b00001111;
    PORTC = 0x00;
    /* board_reset(); */
}

/* void board_reset() */
/* { */
/*     _delay_ms(1000); */
/*     x_micro_fstep(); */
/*     x_micro_bstep(); */
/*     y_micro_fstep(); */
/*     y_micro_bstep(); */

/*     // Reset X axis */
/*     while (PINC & (1 << PINC5)) */
/*     { */
/*         x_micro_bstep(); */
/*     } */

/*     // Reset Y axis */
/*     while (PINC & (1 << PINC4)) */
/*     { */
/*         y_micro_bstep(); */
/*     } */
/* } */

/* void fstep() */
/* { */
/*     if (X_STEP_STATE == 8){ return; } */

/*     for (int i=0; i<X_MICRO_STEPS; i++) */
/*     { */
/*         x_micro_fstep(); */
/*     } */

/*     X_STEP_STATE++; */
/* } */

/* void bstep() */
/* { */
/*     for (int i=0; i<X_MICRO_STEPS; i++) */
/*     { */
/*         if (PINC & (1 << PINC5)) */
/*         { */
/*             x_micro_bstep(); */
/*         }else { break; } */
/*     } */
/*     X_STEP_STATE--; */
/* } */

void fstep()
{
    PORTC = X_MICRO_STATE;
    _delay_ms(X_WAIT);
    PORTC = 0x00;
    X_MICRO_STATE = (X_MICRO_STATE == 0x08)? 0x01 : (X_MICRO_STATE <<= 1);
}

void bstep()
{
    PORTC = X_MICRO_STATE;
    _delay_ms(X_WAIT);
    PORTC = 0x00;
    X_MICRO_STATE = (X_MICRO_STATE == 0x01)? 0x08 : (X_MICRO_STATE >>= 1);
}
