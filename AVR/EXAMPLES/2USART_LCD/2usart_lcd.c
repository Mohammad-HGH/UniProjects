/*****************************************************
CodeVisionAVR C Compiler
© Copyright 2000-2010 HP InfoTech S.R.L.
www.hpinfotech.com

Example program for redirecting the output of the
standard C I/O functions to the following
peripherals:
    - USART 0
    - USART 1
    - LCD

Chip type           : ATmega128
AVR Clock frequency : 3.686400 MHz (STK500)
Baud rate:          : 9600
Comm. parameters    : 8N1

The AVR chip is placed on a STK501 adapter
connected to an Atmel STK500 development board.
The jumper settings for the STK500 board are:
    OSCSEL: 1-2
    XTAL1:  set
    AREF:   set
    RESET:  set
    VTARGET:set

6 pin cable connected between ISP6PIN and
the STK501 SPROG header.

The STK501 TOSC switch is placed in the PIN
position.

The following connections between the STK500
PORTE and STK500 RS232 SPARE headers will enable
USART0 output to the STK500 RS232 SPARE 9 pin
connector:
    PORTE/PE0 - RS232 SPARE/RXD 
    PORTE/PE1 - RS232 SPARE/TXD 
    
The following connections between the STK500
PORTD and STK501 RS232 SPARE #2 headers will enable
USART1 output to the STK501 RS232 SPARE #2 9 pin
connector:
    PORTD/PD2 - RS232 SPARE #2/RXD 
    PORTD/PD3 - RS232 SPARE #2/TXD 
 
Use a 2x16 alphanumeric LCD connected
to the STK500 PORTC header as follows:

  [LCD]   [STK500 PORTC HEADER]
   1 GND- 9  GND
   2 +5V- 10 VCC  
   3 VLC- LCD contrast control voltage 0..1V
   4 RS - 1  PC0
   5 RD - 2  PC1
   6 EN - 3  PC2
  11 D4 - 5  PC4
  12 D5 - 6  PC5
  13 D6 - 7  PC6
  14 D7 - 8  PC7

For more information about STK500 and STK501
please consult the appropriate AVR Studio
help topics.
*****************************************************/

// ATmega128 I/O register definitions
#include <mega128.h>

// Alphanumeric LCD Module functions
// The LCD connections are specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric menu
#include <alcd.h>

// Specify the number of characters/line for the LCD
#define LCD_CHARS_LINE 16

// ATmega128 clock [Hz] 
#define FCLK 3686400
// Baud rate
#define BAUD 9600
// Calculate the UBRR setting
#define UBRR ((long) FCLK/(16*BAUD)-1)

// Bit definitions from the USART control registers
#define RXB8 1
#define TXB8 0
#define UPE 2
#define OVR 3
#define FE 4
#define UDRE 5
#define RXC 7

#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<UPE)
#define DATA_OVERRUN (1<<OVR)
#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC)

// Specify that a new putchar function will be used instead of the one from stdio.h
#define _ALTERNATE_PUTCHAR_

// Standard Input/Output functions
#include <stdio.h>

// Specify the output types
#define USART0 0
#define USART1 1
#define LCD 2

// This variable will specify to which peripheral the output of putchar will be directed
unsigned char poutput;

// Define the new putchar function
void putchar(char c)
{
switch (poutput)
       {
       case USART0: // the output will be directed to USART0
            while ((UCSR0A & DATA_REGISTER_EMPTY)==0);
            UDR0=c;
       break;

       case USART1: // the output will be directed to USART1
            while ((UCSR1A & DATA_REGISTER_EMPTY)==0);
            UDR1=c;
       break;
       
       case LCD:    // the output will be directed to the LCD
            lcd_putchar(c);
       };
}

void main(void)
{
// USART0 initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART0 Receiver: Off
// USART0 Transmitter: On
// USART0 Mode: Asynchronous
// USART0 Baud rate: 9600
UCSR0A=0x00;
UCSR0B=0x08;
UCSR0C=0x06;
UBRR0H=UBRR >> 8;
UBRR0L=UBRR & 0xFF;

// USART1 initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART1 Receiver: Off
// USART1 Transmitter: On
// USART1 Mode: Asynchronous
// USART1 Baud rate: 9600
UCSR1A=0x00;
UCSR1B=0x08;
UCSR1C=0x06;
UBRR1H=UBRR >> 8;
UBRR1L=UBRR & 0xFF;

// printf output will be directed to USART0
poutput=USART0;
printf("This is a test of USART%u.\r",0);

// printf output will be directed to USART1
poutput=USART1;
printf("This is a test of USART%u.\r",1);

// LCD module initialization
lcd_init(LCD_CHARS_LINE);

// printf output will be directed to the LCD
poutput=LCD;
printf("The LCD has %u\ncharacters/line.",LCD_CHARS_LINE);

// Stop here
while (1);
}
