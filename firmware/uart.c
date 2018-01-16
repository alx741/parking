#include "uart.h"
#include <avr/io.h>

#define BAUD_PRESCALE ((((F_CPU/16) + (BAUD/2)) / (BAUD)) - 1)

void uart_init(void)
{
    UBRR0H = (BAUD_PRESCALE >> 8);
    UBRR0L = BAUD_PRESCALE;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (0 << UMSEL00) | (0 << UMSEL01) | (1 << UCSZ00)
            | (1 << UCSZ01) | (0 << UCSZ02);
}

void putchar(char c)
{
    while ((UCSR0A & (1 << UDRE0)) == 0) {}
    UDR0 = c;
}

char getchar(void)
{
    while ((UCSR0A & (1 << RXC0)) == 0) {}
    return UDR0;
}

void puts(const char *s)
{
    while (*s != '\0')
    {
        putchar(*s);
        s++;
    }
}
