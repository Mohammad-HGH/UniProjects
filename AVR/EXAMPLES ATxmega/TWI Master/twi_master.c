/*
Sample program to test the ATxmega128A1 TWID and USARTC0

Copyright (C) 2010 Pavel Haiduc and HP InfoTech S.R.L.,
All rights reserved.
 
A Maxim/Dallas Semiconductor DS1621 Digital Thermometer
and Thermostat is connected to the TWID interface.
The measured temperature is serialy outputed using USARTC0
and displayed on the CodeVisionAVR Terminal.

NOTE: AS THE TWIX LIBRARY IS NOT SUPPLIED WITH THE
EVALUATION VERSION OF CODEVISIONAVR, THIS PROJECT
CAN'T BE SUCCESSFULLY COMPILED WITH THIS VERSION

In order to test the code functionality,
use the Tools|Programmer to program the ATxmega128A1
FLASH with the contents of the \Exe\twi_master.rom file.
 
Use a STK600 development board with STK600-TQFP100 and
STK600-RC100X-13 addapters

The STK600 programmer must be set in JTAG programming mode
in the Tools|Programmer menu.

Make sure that the VTARGET and VREF voltages are set to 3.6V
using AVR Studio.
The VTARGET LED on the STK600 board must be lighted.

Make the following connections on the STK600:

PC3 - RS232 SPARE TXD
PD0 - DS1621 SDA pin 1 - 4.7K resistor to VTG
PD1 - DS1621 SCL pin 2 - 4.7K resistor to VTG
GND - DS1621 GND pin 4
GND - DS1621 A0 pin 7
GND - DS1621 A1 pin 6
GND - DS1621 A2 pin 5
VTG - DS1621 VDD pin 8.

Connect the STK600 RS232 output to the COM port of the PC.
Make sure to configure the CodeVisionAVR Terminal
to 9600bps 8N1, Append LF on reception: disabled.
*/

// USARTC0 initialization function
#include "usartx.h"
// TWI functions for ATxmega devices
#include <twix.h>
// DS1621 functions for ATxmega devices
#include "ds1621x.h"
// printf
#include <stdio.h>
// delay functions
#include <delay.h>

// TWI clock rate [bps]
#define TWI_CLK_RATE 100000

// DS1621 chip #
#define DS1621_CHIP 0
// DS1621 low temperature limit [C]
#define DS1621_TEMP_LOW 25
// DS1621 high temperature limit [C]
#define DS1621_TEMP_HIGH 28
// DS1621 Tout polarity active high
#define DS1621_TOUT 1

// structure that holds information used by the TWID master
// for performing a TWI bus transaction
TWI_MASTER_INFO_t twid_master;

interrupt [TWID_TWIM_vect] void twid_master_isr(void)
{
twi_master_int_handler(&twid_master);
}

void main(void)
{
// USARTC0 TX initialization at Baud rate=9600 
usartc0_tx_init();
printf("USARTC0 is initialized\r\n");

// general TWID initialization
// no external driver interface
// no SDA hold time
twi_init(&TWID,false,false);

// initialize the TWID master
twi_master_init(&twid_master,&TWID,TWI_MASTER_INTLVL_LO_gc,TWI_BAUD_REG(_MCU_CLOCK_FREQUENCY_,TWI_CLK_RATE));
printf("TWID master is initialized\r\n");

// enable LO interrupt level
PMIC.CTRL |= PMIC_LOLVLEN_bm;
// enable interrupts
#asm("sei")

// initialize the DS1621 chip #0 connected to TWID
ds1621_init(&twid_master,DS1621_CHIP,DS1621_TEMP_LOW,DS1621_TEMP_HIGH,DS1621_TOUT);
printf("DS1621 #0 is initialized\r\n");

// display the temperature measured by the DS1621 chip #0
while (1)
      {
      printf("t=%.1f C\r\n",ds1621_temperature(DS1621_CHIP));
      delay_ms(500); // 500ms delay
      }
}