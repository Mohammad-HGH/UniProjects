/* Dallas Semiconductor DS1990 Serial Number
   1 Wire iButton demo

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

   The DS1990 devices are connected to
   bit 6 of PORTA of the ATmega8515 on the STK500
   development board as follows:

   [DS1990]      [PORTA header]
    1 GND         -   9  GND
    2 DATA        -   7  PA6

   All the devices must be connected in parallel
   
   A 4.7k PULLUP RESISTOR MUST BE CONNECTED
   BETWEEN DATA (PA6) AND PORTA HEADER PIN 10 (VTG) !

   In order to use the RS232 SPARE connector
   on the STK500, the following connections must
   be made:
   [RS232 SPARE header] [PORTD header]
   RXD                - 1 PD0
   TXD                - 2 PD1
*/

#include <1wire.h>
#include <mega8515.h>
#include <stdio.h>

#define DS1990_FAMILY_CODE 1
#define SEARCH_ROM 0xF0

/* DS1990 devices ROM code storage area,
   9 bytes are used for each device
   (see the w1_search function description),
   but only the first 8 bytes contain the ROM code
   and CRC */
#define MAX_DEVICES 8
unsigned char rom_code[MAX_DEVICES][9];

main() {
unsigned char i,j,devices;
unsigned char n=1;

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

// print welcome message
printf("DS1990 Serial Number iButton demo\n\r");

// detect how many 1 Wire devices are present on the bus
devices=w1_search(SEARCH_ROM,&rom_code[0][0]);
printf("%u device(s) found\n\r",devices);
for (i=0;i<devices;i++)
    // make sure to select only the DS1990 types
    if (rom_code[i][0]==DS1990_FAMILY_CODE)
       {
       printf("DS1990 #%u serial number:",n++);
       for (j=1;j<=6;j++)
           printf(" %02X",rom_code[i][j]); 
       printf("\n\r");
       };
}
