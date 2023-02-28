/****************************************************************
USB Generic HID Example
© Copyright 2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

CodeVisionAVR Compiler version 3.13 or later
is required.

The program is designed to be executed on the
Atmel STK600 development board, using
an ATxmega128A4U chip.

This program exceeds the allowed code size
of the Evaluation version.
In this case you can program the ATxmega128A4U FLASH memory with
the contents of the \Exe\HID Generic.hex file.

The chip uses the internal 32 MHz RC oscillator,
adjusted and calibrated to 48 MHz using the
DFLL and USB Start Of Frame.
The CPU and Peripheral Clock frequency are set to 12 MHz.

The STK600-TQFP44 socket and STK600-RC044X-55 routing
cards are used for interfacing the chip with
the STK600.

In order to use this example, the 'Generic Hid Test.exe'
program must be launched on the PC.
This program requires the .NET framework 4.0 to be installed
on the computer.
It can be downloaded from:
http://www.microsoft.com/en-us/download/details.aspx?id=17718

Note: The source code for the 'Generic Hid Test.exe' program
is supplied only with the commercial versions of CodeVisionAVR.

The STK600 with the ATxmega128A4U chip, programmed with this
example's code, must be connected to an USB port of the PC.
The STK600's mini USB connector and an appropriate cable
must be used for this purpose.

The 'Generic Hid Test' program will display the report data
sent by the STK600 to the PC host.

Checking the LED boxes in the 'Generic Hid Test' program,
will send report data to the USB device and will light
the appropriate LEDs on the STK600 board.
The LEDs are connected to PORTA pin 0..7 outputs.
For this purpose use a 10 pin ribbon cable to connect
the PORTA and LED headers.
****************************************************************/

#include <io.h>
#include <delay.h>

// USB Device functions
#include <usb_device.h>

// USB Human Interface Device functions
#include <usb_hid.h>

// Chip peripherals initialization
#include "peripherals_init.h"

// USB initialization
#include "usb_init.h"

unsigned char rx_buffer[64];
unsigned char tx_buffer[64]={0};

void main(void)
{
unsigned char n;
unsigned short nbytes;

// Interrupt system initialization
interrupts_init();

// System Clocks initialization to use the internal 32MHz oscillator
system_clocks_init();

// Timer/Counter TCC0 initialization
tcc0_init();

// PORTA is mapped to VPORT0
// PORTB is mapped to VPORT1
PORTCFG.VPCTRLA=PORTCFG_VP13MAP_PORTB_gc | PORTCFG_VP02MAP_PORTA_gc;

// PORTA outputs are inverted totem/pole
PORTA.PIN0CTRL=PORT_INVEN_bm | PORT_OPC_TOTEM_gc;
PORTA.PIN1CTRL=PORT_INVEN_bm | PORT_OPC_TOTEM_gc;
PORTA.PIN2CTRL=PORT_INVEN_bm | PORT_OPC_TOTEM_gc;
PORTA.PIN3CTRL=PORT_INVEN_bm | PORT_OPC_TOTEM_gc;
PORTA.PIN4CTRL=PORT_INVEN_bm | PORT_OPC_TOTEM_gc;
PORTA.PIN5CTRL=PORT_INVEN_bm | PORT_OPC_TOTEM_gc;
PORTA.PIN6CTRL=PORT_INVEN_bm | PORT_OPC_TOTEM_gc;
PORTA.PIN7CTRL=PORT_INVEN_bm | PORT_OPC_TOTEM_gc;

// Use Virtual Port VPORT0 pins 0..7 as outputs
// to drive the LEDs
VPORT0_DIR=0xFF;

// Initially all LEDs are off
VPORT0_OUT=0;

// USB Controller initialization

// Use the internal 32 MHz RC oscillator, adjusted and
// calibrated by the the DFLL and USB Start of Frame
// to 48 MHz, as USB clock source
// USB clock prescaler division factor: 1
CLK.USBCTRL=CLK_USBSRC_RC32M_gc | CLK_USBSEN_bm | CLK_USBPSDIV_1_gc;

// Initialize the USB Controller in Full Speed, Device mode
usb_init_device(&usb_config);

// Globally enable interrupts
#asm("sei")

// Wait for the USB device to be enumerated by the host
while (!usb_enumerated);

// Wait 1.5 seconds for the operating system to
// load any drivers needed by the USB device
delay_ms(1500);

// Enable Timer/Counter TCC0 overflow interrupt
TCC0.INTCTRLA=TC_ERRINTLVL_OFF_gc | TC_OVFINTLVL_LO_gc;

while (1)
      {
      // Check if we have received some data
      nbytes=sizeof(rx_buffer); // Number of bytes to receive
      if (usb_getbuf(rx_buffer,&nbytes)==USB_RES_OK)
         {
         // Output the low nibble of the received data byte 0
         // to VPORT0 pins 0..3 and light the corresponding LEDs
         // The LED connected to VPORT0 pin 7 will be toggled by
         // the Timer TCC0 overflow interrupt
         n=VPORT0_OUT & (1<<7);
         n|=rx_buffer[0] & 0x0F;
         VPORT0_OUT=n;
         }
      }
}

// Timer/Counter TCC0 overflow interrupt service routine
// occurs every 1000 ms
interrupt [TCC0_OVF_vect] void timer1_ovf_isr(void)
{
static unsigned short counter=0;

// Toggle the LED connected to VPORT0 pin 7 to show
// that the Timer/Counter TCC0 overflow occurs
VPORT0_OUT^=1<<7;

// Write the counter in the first 2 bytes of the transmitted data
tx_buffer[0]=counter & 0xFF;
tx_buffer[1]=counter >> 8;

// Write to the LEDs 0..3 current light state into byte #3
tx_buffer[2]=VPORT0_OUT & 0x0F;

// Send buffer's content to the host
usb_putbuf(tx_buffer,sizeof(tx_buffer));

// Increment the counter
++counter;
}

