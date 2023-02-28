/*******************************************************
USB Mouse HID Example
© Copyright 2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

CodeVisionAVR Compiler version 3.11 or later
is required.

The program is designed to be executed on the
Atmel AT90USBKEY development board, using
an AT90USB1287 chip running at 8.000000 MHz.

The AT90USBKEY, programmed with this example's
code, must be connected to an USB port of the PC.

After the HID mouse device is detected,
the mouse cursor will be moved in a triangular
shape on the screen.

The LED5.2, connected to PORTD bit 7 output,
will be lighted during mouse cursor movement.
*******************************************************/

#include <90usb1287.h>

#include <delay.h>

// USB Device functions
#include <usb_device.h>

// USB Human Interface Device functions
#include <usb_hid.h>

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

// Crystal Oscillator division factor: 1
#pragma optsize-
CLKPR=(1<<CLKPCE);
CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif

// USB Controller initialization in device mode
usb_init_device(&usb_config);

// Global enable interrupts
#asm("sei")

// Wait for the USB device to be enumerated by the host
while (!usb_enumerated);

// Wait 1.5 seconds for the operating system to
// load any drivers needed by the USB device
delay_ms(1500);

// Set PORTD bit 7 output to control LED5.2
DDRD=0x80;

while (1)
      {
      // Turn the LED5.2 on while moving the mouse
	  PORTD.7=1;
      // Move the mouse cursor in a triangular shape
	  p=triangle;
	  for (i=1; i<=POINTS; i++)
          {
		  usb_mouse_move(p->x,p->y,0);
          p++;
          // 30 ms delay in moving the mouse cursor
		  delay_ms(30);
		  }
      // Turn LED 5.2 off
	  PORTD.7=0;
      // Wait 5 seconds between moves
	  delay_ms(5000);
      }
}



