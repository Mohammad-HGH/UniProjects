/*************************************************
   Digital voltmeter using an
   Maxim MAX1241 ADC
   connected to an ATmega8515
   using the SPI bus
   The measured voltage is transmitted
   through the STK500 RS232 interface
   Communication parameters: 9600 8N1

   CodeVisionAVR C Compiler
   (C) 2000-2007 HP InfoTech S.R.L.
   www.hpinfotech.ro

   Chip type        : ATmega8515
   Clock frequency  : 3.686400 MHz
   Memory model     : Small
   Internal RAM size: 512
   External RAM size: 0
   Data Stack size  : 128
*************************************************

   PLEASE MAKE SURE THAT THE CKSEL0..3 FUSE
   BITS ARE PROGRAMMED TO USE THE EXTERNAL
   CLOCK SOURCE OF THE STK500 AND NOT
   THE INTERNAL 1MHz OSCILLATOR.
   The ATmega8515 chip comes from the factory
   with CKSEL0..3 fuse bits set to use the
   internal 1 MHz oscillator.

 	MAX1241 connections to the STK500:
	[MAX1241] [PORTB header]
	1 VDD   - 10 +5V
	2 Vin
	3 /SHDN - 1  PB0
	4 REF   - 10 +5V
	5 GND   - 9  GND
	6 DOUT  - 7  MISO
	7 /CS   - 2  PB1
	8 SCLK  - 8  SCK

   In order to use the RS232 SPARE connector
   on the STK500, the following connections must
   be made:
   [RS232 SPARE header] [PORTD header]
   RXD                - 1 PD0
   TXD                - 2 PD1

   NOTE: AFTER PROGRAMMING THE CHIP, DISCONNECT
   THE 6-WIRE PROGRAMMING CABLE FROM THE SPROG3
   HEADER
*/

#include <mega8515.h>

// Standard Input/Output functions
#include <stdio.h>

// SPI functions
#include <spi.h>

// delays
#include <delay.h>

// MAX1241 Voltage Reference [mV]
#define VREF 5000

// MAX1241 control signals definitions
#define NSHDN PORTB.0
#define NCS PORTB.1
#define DOUT PINB.6

union adcu
          {
          unsigned char byte[2];
          unsigned int word;
          };
          
// Make one AD conversion and return the value
unsigned int max1241_read(void)
{
union adcu adc_data;
// exit MAX1241 from shutdown
NSHDN=1;
// wait 5us for the MAX1241 to wake up
delay_us(5);
// now select the chip to start the conversion
NCS=0;
// wait the conversion to complete
// DOUT will be 0 during conversion
while (DOUT==0);
// DOUT=1 -> conversion completed
// read MSB
adc_data.byte[1]=spi(0);
// read LSB
adc_data.byte[0]=spi(0);
// deselect the chip
NCS=1;
// enter shutdown
NSHDN=0;
// now format the result and return it
return (adc_data.word>>3)&0xfff;
}

void main(void)
{
// store the conversion result here
unsigned n;

// Input/Output Ports initialization
// Port A
DDRA=0x00;
PORTA=0x00;

// Port B
// the /SS pin is set as an output
// with level 1, it's required by
// the SPI to work in master mode
DDRB=0xA3;
PORTB=0x12;

// Port C
DDRC=0x00;
PORTC=0x00;

// Port D
DDRD=0x00;
PORTD=0x00;

// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: Off
// USART Transmitter: On
// USART Mode: Asynchronous
// USART Baud rate: 9600
UCSRA=0x00;
UCSRB=0x08;
UCSRC=0x86;
UBRRH=0x00;
UBRRL=0x17;

// SPI initialization
// SPI Type: Master
// SPI Clock Rate: 921.6 kHz
// SPI Clock Phase: Cycle Half
// SPI Clock Polarity: Low
// SPI Data Order: MSB First
SPCR=0x50;
SPSR=0x00;

putsf("MAX1241 Demo using the CodeVisionAVR C Compiler");
putsf("***********************************************\n");

// Make AD conversions and send the results to RS232
while (1)
      {
      n=max1241_read();
      printf("MAX1241-> N=%4u U=%4umV\r\n",n,(unsigned) ((long) n*VREF/4096));
      // 0.3 sec. delay
      delay_ms(300);
      };
}
