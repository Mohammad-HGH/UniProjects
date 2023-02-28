/*******************************************************
CodeVisionAVR USB HID Keyboard Example
© Copyright 2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

CodeVisionAVR Compiler version 3.11 or later
is required.

The program is designed to be executed on the
Atmel AT90USBKEY development board, using
an AT90USB1287 chip running at 8.000000 MHz.

The AT90USBKEY, programmed with this example's
code, must be connected to an USB port of the PC.

Open a new file in the CodeVisionAVR editor
and click with the mouse in the editor window,
so that the caret will displayed.

After the HID keyboard is detected by the PC,
joystick movements on the AT90USBKEY board will be
translated to key inputs in the editor window:

left   -> 'l' or 'L' key
right  -> 'r' or 'R' key
up     -> 'u' or 'U' key
down   -> 'd' or 'D' key
select -> toggle Caps Lock on/off.

Board's LEDs will display keyboard status.
*******************************************************/

#include <90usb1287.h>

#include <delay.h>
#include <iobits.h>

// USB Device functions
#include <usb_device.h>

// USB Human Interface Device functions
#include <usb_hid.h>

// USB initialization
#include "usb_init_key.h"

// Joystick input signals masks
#define JOYSTICK_RIGHT (1<<3)
#define JOYSTICK_DOWN (1<<4)
#define JOYSTICK_SELECT (1<<5)
#define JOYSTICK_LEFT (1<<6)
#define JOYSTICK_UP (1<<7)

void main(void)
{
unsigned char j; // Current joystick input state
unsigned char j0; // Previous joystick input state
unsigned char jcnt; // Joystick input debounce counter

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
// Configure joystick port inputs
DDRB=0;
DDRE=0;
// Enable pull-ups on joystick port inputs
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

// Joystick not moved yet
j0=0;
while (1)
      {
      // Display keyboard status LEDs
      PORTD=usb_keyboard_leds<<4;
      // Wait for the joystick to be moved
      // Combine the joystick switch inputs in a single variable
      // and invert input bits:
      // bit 3=1 - joystick right - output key scan codes 'r', 'R'
      // bit 4=1 - joystick down - output key scan codes 'd', 'D'
      // bit 5=1 - joystick select - toggle CAPS LOCK On/Off
      // bit 6=1 - joystick left - output key scan codes 'l', 'L'
      // bit 7=1 - joystick up - output key scan codes 'u', 'U'
      j=((PINB & 0xE0) | ((PINE>>1) & 0x18)) ^ 0xF8;
      if (j!=0)
         {
         // Joystick moved
         if (j0==0)
            {
            j0=j; // First move, retain new joystick switches state
            jcnt=100; // Init the joystick key debounce counter
            goto continue_read;
            }
         // Compare with previous switches state
         if ((j==j0) && (jcnt!=0))
            {
            // Ok, same switches state
            if (--jcnt==0)
               {
               // The same joystick switches state remained for 100 ms,
               // so the move is valid
               // Send appropriate keyscan codes to the host
               if (j & JOYSTICK_RIGHT) // 'r', 'R'
                  usb_keyboard_keypress(KS_R,0);
               if (j & JOYSTICK_LEFT) // 'l', 'L'
                  usb_keyboard_keypress(KS_L,0);
               if (j & JOYSTICK_UP) // 'u', 'U'
                  usb_keyboard_keypress(KS_U,0);
               if (j & JOYSTICK_DOWN) // 'd', 'D'
                  usb_keyboard_keypress(KS_D,0);
               if (j & JOYSTICK_SELECT) // Toggle CAPS LOCK On/Off
                  usb_keyboard_keypress(KS_CAPS_LOCK,0);
               // Ensure 200 ms pause between key presses
               delay_ms(200);
               // Prepare to read new joystick switches state
               goto new_read;
               }
            continue_read:
            // Contact noise debouncing time not elapsed yet
            delay_ms(1); // Wait 1 ms before reading new switches state
            continue;
            }
         }
      new_read:
      // Prepare to read joystick switches state
      j0=0;
      }
}
