/*
Sample program to test the ATxmega TWIC master and
TWID slave operation.

Copyright (C) 2010 Pavel Haiduc and HP InfoTech S.R.L.,
All rights reserved.
 
If one or several switches SW0..SW7 are pressed
on the STK600 board, their state is transmitted
by the TWIC master to the TWID slave, which
displays the received data on the LED0..LED7.
The slave sends to the master the contents of the
test_data array.

NOTE: AS THE TWIX LIBRARY IS NOT SUPPLIED WITH THE
EVALUATION VERSION OF CODEVISIONAVR, THIS PROJECT
CAN'T BE SUCCESSFULLY COMPILED WITH THIS VERSION

In order to test the code functionality,
use the Tools|Programmer to program the ATxmega128A1
FLASH with the contents of the \Exe\twi_slave.rom file.
 
Use a STK600 development board with STK600-TQFP100 and
STK600-RC100X-13 addapters

The STK600 programmer must be set in JTAG programming mode
in the Tools|Programmer menu.

Make sure that the VTARGET and VREF voltages are set to 3.6V
using AVR Studio.
The VTARGET LED on the STK600 board must be lighted.

Make the following connections on the STK600:

PC0 - PD0 SDA pin - 4.7K resistor to VTG
PC1 - PD1 SCL pin - 4.7K resistor to VTG
SW0..SW7 - PE0..PE7 using a ribbon cable with 10 pin connectors
LED0..LED7 - PA0..PA7 using a ribbon cable with 10 pin connectors.
*/

#include <twix.h>
#include <string.h>
#include <delay.h>

// TWI clock rate [bps]
#define TWI_CLK_RATE 100000
// 7 bit TWI slave address
#define SLAVE_ADDR 0x5A

// structure that holds information used by the TWIC master
// for performing a TWI bus transaction
TWI_MASTER_INFO_t twi_master;

// TWIC master interrupt service routine
interrupt [TWIC_TWIM_vect] void twic_master_isr(void)
{
twi_master_int_handler(&twi_master);
}

// TWID slave receive and transmit buffers
unsigned char twi_slave_rx_buffer[16];
// data to be transmitted to the master
unsigned char test_data[]="TWI test";
 
// structure that holds information used by the TWID slave
// for performing a TWI bus transaction
TWI_SLAVE_INFO_t twi_slave;

// TWID slave interrupt service routine
interrupt [TWID_TWIS_vect] void twid_slave_isr(void)
{
twi_slave_int_handler(&twi_slave);
}

// TWID slave transaction processing function
void twi_slave_trans(void)
{
// read the received data from the buffer and output it to the LEDs on PORTA
PORTA.OUT=twi_slave.rx_buffer[twi_slave.rx_index];

// prepare to transmit the contents of the test_data array to the master
// initialize the number of bytes to transmit only once at the beginning of the transmission
if (twi_slave.bytes_to_tx==0) twi_slave.bytes_to_tx=sizeof(test_data);
}

void main(void)
{
unsigned char switches;
unsigned char rx_data[sizeof(test_data)]; // received data from the slave

// initialize PORTA as inverted outputs, used for driving LEDs
PORTA.OUT=0x00; // all LEDs initially off
PORTA.DIR=0xFF;
PORTA.PIN0CTRL=PORT_INVEN_bm | PORT_OPC_TOTEM_gc;
PORTA.PIN1CTRL=PORT_INVEN_bm | PORT_OPC_TOTEM_gc;
PORTA.PIN2CTRL=PORT_INVEN_bm | PORT_OPC_TOTEM_gc;
PORTA.PIN3CTRL=PORT_INVEN_bm | PORT_OPC_TOTEM_gc;
PORTA.PIN4CTRL=PORT_INVEN_bm | PORT_OPC_TOTEM_gc;
PORTA.PIN5CTRL=PORT_INVEN_bm | PORT_OPC_TOTEM_gc;
PORTA.PIN6CTRL=PORT_INVEN_bm | PORT_OPC_TOTEM_gc;
PORTA.PIN7CTRL=PORT_INVEN_bm | PORT_OPC_TOTEM_gc;

// initialize PORTE as inputs used for reading switches
// pullup resistors are already present on the STK600 board 
PORTE.DIR=0x00;

// general TWIC initialization
// no external driver interface
// no SDA hold time
twi_init(&TWIC,false,false);

// initialize the TWIC master
// use low priority level interrupt
twi_master_init(&twi_master,&TWIC,TWI_MASTER_INTLVL_LO_gc,
    TWI_BAUD_REG(_MCU_CLOCK_FREQUENCY_,TWI_CLK_RATE));

// general TWID initialization
// no external driver interface
// no SDA hold time
twi_init(&TWID,false,false);

// initialize the TWID slave
// use low priority level interrupt
twi_slave_init(&twi_slave,&TWID,TWI_SLAVE_INTLVL_LO_gc,false,SLAVE_ADDR,0,
    twi_slave_rx_buffer,sizeof(twi_slave_rx_buffer),
    test_data,twi_slave_trans);

// enable low interrupt level interrupts
PMIC.CTRL|=PMIC_LOLVLEN_bm;

// globaly enable interrupts
#asm("sei")

while (1)
      {
      // read the SW0..7 switches
      // the switches value is inverted because the connection is established to GND
      // when a switch is pressed
      switches= ~PORTE.IN;
      // check if at least one switch was pressed
      if (switches)
         {
         // yes, transmit the switches state to the slave
         // and receive the contents of the test_data array sent by the slave in rx_data
         twi_master_trans(&twi_master,SLAVE_ADDR,&switches,sizeof(switches),rx_data,sizeof(rx_data));
         // check that correct data was received from the slave
         if (strncmp(rx_data,test_data,sizeof(test_data)))
            // rx_data doesn't match test_data
            while (1)
                  {
                  PORTA.OUT=0xFF; // flash all LEDs to signal the mismatch
                  delay_ms(200);
                  PORTA.OUT=0x00;
                  delay_ms(200);
                  }  
         }
      }
}
