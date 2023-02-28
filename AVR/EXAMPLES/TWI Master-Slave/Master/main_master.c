/* Example program for I2C bus communication
   as a master using TWI

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
   with a confirmation message, which will be
   then transmitted by the master development
   board by RS232 to a COM port of the PC. 
      
   The CodeVisionAVR Terminal can be used to
   display this message.

   Serial communication parameters: 9600 8N1

   I2C bus port bits allocations for TWI on PORTC:
   SDA=bit 4, SCL=bit 5
   These signals must be connected to the corresponding
   SDA and SCL signals of the I2C slave board.  
   
   NOTE: 3.3k..4.7k PULL-UP RESITORS TO +5V MUST
   BE USED FOR THE SDA & SCL I2C BUS SIGNALS
*/


#include <twi.h>
#include <stdio.h>
#include <string.h>
#include <delay.h>
#include <io.h>

// 7bit slave I2C address
#define TWI_SLAVE_ADDR 0x50

char tx_buffer[80]; // transmission buffer
char rx_buffer[80]; // receive buffer

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

void main(void)
{
unsigned int i=0;

// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: Off
// USART Transmitter: On
// USART0 Mode: Asynchronous
// USART Baud Rate: 9600 @ 16MHz
UCSR0A=0x00;
UCSR0B=0x08;
UCSR0C=0x06;
UBRR0H=0x00;
UBRR0L=0x67;

// initialize the TWI master to use a SCL bit rate of 100kHz
twi_master_init(100);

printf("TWI master OK\r\n");

#asm("sei") // enable interrupts to start TWI communication

while(1)
     {
     // prepare the data to be transmitted to the slave
     sprintf(tx_buffer,"Packet %u",i++);
     printf("Transmitted to the TWI slave: %s\r\n",tx_buffer);
     // transmit data to the slave and receive response in rx_buffer
     if (twi_master_trans(TWI_SLAVE_ADDR,tx_buffer,strlen(tx_buffer),rx_buffer,sizeof(rx_buffer)))
        {
        // TWI transaction was performed without errors
        printf("TWI transaction result: %p\r\n",status_msg[twi_result]);
        if (twi_rx_index)
           {
           // make sure that the slave response is NULL terminated
           rx_buffer[twi_rx_index]=NULL;
           // display the slave response on the Terminal
           printf("TWI slave response: %s\r\n",rx_buffer);
           }
        }
     else printf("TWI transaction error: %p\r\n\r\n",status_msg[twi_result]);
     delay_ms(500);
     };
}
