/* Multipoint thermometer with LCD display
   using the Maxim DS18B20
   1 Wire bus temperature sensors

   CodeVisionAVR C Compiler
   (C) 2000-2010 HP InfoTech S.R.L.
   www.hpinfotech.ro

   Chip: ATxmega128A1

   The DS18B20 sensors are connected to
   bit 6 of PORTA of the ATxmega128A1 as follows:

   [DS18B20]     [STK500 PORTA HEADER]
    1 GND         -   9  GND
    2 DQ          -   7  PA6
    3 VDD         -  10 +5V

   All the temperature sensors must be connected
   in parallel

   AN 4.7k PULLUP RESISTOR MUST BE CONNECTED
   BETWEEN DQ (PA6) AND +5V !
*/

/*
Make the following connections on the STK600
PC3 - RS232 SPARE TXD

Connect the STK600 RS232 output to the COM port of the PC
Make sure to configure the CodeVisionAVR Terminal
to 9600bps 8N1

*/

#include <io.h>
#include <ds18b20.h>
#include <delay.h>
#include <stdio.h>
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


/* maximum number of DS18B20 connected to the 1 Wire bus */
#define MAX_DEVICES 8

/* DS18B20 devices ROM code storage area */
unsigned char rom_code[MAX_DEVICES][9];

main()
{
unsigned char i,j,devices;

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

printf("CodeVisionAVR ATxmega 1 Wire Bus Demo\n\r");

/* detect how many DS18B20 devices
   are connected to the 1 Wire bus */
devices=w1_search(0xf0,rom_code);
printf("%u DS18B20 Device(s) detected\n\r",devices);

/* display the ROM codes for each device */
if (devices)
   {
   for (i=0;i<devices;i++)
       {
       printf("Device #%u ROM Code is:",i+1);
       for (j=0;j<8;j++)
           {
           printf("%02X ",rom_code[i][j]);
           };
       };
   printf("\n\r");
   }
else
while (1); /* stop here if no devices were found */

/* configure each DS18B20 device for 12 bit temperature
   measurement resolution */
for (i=0;i<devices;)
    if (!ds18b20_init(&rom_code[i++][0],20,30,DS18B20_12BIT_RES))
       {
       printf("Init error for device #%u",i);
       while (1); /* stop here if init error */
       };

/* measure and display the temperature(s) */
while (1)
      {
      j=1;
      for (i=0;i<devices;i++)
          {
          printf("t%u=%+.3f\xdf" "C\n\r",j++,ds18b20_temperature(&rom_code[i][0]));
          delay_ms(500);
          };
      };
}
