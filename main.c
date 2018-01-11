#include <string.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "servo.h"
#include "stepper.h"

#define F_CPU 16000000

#define COMMAND_PROMPT '>'
#define COMMAND_END ';'

#define ENTER_DELAY 100
#define EXIT_DELAY 500


void react_arms(void)
{
    // Entry Open
    if (! (PINB & (1 << PB4)))
    {
        _delay_ms(ENTER_DELAY);
        if (! (PINB & (1 << PB4)))
        {
            servo_open(1);
        }
    }
    if (! (PINB & (1 << PB5)))
    {
        _delay_ms(ENTER_DELAY);
        if (! (PINB & (1 << PB5)))
        {
            servo_open(2);
        }
    }

    // Entry Close
    if (PINB & (1 << PB4))
    {
        _delay_ms(EXIT_DELAY);
        if (PINB & (1 << PB4))
        {
            servo_close(1);
        }
    }
    if (PINB & (1 << PB5))
    {
        _delay_ms(EXIT_DELAY);
        if (PINB & (1 << PB5))
        {
            servo_close(2);
        }
    }
}


void init(void)
{
    stepper_init();
    servo_init();
    uart_init();

    // Car sensors inputs
    DDRB &= ~(1 << PORTB4);
    DDRB &= ~(1 << PORTB5);
    DDRB &= ~(1 << DDB4);
    DDRB &= ~(1 << DDB5);
    PORTB |= (1 << PORTB4); // Pull-ups
    PORTB |= (1 << PORTB5);

    // Interrupts
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << PCINT4);
    PCMSK0 |= (1 << PCINT5);
    sei();
}

void execute_command(char *s)
{
    switch (s[0])
    {
        case 'r':
            rotate_right(10);
            break;

        case 'l':
            rotate_left(10);
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
        // code is blocking from here on
        com_char = getchar();
        int i = 0;
        while (com_char != COMMAND_END || i == 10)
        {
            command[i] = com_char;
            com_char = getchar();
            i++;
        }

        execute_command(command);
        memset(command, 0, com_max);
        _delay_ms(50);
        putchar(COMMAND_PROMPT);
    }
}

ISR(BADISR_vect){}

ISR(PCINT0_vect)
{
    react_arms();
}
