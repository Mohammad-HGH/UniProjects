/*******************************************************
USB HID Joystick Example
© Copyright 2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

CodeVisionAVR Compiler version 3.11 or later
is required.

The program is designed to be executed on the
Atmel AT90USBKEY development board, using
an AT90USB1287 chip running at 8.000000 MHz.

In order to use this example, the
'Joystick Test.exe' program must be
launched on the PC.

The AT90USBKEY, programmed with this example's
code, must be connected to an USB port of the PC.

The 'Joystick Test' program will display the
AT90USBKEY joystick movements.
*******************************************************/

#include <90usb1287.h>
#include <delay.h>
#include <string.h>

// USB Device functions
#include <usb_device.h>

// USB Human Interface Device functions
#include <usb_hid.h>

// USB initialization
#include "usb_joystick_init.h"

// Joystick input signals masks
#define JOYSTICK_RIGHT (1<<3)
#define JOYSTICK_DOWN (1<<4)
#define JOYSTICK_SELECT (1<<5)
#define JOYSTICK_LEFT (1<<6)
#define JOYSTICK_UP (1<<7)

// LEDs
#define LED2_1 PORTD.4
#define LED2_2 PORTD.5
#define LED5_1 PORTD.6
#define LED5_2 PORTD.7

void main(void)
{
signed char i; // z axys increment/decrement
unsigned char j; // Joystick inputs
USB_JOYSTICK_t new_joystick; // New new_joystick movement and button state

// Crystal Oscillator division factor: 1
#pragma optsize-
CLKPR=(1<<CLKPCE);
CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif

// Initialize the I/O ports
// LED outputs
DDRD=0b11110000;
// All LEDs are off
PORTD=0;
// Configure new_joystick port inputs
DDRB=0;
DDRE=0;
// Enable pull-ups on new_joystick port inputs
PORTB=0b11100000;
PORTE=0b00110000;

// USB Controller initialization in device mode
usb_init_device(&usb_config);

// Global enable interrupts
#asm("sei")

// Wait for the USB device to be enumerated by the host
while (!usb_enumerated);

// Wait 1.5 seconds for the operating system to
// load any drivers needed by the USB device
delay_ms(1500);

// Initially there's no joystick movement and no buttons are pressed
usb_joystick.x=0;
usb_joystick.y=0;
usb_joystick.z=0;
usb_joystick.buttons=0;

// Initially increment the z axis
i=1;

while (1)
      {
      // Wait for the joystick to be moved
      // Combine the new_joystick switch inputs in a single variable
      // and invert input bits:
      // bit 3=1 - new_joystick right
      // bit 4=1 - new_joystick down
      // bit 5=1 - new_joystick select
      // bit 6=1 - new_joystick left
      // bit 7=1 - new_joystick up
      j=((PINB & 0xE0) | ((PINE>>1) & 0x18)) ^ 0xF8;
      // Consider that no joystick buttons are pressed yet
      new_joystick.buttons=0;
      // Set new new_joystick state
      if (j & JOYSTICK_RIGHT)
         {
         // Move right
         if (new_joystick.x<100) new_joystick.x++;
         // Also press button 1
         new_joystick.buttons=USB_JOYSTICK_BTN1;
         // Set LED 2.1 on
         LED2_1=1;
         }
      else
      if (j & JOYSTICK_LEFT)
         {
         // Move left
         if (new_joystick.x>-100) new_joystick.x--;
         // Set LED 2.2 on
         LED2_2=1;
         }
      if (j & JOYSTICK_UP)
         {
         // Move up
         if (new_joystick.y<100) new_joystick.y++;
         // Also press button 2
         new_joystick.buttons=new_joystick.buttons | USB_JOYSTICK_BTN2;
         // Set LED 5.1 on
         LED5_1=1;
         }
      else
      if (j & JOYSTICK_DOWN)
         {
         // Move down
         if (new_joystick.y>-100) new_joystick.y--;
         // Set LED 5.2 on
         LED5_2=1;
         }
      if (j & JOYSTICK_SELECT)
         {
         // Move up on Z axis
         if (new_joystick.z>=100) i=-1; // When z max. reached start decrementing
         else
         if (new_joystick.z<=-100) i=1; // When z min. reached start incrementing
         // Increment/decrement z value
         new_joystick.z+=i;
         // Also press both buttons
         new_joystick.buttons=USB_JOYSTICK_BTN1 | USB_JOYSTICK_BTN2;
         // Set all LEDs on
         PORTD=0xF0;
         }
      // If new_joystick state changed, send the
      // new_joystick movement data to the host
      if (memcmp(&usb_joystick,&new_joystick,sizeof(USB_JOYSTICK_t)))
         usb_joystick_move(&new_joystick);
      // If there's no joystick movement, all LEDs must be off
      if (j==0) PORTD=0;
      // Ensure a delay when incrementing/decrementing joystick axes
      else delay_ms(100);
      }
}
