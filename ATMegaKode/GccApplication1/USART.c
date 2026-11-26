/*
 * CFile1.c
 *
 * Created: 26/11/2026 11.01.56
 *  Author: Micha
 */ 


#include "usart.h"

#define TX_BUFFER_SIZE 256

char tx_buffer[TX_BUFFER_SIZE];
volatile uint8_t tx_write = 0;
volatile uint8_t tx_read = 0;

ISR(USART_UDRE_vect)
{
	if (tx_write != tx_read)
	{
		UDR = tx_buffer[tx_read];
		tx_read = (tx_read + 1) % TX_BUFFER_SIZE;
	}
	else
	{
		UCSRB &= ~(1 << UDRIE); // disable interrupt when buffer empty
	}
}


void USART_Init( unsigned int baud )
{
	/* Set TxD (PD1) as output*/
	DDRD |= (1 << PD1);
	/* Set baud rate */
	UBRRH = (unsigned char)(baud>>8);
	UBRRL = (unsigned char)baud;
	/* Enable transmitter */
	UCSRB = (1<<RXEN) | (1<<TXEN);
	/* Set frame format: 8data, 1stop bit (|(1<<USBS) for 2stop bit)*/
	UCSRC = (1<<URSEL)|(3<<UCSZ0);
}

void USART_Print(const char *str)
{
	while (*str)
	{
		uint8_t next_head = (tx_write + 1) % TX_BUFFER_SIZE;
		if (next_head != tx_read)  // only write if space available
		{
			tx_buffer[tx_write] = *str++;
			tx_write = next_head;
		}
	}
	UCSRB |= (1 << UDRIE); // enable UDRE interrupt to start sending
}

int USART_Receive( void )
{
	if (UCSRA &(1<<RXC)){
		return UDR;
	}
	return -1;
}