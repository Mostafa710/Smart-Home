#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "std_macros.h"

void USART_init(unsigned long Baud)
{
	/* 1- Choose the Baud Rate that will be used by transmitter and receiver */
	unsigned short UBRR;
	UBRR = (F_CPU/(16*Baud)) - 1;
	UBRRH = (unsigned char)(UBRR>>8);
	UBRRL = (unsigned char)UBRR;
	/* 2- Enable USART transmitter and receiver */
	SET_BIT(UCSRB,TXEN);
	SET_BIT(UCSRB,RXEN);
	/* 3- Choose number of data bits to be sent, parity and stop bits from UCSRC.
	   we will work with 8 bits data, 1 stop bit and no parity bits. */
	UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);
}
void USART_send_data(char data)
{
	/* Wait for UDR transmit buffer to be empty */
	while(READ_BIT(UCSRA,UDRE)==0);
	/* Write data in UDR transmit buffer */
	UDR = data;
}
char USART_receive_data(void)
{
	/* Wait for UDR receive buffer to be filled with data */
	while(READ_BIT(UCSRA,RXC)==0);
	/* Read data from UDR receive buffer */
	return UDR;
}
void USART_send_string(char *ptr)
{
	while(*ptr != 0)
	{
		USART_send_data(*ptr);
		ptr++;
		_delay_ms(100);
	}
}