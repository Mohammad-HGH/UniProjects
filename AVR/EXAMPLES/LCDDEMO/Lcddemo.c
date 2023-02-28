/* 
  LCD Demo
  
  CodeVisionAVR C Compiler
  (C) 2000-2010 HP InfoTech S.R.L.
  www.hpinfotech.ro
  
  Chip: ATmega8515
  Memory Model: SMALL
  Data Stack Size: 128 bytes

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
*/

// Alphanumeric LCD Module functions
// The LCD connections are specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric menu
#include <alcd.h>

void main(void)
{
// initialize the LCD for
// 2 lines & 16 columns
lcd_init(16);

// go on the second LCD line
lcd_gotoxy(0,1);

// display the message
lcd_putsf("Hello world");

// stop here
while (1);
}

