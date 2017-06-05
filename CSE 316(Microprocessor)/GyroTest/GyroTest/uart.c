#include <avr/io.h>
#include <stdio.h>
#include "uart.h"
#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#ifndef BAUD
#define BAUD 9600
#endif
#include <util/setbaud.h>

/* http://www.cs.mun.ca/~rod/Winter2007/4723/notes/serial/serial.html */
void uart_init(void) {
    UBRRH = UBRRH_VALUE;
    UBRRL = UBRRL_VALUE;
    
#if USE_2X
    UCSRA |= (1<<(U2X));
#else
    UCSRA &= ~(1<<(U2X));
#endif

    UCSRC =(1<<URSEL)|(1<<(UCSZ1)) | (1<<(UCSZ0)); /* 8-bit data */ 
    UCSRB = (1<<(RXEN)) | (1<<(TXEN));   /* Enable RX and TX */    
}

void uart_putchar(char c, FILE *stream) {
    if (c == '\n') {
        uart_putchar('\r', stream);
    }
    loop_until_bit_is_set(UCSRA, UDRE);
    UDR = c;
}

char uart_getchar(FILE *stream) {
    loop_until_bit_is_set(UCSRA, RXC);
    return UDR;
}
