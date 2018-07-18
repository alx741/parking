#include <string.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "servo.h"
#include "stepper.h"
#include "command.h"

#define ENTER_DELAY 100
#define EXIT_DELAY 500

static bool LIGHTS_STATE_ON = false;
static bool AUTO_ENTRY_REACT_ENABLED = true;

void execute_command(command_t c);
void react_arms(void);


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

    // Light control
    DDRB |= (1 << PORTB3);
    PORTB &= ~(1 << PORTB3);

    // Reservation ligths
    DDRD |= (1 << PORTD2);
    PORTD &= ~(1 << PORTD2);
    DDRD |= (1 << PORTD3);
    PORTD &= ~(1 << PORTD3);
    DDRC |= (1 << PORTC5);
    PORTC &= ~(1 << PORTC5);
}

void toggle_lights(void)
{
    if (LIGHTS_STATE_ON)
    {
        PORTB &= ~(1 << PORTB3);
        LIGHTS_STATE_ON = false;
    }
    else
    {
        PORTB |= (1 << PORTB3);
        LIGHTS_STATE_ON = true;
    }
}

void lights_on(void)
{
    PORTB |= (1 << PORTB3);
    LIGHTS_STATE_ON = true;
}

void lights_off(void)
{
    PORTB &= ~(1 << PORTB3);
    LIGHTS_STATE_ON = false;
}

command_t read_command(char c)
{
    command_t command;
    command.command = 0xFF & c;
    return command;
}

int main(void)
{
    init();

    while (1)
    {
        putchar(COMMAND_PROMPT);
        execute_command(read_command(getchar()));
    }
}

void execute_command(command_t c)
{
    switch (c.command)
    {
        case STEP_RIGHT:
            step_right();
            break;

        case STEP_LEFT:
            step_left();
            break;

        case ROTATE_RIGHT:
            rotate_right(19);
            break;

        case ROTATE_LEFT:
            rotate_left(19);
            break;

        case LIGHTS_ON:
            lights_on();
            break;

        case LIGHTS_OFF:
            lights_off();
            break;

        case LIGHTS_TOGGLE:
            toggle_lights();
            break;

        case FRONT_ARM_UP:
            servo_open(1);
            break;

        case FRONT_ARM_DOWN:
            servo_close(1);
            break;

        case BACK_ARM_UP:
            servo_open(2);
            break;

        case BACK_ARM_DOWN:
            servo_close(2);
            break;

        case FRONT_ENTRY_SENSE:
            if (PINB & (1 << PB4)) { putchar('f'); }
            else { putchar('t'); }
            break;

        case BACK_ENTRY_SENSE:
            if (PINB & (1 << PB5)) { putchar('f'); }
            else { putchar('t'); }
            break;

        case ENABLE_AUTO_ENTRY_REACT:
            AUTO_ENTRY_REACT_ENABLED = true;
            break;

        case DISABLE_AUTO_ENTRY_REACT:
            AUTO_ENTRY_REACT_ENABLED = false;
            break;

        case RESERVE_ON_1:
            PORTD |= (1 << PORTD2);
            break;

        case RESERVE_ON_2:
            PORTD |= (1 << PORTD3);
            break;

        case RESERVE_ON_3:
            PORTC |= (1 << PORTC5);
            break;

        case RESERVE_OFF_1:
            PORTD &= ~(1 << PORTD2);
            break;

        case RESERVE_OFF_2:
            PORTD &= ~(1 << PORTD3);
            break;

        case RESERVE_OFF_3:
            PORTC &= ~(1 << PORTC5);
            break;
    }
}

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

ISR(BADISR_vect){}

ISR(PCINT0_vect)
{
    if (AUTO_ENTRY_REACT_ENABLED)
    {
        react_arms();
    }
}
