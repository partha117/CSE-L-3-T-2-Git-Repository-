/*
 * uartTest.c
 *
 * Created: 27-Apr-17 9:09:54 PM
 * Author : ASUS
 */ 

#include <avr/io.h>
#include "uart.c"
#include "util/delay.h"
#ifndef F_CPU
#define F_CPU 1000000UL
#endif
int main(void)
{
    /* Replace with your application code */
	/*Variable declaration*/

	/*USART initialization*/
	uart_init();
	
	//char input;
    while (1) 
    {
		puts("Hello world!");
		
		puts("It is");
		//input = getchar();
		//printf("You wrote %c\n", input);
		
    }
}

