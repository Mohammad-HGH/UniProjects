/* 
   Digital voltmeter using an
   Analog Devices AD7896 ADC
   connected to an ATmega8515
   using the SPI bus
   
   CodeVisionAVR C Compiler
   (C) 2000-2010 HP InfoTech S.R.L.
   www.hpinfotech.ro

   Chip: ATmega8515
   Memory Model: SMALL
   Data Stack Size: 128 bytes
   Clock frequency: 3.6864 MHz
   
   AD7896 connections to the STK500
   
   [AD7896]   [PORTB header]
    1 Vin
    2 Vref=5V 10 VTG
    3 AGND  - 9  GND
    4 SCLK  - 8  SCK
    5 SDATA - 7  MISO
    6 DGND  - 9  GND
    7 CONVST- 2  PB1
    8 BUSY  - 1  PB0

   Use an 2x16 alphanumeric LCD connected
   to PORTC as follows:

   [LCD]   [PORTC header]
    1 GND- 9  GND
    2 +5V- 10 VTG
    3 VLC- LCD contrast control voltage 0..1V
    4 RS - 1  PC0
    5 RD - 2  PC1
    6 EN - 3  PC2
   11 D4 - 5  PC4
   12 D5 - 6  PC5
   13 D6 - 7  PC6
   14 D7 - 8  PC7

   NOTE: AFTER PROGRAMMING THE CHIP, DISCONNECT
   THE 6-WIRE PROGRAMMING CABLE FROM THE SPROG3
   HEADER
*/
  
// Alphanumeric LCD Module functions
// The LCD connections are specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric menu
#include <alcd.h>
// SPI driver routine
#include <spi.h> 
#include <mega8515.h>
#include <stdio.h>
#include <delay.h>

// AD7896 reference voltage [mV]
#define VREF 5000L

// AD7896 control signals PORTB bit allocation
#define ADC_BUSY PINB.0
#define NCONVST PORTB.1

// LCD display buffer
char lcd_buffer[33];

unsigned read_adc(void)
{
unsigned result;
// start conversion in mode 1
// (high sampling performance)
NCONVST=0;
NCONVST=1;
// wait for the conversion to complete
while (ADC_BUSY);
// read the MSB using SPI
result=(unsigned) spi(0)<<8;
// read the LSB using SPI and combine with MSB
result|=spi(0);
// calculate the voltage in [mV]
result=(unsigned) (((unsigned long) result*VREF)/4096L);
// return the measured voltage
return result;
}

void main(void)
{
// initialize PORTB
// PB.0 input from AD7896 BUSY
// PB.1 output to AD7896 /CONVST
// PB.2 & PB.3 inputs
// PB.4 output (SPI /SS pin)
// PB.5 input
// PB.6 input (SPI MISO)
// PB.7 output to AD7896 SCLK
DDRB=0x92;
// initialize the SPI in master mode
// no interrupts, MSB first, clock phase negative
// SCK low when idle, clock phase=0
// SCK=fxtal/4
SPCR=0x54;
SPSR=0x00;
// the AD7896 will work in mode 1
// (high sampling performance)
// /CONVST=1, SCLK=0
PORTB=2;
// initialize the LCD
lcd_init(16);

lcd_putsf("AD7896 SPI bus\nVoltmeter");
delay_ms(2000);
lcd_clear();

// read and display the ADC input voltage
while (1)
      {
      sprintf(lcd_buffer,"Uadc=%4umV",read_adc());
      lcd_clear();
      lcd_puts(lcd_buffer);
      delay_ms(100);
      };
}
