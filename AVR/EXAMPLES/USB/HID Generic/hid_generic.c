/****************************************************************
USB Generic HID Example
© Copyright 2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

CodeVisionAVR Compiler version 3.11 or later
is required.

The program is designed to be executed on the
Atmel AT90USBKEY development board, using
an AT90USB1287 chip running at 8.000000 MHz.

In order to use this example, the
'Generic Hid Test.exe' program must be
launched on the PC.
This program requires the .NET framework 4.0
to be installed on the computer.
It can be downloaded from:
http://www.microsoft.com/en-us/download/details.aspx?id=17718

Note: The source code for the 'Generic Hid Test.exe' program
is supplied only with the commercial versions of CodeVisionAVR.

The AT90USBKEY, programmed with this example's
code, must be connected to an USB port of the PC.

The 'Generic Hid Test' program will display the
report data sent by the AT90USBKEY to the PC host.

Checking the LED boxes in the 'Generic Hid Test'
program, will send report data to the USB device
and will light the appropriate LEDs on the
AT90USBKEY board.
The LEDs are connected to PORTD bit 4..7 outputs.
****************************************************************/

#include <90usb1287.h>
#include <delay.h>

// USB Device functions
#include <usb_device.h>

// USB Human Interface Device functions
#include <usb_hid.h>

// USB initialization
#include "usb_init.h"

unsigned char rx_buffer[64];
unsigned char tx_buffer[64]={0};

void main(void)
{
unsigned short nbytes;

// Crystal Oscillator division factor: 1
#pragma optsize-
CLKPR=(1<<CLKPCE);
CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif

// Use PORTD bits 4..7 as outputs to drive the 4 LEDs
DDRD=0xF0;

// USB Controller initialization in device mode
usb_init_device(&usb_config);

// Globally enable interrupts
#asm("sei")

// Wait for the USB device to be enumerated by the host
while (!usb_enumerated);

// Wait 1.5 seconds for the operating system to
// load any drivers needed by the USB device
delay_ms(1500);

// Configure Timer1 to generate a timer overflow interrupt every 1 second
// The AVR chip is clocked at 8 MHz
TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<COM1C1) | (0<<COM1C0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (1<<CS12) | (0<<CS11) | (0<<CS10);
TCNT1H=0x85;
TCNT1L=0xEE;

// Enable Timer1 overflow interrupt
TIMSK1=(0<<ICIE1) | (0<<OCIE1C) | (0<<OCIE1B) | (0<<OCIE1A) | (1<<TOIE1);

while (1)
      {
      // Check if we have received some data
      nbytes=sizeof(rx_buffer); // Number of bytes to receive
      if (usb_getbuf(rx_buffer,&nbytes)==USB_RES_OK)
         {
         // Output the received data byte 0 to PORTD bits 4..7 and
         // light the corresponding LEDs
         PORTD=rx_buffer[0]<<4;
         }
      }
}

// Timer1 overflow interrupt service routine
// occurs every 1 second
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
static unsigned short counter=0;

// Reinitialize Timer1 value
TCNT1H=0x85EE >> 8;
TCNT1L=0x85EE & 0xff;

// Write the counter in the first 2 bytes of the transmitted data
tx_buffer[0]=counter & 0xFF;
tx_buffer[1]=counter >> 8;

// Write the LEDs current light state into byte #3
tx_buffer[2]=PORTD>>4;

// Send buffer's content to the host
usb_putbuf(tx_buffer,sizeof(tx_buffer));

// Increment the counter
++counter;
}
