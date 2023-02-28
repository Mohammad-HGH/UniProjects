/*
Sample program to test the ATxmega128A1 USART C0 and EEPROM
Use a STK600 development board with STK600-TQFP100 and
STK600-RC100X-13 addapters

The STK600 programmer must be set in JTAG programming mode
in the Tools|Programmer menu.

Make sure that the VTARGET and VREF voltages are set to 3.6V
using AVR Studio.
The VTARGET LED on the STK600 board must be lighted.

Make the following connections on the STK600
PC3 - RS232 SPARE TXD.

Connect the STK600 RS232 output to the COM port of the PC
Make sure to configure the CodeVisionAVR Terminal
to 9600bps 8N1.
*/

#include <stdio.h>
#include <io.h>
#include <stdint.h>
#include <stdbool.h>

#define USART_Format_Set(_usart, _charSize, _parityMode, _twoStopBits)\
    (_usart)->CTRLC = (uint8_t) _charSize | _parityMode |\
                      (_twoStopBits ? USART_SBMODE_bm : 0)

#define USART_Baudrate_Set(_usart, _bselValue, _bScaleFactor)\
	(_usart)->BAUDCTRLA =(uint8_t)_bselValue;\
	(_usart)->BAUDCTRLB =(_bScaleFactor << USART_BSCALE0_bp)|(_bselValue >> 8)

#define USART_Rx_Enable(_usart) ((_usart)->CTRLB |= USART_RXEN_bm)
#define USART_Tx_Enable(_usart)	((_usart)->CTRLB |= USART_TXEN_bm)

typedef struct
        {
        int a;
        unsigned char b:3;
        unsigned int c:1;
        unsigned int d:10;
        unsigned long e:3;
        unsigned long f:20;
        } stest;

// init the EEPROM structure
eeprom stest eeprom_struct={1234,2,1,999,5,0x11111};

void main(void)
{
eeprom stest *p=&eeprom_struct;

// increment some of the bit fields of the structure located in EEPROM
eeprom_struct.b++;
eeprom_struct.d++;
eeprom_struct.f++;

// Init the USART
// This PORT setting is only valid for USARTC0
// if other USART is used a different PORT and/or pins are used,
// please see the datasheet
// PIN3 (TXD0) as output
PORTC.DIRSET = PIN3_bm;
// PC2 (RXD0) as input
PORTC.DIRCLR = PIN2_bm;

// USARTC0, 8 Data bits, No Parity, 1 Stop bit
USART_Format_Set(&_ATXMEGA_USART_, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, false);

// Set Baudrate to 9600 bps
// Use the default I/O clock fequency that is 2 MHz
// Do not use the baudrate scale factor
// Baudrate select = (1/(16*(((I/O clock frequency)/Baudrate)-1) = 12
USART_Baudrate_Set(&_ATXMEGA_USART_, 12, 0);

// Enable TX
USART_Tx_Enable(&_ATXMEGA_USART_);

printf("The ATxmega128A1 USART C0 is working :-)\n\r");
printf("Let's see the EEPROM structure contents:\n\ra=%d b=%d c=%d d=%d e=%d f=%lX\n\r",p->a,p->b,p->c,p->d,p->e,p->f);
printf("Switch the STK600 power OFF and ON again\n\rThe EEPROM structure 'b', 'd' and 'f' bit fields will be incremented\n\r\n\r");

while (1);
}
