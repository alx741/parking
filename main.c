#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include "uart.h"
#include "servo.h"
#include "stepper.h"

#define F_CPU 16000000

#define COMMAND_PROMPT '>'
#define COMMAND_END ';'

void react_arms(void)
{
    // Entry Open
    if (! (PINB & (1 << PB4)))
    {
        // Car on sensor for at least 150ms
        _delay_ms(150);
        if (! (PINB & (1 << PB4)))
        {
            servo_open(1);
        }
    }

    // Entry Close
    if (PINB & (1 << PB4))
    {
        // Car left sensor 300ms ago
        _delay_ms(300);
        if (PINB & (1 << PB4))
        {
            servo_close(1);
        }
    }
}


void init(void)
{
    stepper_init();
    servo_init();
    uart_init();

    // Sensors inputs
    /* DDRB |= (1 << PORTB5); */
    DDRB &= ~(1 << PORTB4);
    DDRB &= ~(1 << PORTB5);
    /* PORTB &= ~(1 << PORTB5); */
}

void execute_command(char *s)
{
    switch (s[0])
    {
        /* case 's': */
        /*     (s[1] == '1') ? servo_toggle(1) : servo_toggle(2); */
        /*     break; */

        case 'x':
            step_right();
            break;

        case 'y':
            step_left();
            break;

        case 'r':
            rotate_right(30);
            break;

        case 'l':
            rotate_left(30);
            break;
    }
}


int main(void)
{
    const int com_max = 10;
    char com_char;
    char command[com_max];

    init();
    putchar(COMMAND_PROMPT);

    while (1)
    {
        react_arms();

        // code is blocking from here on
        /* com_char = getchar(); */
        /* int i = 0; */
        /* while (com_char != COMMAND_END || i == 10) */
        /* { */
        /*     command[i] = com_char; */
        /*     com_char = getchar(); */
        /*     i++; */
        /* } */

        /* execute_command(command); */
        /* memset(command, 0, com_max); */
        /* _delay_ms(50); */
        /* putchar(COMMAND_PROMPT); */
    }
}
