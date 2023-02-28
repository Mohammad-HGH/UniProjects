/* Thermometer with LCD display
   using the National Semiconductor LM75 I2C bus
   temperature sensor

   CodeVisionAVR V2.05.1+ C Compiler
   (C) 2000-2011 HP InfoTech S.R.L.
   www.hpinfotech.ro

   Chip: ATmega8515
   Memory Model: SMALL
   Data Stack Size: 128 bytes

   I2C bus port bits allocations for PORTA:
   SCL=bit 0, SDA=bit 1 are specified in the
   Project|Configure|C Compiler|Libraries|I2C menu
   
   [LM75]   [STK500 PORTA HEADER]
    1 SDA - 2  PA1
    2 SCL - 1  PA0
    3 OS  - not connected
    4 GND - 9  GND
    5 A2  - 9  GND
    6 A1  - 9  GND
    7 A0  - 9  GND
    8 VCC - 10 VCC
     
   NOTE: 3.3k..4.7k PULL-UP RESITORS TO +5V MUST
   BE USED FOR THE SDA & SCL I2C BUS SIGNALS

   Use an 2x16 alphanumeric LCD connected
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

   The LCD connections are specified in the
   Project|Configure|C Compiler|Libraries|Alphanumeric menu

*/

#include <lm75.h>  // LM75 driver routines
#include <stdio.h> // sprintf
#include <delay.h> // delay_ms
#include <alcd.h> // Alphanumeric LCD Module functions

// LCD display buffer
char lcd_buffer[33];

void main(void)
{
char sign;
int temp;
// initialize the LCD
lcd_init(16);
// initialize the I2C bus
i2c_init();
// initialize the LM75 chip with address 0
// hysterezis temperature 20ÝC
// O.S. temperature 25ÝC
// O.S. output is active high
lm75_init(0,20,25,1);
// temperature display loop
while (1)
      {
      // read LM75 temperature *10ÝC
      // from chip with address 0
      temp=lm75_temperature_10(0);
      sign='+';
      if (temp<0)
         {
         sign='-';
         temp=-temp;
         };
      sprintf(lcd_buffer,"t=%c%i.%u\xdf" "C",sign,temp/10,temp%10);
      // display the temperature
      lcd_clear();
      lcd_puts(lcd_buffer);
      delay_ms(200);
      }
}
