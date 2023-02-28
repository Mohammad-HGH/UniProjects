/* 
  LCD user defined characters demo
  
  CodeVisionAVR C Compiler
  (C) 2000-2007 HP InfoTech S.R.L.
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

typedef unsigned char byte;

// table for the user defined character
// arrow that points to the top right corner
flash byte char_table[8]={
0b10000000,
0b10001111,
0b10000011,
0b10000101,
0b10001001,
0b10010000,
0b10100000,
0b11000000};

// tables that hold the messages to be displayed
flash char msg_ok[]="OK";
flash char msg_error[]="ERROR";

// function used to define user characters
void define_char(byte flash *pc,byte char_code)
{
byte i,address;
address=(char_code<<3)|0x40;
for (i=0; i<8; i++) lcd_write_byte(address++,*pc++);
}

void main(void)
{
byte i,address;
// pointer to the messages to be displayed
flash char *pmsg=msg_ok; // presume all will be OK

// initialize the LCD for
// 2 lines & 16 columns
lcd_init(16);

// define user character 0
define_char(char_table,0);

// switch to writing in Display RAM
lcd_gotoxy(0,0);
// display the user defined characters
lcd_putsf("User defined\nchar 0:");
// display the user defined char 0
lcd_putchar(0);

// read back the data from the LCD module
// and check if it's the same with the char_table array
address=0x40; // character generator starting address
for (i=0; i<8; i++)
    if (lcd_read_byte(address++)!=char_table[i])
       {
       // display an error message in case of data not matching
       pmsg=msg_error;
       // stop here
       break;
       }
       
// ensure that the correct LCD cursor is at the correct position
// after lcd_read_byte was executed
lcd_gotoxy(9,1);
// display the message 
lcd_putsf(pmsg);
// stop here
while (1);
}
