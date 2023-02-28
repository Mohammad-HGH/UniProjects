/* Multipoint thermometer with LCD display
   using the Dallas Semiconductor DS1820/18S20
   1 Wire bus temperature sensors

   CodeVisionAVR C Compiler
   (C) 2000-2010 HP InfoTech S.R.L.
   www.hpinfotech.ro

   Chip: ATmega8515
   Memory Model: SMALL
   Data Stack Size: 128 bytes
   
   PLEASE MAKE SURE THAT THE CKSEL0..3 FUSE
   BITS ARE PROGRAMMED TO USE THE EXTERNAL
   3.6864MHz CLOCK SOURCE OF THE STK500 AND NOT
   THE INTERNAL 1MHz OSCILLATOR.
   The ATmega8515 chip comes from the factory
   with CKSEL0..3 fuse bits set to use the
   internal 1 MHz oscillator.

   The DS1820/18S20 sensors are connected to
   bit 6 of PORTA of the ATmega8515 as follows:

   [DS1820/18S20] [STK500 PORTA HEADER]
    1 GND         -   9  GND
    2 DQ          -   7  PA6
    3 VDD         -  10 +5V

   All the temperature sensors must be connected
   in parallel
   
   AN 4.7k PULLUP RESISTOR MUST BE CONNECTED
   BETWEEN DQ (PA6) AND +5V !
*/


/* Use an 2x16 alphanumeric LCD connected
   to PORTC as follows:

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
#include <ds1820.h>
#include <delay.h>
#include <stdio.h>

char lcd_buffer[33];

/* maximum number of DS1820/DS18S20 connected to the 1 Wire bus */
#define MAX_DEVICES 8

/* DS1820/DS18S20 devices ROM code storage area */
unsigned char rom_code[MAX_DEVICES][9];

main()
{
unsigned char i,j,devices;
int temp;

lcd_init(16);
lcd_putsf("CodeVisionAVR\n1 Wire Bus Demo");
delay_ms(2000);
lcd_clear();

/* detect how many DS1820/DS18S20 devices
   are connected to the 1 Wire bus */
devices=w1_search(0xf0,rom_code);
sprintf(lcd_buffer,"%u DS1820\nDevice detected",devices);
lcd_puts(lcd_buffer);
delay_ms(2000);

/* display the ROM codes for each device */
if (devices)
   {
   for (i=0;i<devices;i++)
       {
       sprintf(lcd_buffer,"Device #%u ROM\nCode is:",i+1);
       lcd_clear();
       lcd_puts(lcd_buffer);
       delay_ms(2000);
       lcd_clear();
       for (j=0;j<8;j++)
           {
           sprintf(lcd_buffer,"%02X ",rom_code[i][j]);
           lcd_puts(lcd_buffer);
           if (j==3) lcd_gotoxy(0,1);
           };
       delay_ms(5000);
       };
   }
else
while (1); /* stop here if no devices were found */

/* measure and display the temperature(s) */       
while (1)
      {
      for (i=0;i<devices;)
          {
          temp=ds1820_temperature_10(&rom_code[i][0]);
          j='+';
          if (temp<0)
             {
             j='-';
             temp=-temp;
             };
          sprintf(lcd_buffer,"t%u=%c%i.%u\xdf" "C",++i,j,temp/10,temp%10);
          lcd_clear();
          lcd_puts(lcd_buffer);
          delay_ms(800);
          };
      };
}
