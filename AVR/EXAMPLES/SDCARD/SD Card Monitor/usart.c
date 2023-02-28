/*------------------------------------------------*/
/* USART functions                                 */

#include <io.h>
#include <stdint.h>
#include <stdio.h>
#include "interrupt.h"
#include "usart.h"

#define BAUD 19200
#define UBRR_VAL (_MCU_CLOCK_FREQUENCY_/(BAUD*16L)-1)

/* Signal that the putchar and getchar functions are redefined */
#define _ALTERNATE_PUTCHAR_
#define _ALTERNATE_GETCHAR_

#define _BV(BIT) (1<<BIT)

typedef struct _fifo {
	uint8_t	idx_w;
	uint8_t	idx_r;
	uint8_t	count;
	uint8_t buff[64];
} FIFO;


static volatile
FIFO txfifo, rxfifo;

/* Initialize USART */
void usart_init()
{
	rxfifo.idx_r = 0;
	rxfifo.idx_w = 0;
	rxfifo.count = 0;
	txfifo.idx_r = 0;
	txfifo.idx_w = 0;
	txfifo.count = 0;

     UBRR0H = UBRR_VAL >> 8;
	UBRR0L = UBRR_VAL & 0xFF;
	UCSR0B = _BV(RXEN0)|_BV(RXCIE0)|_BV(TXEN0);
}


/* Get a received character */
uint8_t usart_test ()
{
	return rxfifo.count;
}


char getchar (void)
{
	uint8_t d, i;

	i = rxfifo.idx_r;
	while(rxfifo.count == 0);
	d = rxfifo.buff[i++];
	cli();
	rxfifo.count--;
	sei();
	if(i >= sizeof(rxfifo.buff))
		i = 0;
	rxfifo.idx_r = i;

	return d;
}


/* Put a character to transmit */
void putchar (char c)
{
	uint8_t i;

	i = txfifo.idx_w;
	while(txfifo.count >= sizeof(txfifo.buff));
	txfifo.buff[i++] = c;
	cli();
	txfifo.count++;
	UCSR0B = _BV(RXEN0)|_BV(RXCIE0)|_BV(TXEN0)|_BV(UDRIE0);
	sei();
	if(i >= sizeof(txfifo.buff))
		i = 0;
	txfifo.idx_w = i;
}


/* USART RXC interrupt */
interrupt [USART0_RXC] void usart_rx_isr(void)
{
	uint8_t d, n, i;

	d = UDR0;
	n = rxfifo.count;
	if(n < sizeof(rxfifo.buff)) {
		rxfifo.count = ++n;
		i = rxfifo.idx_w;
		rxfifo.buff[i++] = d;
		if(i >= sizeof(rxfifo.buff))
			i = 0;
		rxfifo.idx_w = i;
	}
}


/* USART DRE interrupt */
interrupt [USART0_DRE] void usart_tx_isr(void)
{
	uint8_t n, i;

	n = txfifo.count;
	if(n) {
		txfifo.count = --n;
		i = txfifo.idx_r;
		UDR0 = txfifo.buff[i++];
		if(i >= sizeof(txfifo.buff))
			i = 0;
		txfifo.idx_r = i;
	}
	if(n == 0)
		UCSR0B = _BV(RXEN0)|_BV(RXCIE0)|_BV(TXEN0);
}

