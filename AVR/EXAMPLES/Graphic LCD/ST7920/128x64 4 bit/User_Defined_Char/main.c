/**********************************************************
Sitronix ST7920 Graphic LCD controller demo
(C) Copyright 2011-2012 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

The ST7920 controller is interfaced to the
AVR chip in 4 bit mode.

The program is designed to be compiled with the Evaluation
version, so it's capabilities are limited.

The demo is designed to run on any ATmega328P
development board with chip clock of 16 MHz.
(Arduino Uno board)

A CX12864B-1 128x64 pixels graphic LCD module was used.

The LCD module to Arduino Uno connections are:
 7 DB0  - not connected
 8 DB1  - not connected
 9 DB2  - not connected
10 DB3  - not connected
11 DB4  - PORTD bit 0 (Digital 0)
12 DB5  - PORTD bit 1 (Digital 1)
13 DB6  - PORTD bit 2 (Digital 2)
14 DB7  - PORTD bit 3 (Digital 3)

 6 E    - PORTD bit 4 (Digital 4)
 5 R/W  - PORTD bit 5 (Digital 5)
 4 RS   - PORTD bit 6 (Digital 6)
17 /RST - PORTD bit 7 (Digital 7)

15 PSB  - +5V

 1 VSS  - GND
 2 VDD  - +5V
 3 VO   - -------------| Contrast control voltage
                       |
                       V  10k
                 |-----------|
18 Vout - -------|           |--|GND
                 |-----------|

The connections can be modified by accessing the
Project|Configure|C Compiler|Libraries|Graphic LCD menu.
**********************************************************/

#include <glcd.h>
#include <delay.h>

/* User defined characters */
flash char user_def_char[4][32]=
{
/* Character with code: 0 */
{
0b00000000,0b11111111,
0b00000000,0b00000011,
0b00000000,0b00000101,
0b00000000,0b00001001,
0b00000000,0b00010001,
0b00000000,0b00100001,
0b00000000,0b01000001,
0b00000000,0b10000001,
0b00000001,0b00000000,
0b00000010,0b00000000,
0b00000100,0b00000000,
0b00001000,0b00000000,
0b00010000,0b00000000,
0b00100000,0b00000000,
0b01000000,0b00000000,
0b10000000,0b00000000
},
/* Character with code: 2 */
{
0b00000000,0b00000001,
0b00000000,0b00000010,
0b00000000,0b00000100,
0b00000000,0b00001000,
0b00000000,0b00010000,
0b00000000,0b00100000,
0b00000000,0b01000000,
0b00000000,0b10000000,
0b10000001,0b00000000,
0b10000010,0b00000000,
0b10000100,0b00000000,
0b10001000,0b00000000,
0b10010000,0b00000000,
0b10100000,0b00000000,
0b11000000,0b00000000,
0b11111111,0b00000000
},
/* Character with code: 4 */
{
0b11111111,0b11111111,
0b10000000,0b00000011,
0b10000000,0b00000101,
0b10000000,0b00001001,
0b10000000,0b00010001,
0b10000000,0b00100001,
0b10000000,0b01000001,
0b10000000,0b10000001,
0b10000001,0b00000001,
0b10000010,0b00000001,
0b10000100,0b00000001,
0b10001000,0b00000001,
0b10010000,0b00000001,
0b10100000,0b00000001,
0b11000000,0b00000001,
0b11111111,0b11111111
},
/* Character with code: 6 */
{
0b11111111,0b11111111,
0b11000000,0b00000001,
0b10100000,0b00000001,
0b10010000,0b00000001,
0b10001000,0b00000001,
0b10000100,0b00000001,
0b10000010,0b00000001,
0b10000001,0b00000001,
0b10000000,0b10000001,
0b10000000,0b01000001,
0b10000000,0b00100001,
0b10000000,0b00010001,
0b10000000,0b00001001,
0b10000000,0b00000101,
0b10000000,0b00000011,
0b11111111,0b11111111
}
};

void main()
{
unsigned char c;
GLCDINIT_t init;

/* No font is used */
init.font=NULL;
 
/* No need for reading data from external memory */
init.readxmem=NULL;

/* No need for reading data from external memory */ 
init.writexmem=NULL;

/* Initialize the LCD controller and graphics */
glcd_init(&init); 

/* Display text using the built-in character generator */
lcd_putsf("Sitronix ST7920\nCodeVisionAVR\nDemo");

/* 2 seconds delay */          
delay_ms(2000);
lcd_clear();

/* Define 4 characters with codes: 0, 2, 4, 6 */
for (c=0; c<=3; c++)
    glcd_definechar(c*2,user_def_char[c]);

lcd_putsf("User defined\n"
          "characters:\n");

/* User defined characters must be located on even x coordinates */
/* Display user defined characters with codes: 0, 2, 4, 6 */
for (c=0; c<=6; c+=2)
    {
    lcd_putchar(0); /* Always first write MSB=0 */
    lcd_putchar(c); /* Write LSB=character code */
    }

}
