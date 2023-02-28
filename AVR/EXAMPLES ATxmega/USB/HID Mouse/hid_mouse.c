/*******************************************************
USB Mouse HID Example
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
the contents of the \Exe\HID Mouse.hex file.

The chip uses the internal 32 MHz RC oscillator,
adjusted and calibrated to 48 MHz using the
DFLL and USB Start Of Frame.
The CPU and Peripheral Clock frequency are set to 12 MHz.

The STK600-TQFP44 socket and STK600-RC044X-55 routing
cards are used for interfacing the chip with
the STK600.

The STK600 with the ATxmega128A4U chip, programmed with this
example's code, must be connected to an USB port of the PC.
The STK600's mini USB connector and an appropriate cable
must be used for this purpose.

After the HID mouse device is detected,
the mouse cursor will be moved in a triangular
shape on the screen.

The LEDs are connected to PORTA pin 0..7 outputs.
For this purpose use a 10 pin ribbon cable to connect
the PORTA and LED headers.

The LED0, connected to PORTA pin 0 output,
will be lighted during mouse cursor movement.
*******************************************************/

#include <io.h>

#include <delay.h>

// USB Device functions
#include <usb_device.h>

// USB Human Interface Device functions
#include <usb_hid.h>

// Chip peripherals initialization
#include "peripherals_init.h"

// USB initialization
#include "usb_init_mouse.h"

// Point's coordinates
typedef struct
{
signed char x,y;
} POINT_t;

// Table with coordinates needed to move
// the mouse in a triangle
flash POINT_t triangle[]=
{
30,0,
30,0,
30,0,
30,0,
30,0,
30,0,
30,0,
30,0,
30,0,
30,0,
-30,-30,
-30,-30,
-30,-30,
-30,-30,
-30,-30,
-30,30,
-30,30,
-30,30,
-30,30,
-30,30
};

// # of points of the triangle
#define POINTS (sizeof(triangle)/sizeof(POINT_t))

void main(void)
{
flash POINT_t *p;
unsigned char i;

// Interrupt system initialization
interrupts_init();

// System Clocks initialization to use the internal 32MHz oscillator
system_clocks_init();

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

// Global enable interrupts
#asm("sei")

// Wait for the USB device to be enumerated by the host
while (!usb_enumerated);

// Wait 1.5 seconds for the operating system to
// load any drivers needed by the USB device
delay_ms(1500);

while (1)
      {
      // Turn the LED 0 on while moving the mouse
	  VPORT0_OUT.0=1;
      // Move the mouse cursor in a triangular shape
	  p=triangle;
	  for (i=1; i<=POINTS; i++)
          {
		  usb_mouse_move(p->x,p->y,0);
          p++;
          // 30 ms delay in moving the mouse cursor
		  delay_ms(30);
		  }
      // Turn LED 0 off
	  VPORT0_OUT.0=0;
      // Wait 5 seconds between moves
	  delay_ms(5000);
      }
}



