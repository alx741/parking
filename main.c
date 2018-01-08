#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include "uart.h"
#include "servo.h"
#include "stepper.h"

#define F_CPU 16000000

#define COMMAND_PROMPT '>'
#define COMMAND_END ';'

void init(void)
{
    stepper_init();
    /* servo_init(); */
    uart_init();
    /* DDRC |= 0b00000001; */
    /* PORTC = 0b00000001; */
}

void execute_command(char *s)
{
    switch (s[0])
    {
        /* case 's': */
        /*     (s[2] == 'f') ? servo_fstep() : servo_bstep(); */
        /*     break; */

        case 'x':
            step_right();
            break;

        case 'y':
            step_left();
            break;

        /* case 'r': */
        /*     (s[2] == 'r') ? step_right(10) : step_left(10); */
        /*     break; */

        case 'r':
            rotate_right(30);
            break;

        case 'l':
            rotate_left(30);
            break;

        /* case 'r': */
        /*     servo_reset(); */
        /*     break; */
    }
}


int main(void)
{
    const int com_max = 10;
    char com_char;
    char command[com_max];

    init();
    putchar(COMMAND_PROMPT);

    while (1) {
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
