/* Thermometer with RS232 serial data transmision
   using the National Semiconductor LM75 I2C bus
   temperature sensor

   Chip: ATmega8515
   Memory Model: SMALL
   Data Stack Size: 128 bytes
   Clock frequency: 3.6864 MHz
   
   PLEASE MAKE SURE THAT THE CKSEL0..3 FUSE
   BITS ARE PROGRAMMED TO USE THE EXTERNAL
   CLOCK SOURCE OF THE STK500 AND NOT
   THE INTERNAL 1MHz OSCILLATOR.
   The ATmega8515 chip comes from the factory
   with CKSEL0..3 fuse bits set to use the
   internal 1 MHz oscillator.
   
   Communication parameters: 9600 8N1

   CodeVisionAVR V2.05.1+ C Compiler
   (C) 2000-2011 HP InfoTech S.R.L.
   www.hpinfotech.ro

   In order to use the RS232 SPARE connector
   on the STK500, the following connections must
   be made:
   [RS232 SPARE header] [PORTD header]
   RXD                - 1 PD0
   TXD                - 2 PD1

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
*/

// LM75 driver routines
#include <lm75.h>
// printf
#include <stdio.h>
// ATmega8515 I/O register definitions
#include <mega8515.h>
#include <delay.h> 

void main(void)
{
char sign;
int temp;

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

// initialize the I2C bus
i2c_init();

// initialize the LM75 chip with address 0
// hysterezis temperature 20ÝC
// O.S. temperature 25ÝC
// O.S. output is active high
lm75_init(0,20,25,1);

// temperature transmission loop
while (1)
      {
      // read LM75 temperature *10ÝC
      // from chip with address 0
      temp=lm75_temperature_10(0);
      delay_ms(300);
      sign='+';
      if (temp<0)
         {
         sign='-';
         temp=-temp;
         };
      // send the measured temperature via the
      // RS232 serial communication
      printf("t=%c%i.%u\xf8" "C\r\n",sign,temp/10,temp%10);
      }
}
