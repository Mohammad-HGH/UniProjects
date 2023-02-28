/* Example program for I2C bus communication
   as a slave using TWI

   CodeVisionAVR V2.05.1+ C Compiler
   (C) 2000-2011 HP InfoTech S.R.L.
   www.hpinfotech.ro

   Chip: ATmega328P
   Memory Model: SMALL
   Data Stack Size: 512 bytes
   Clock frequency: 16.0 MHz
   
   The master will transmit packets of
   data to the slave using the I2C bus.
   The slave will respond back through the I2C bus
   with a confirmation message.
   The slave will also display the contents
   of the received data packets on a 16x2 LCD
   connected as outlined below:
   
   [LCD]    [AVR]
    1 GND - GND
    2 +5V - VCC  
    3 VLC - LCD contrast control voltage 0..1V
    4 RS  - PB0
    5 RD  - GND
    6 EN  - PB1
   11 D4  - PD4
   12 D5  - PD5
   13 D6  - PD6
   14 D7  - PD7

   The LCD connections are specified in the
   Project|Configure|C Compiler|Libraries|Alphanumeric menu

   I2C bus port bits allocations for TWI on PORTC:
   SDA=bit 4, SCL=bit 5
   These signals must be connected to the corresponding
   SDA and SCL signals of the I2C master board.  
   
   NOTE: 3.3k..4.7k PULL-UP RESITORS TO +5V MUST
   BE USED FOR THE SDA & SCL I2C BUS SIGNALS
*/

#include <io.h>
#include <stdio.h>
#include <twi.h>
#include <delay.h>
#include <alcd.h>
#include <string.h>

// 7bit slave I2C address
#define TWI_SLAVE_ADDR 0x50

char rx_buffer[32]; // slave receive buffer
char tx_buffer[32]; // slave transmission buffer

// flag that signals that the TWI slave reception was OK
bit received_ok=false;

flash char * flash status_msg[8]=
{
"OK",
"Buffer overflow",
"Arbitration lost",
"Bus error",
"NACK received",
"Bus timeout",
"Fail",
"Unknown error"
};

bool slave_rx_handler(bool rx_complete)
{
if (twi_result==TWI_RES_OK) received_ok=true; // signal that data was received without errors
else
   {
   // TWI receive error, display the twi_result value
   lcd_clear();
   lcd_putsf("Receive error:\n");
   lcd_putsf(status_msg[twi_result]);
   received_ok=false; // signal that data was received with errors
   return false; // stop reception
   }
   
if (rx_complete)
   // the TWI master has finished transmitting data
   return false; // no more bytes to receive
   
// signal to the TWI master that the TWI slave is ready to accept more data
// as long as there is space in the receive buffer
return (twi_rx_index<sizeof(rx_buffer));
}

unsigned char slave_tx_handler(bool tx_complete)
{
unsigned char i;
if (tx_complete==false)
   {
   // transmission from slave to master is about to start
   // copy the text to transmit to the TWI master in the transmission buffer
   strcpyf(tx_buffer,"Data packet received OK\n\r");
   // # of bytes to transmit from the TWI slave to the TWI master
   return strlen(tx_buffer);
   }
// transmission from slave to master has already started,
// no more bytes to send in this transaction
if (received_ok)
   {
   // no TWI receive error, display the received data on the LCD
   lcd_clear();
   for (i=0;i<twi_rx_index;i++) lcd_putchar(rx_buffer[i]);
   }
return 0; 
}

void main(void)
{
// initialize the LCD
lcd_init(16);
lcd_clear();

// initialize the TWI slave
twi_slave_init(false,TWI_SLAVE_ADDR,rx_buffer,sizeof(rx_buffer),tx_buffer,slave_rx_handler,slave_tx_handler);

lcd_putsf("TWI slave OK");
delay_ms(2000);

// enable interrupts to start TWI communication
#asm("sei")

// all processing is performed by TWI interrupts inside twi.lib
while (1);
}
