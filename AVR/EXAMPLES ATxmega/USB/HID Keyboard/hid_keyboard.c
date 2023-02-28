/*******************************************************
CodeVisionAVR USB HID Keyboard Example
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
the contents of the \Exe\HID Keyboard.hex file.

The chip uses the internal 32 MHz RC oscillator,
adjusted and calibrated to 48 MHz using the
DFLL and USB Start Of Frame.
The CPU and Peripheral Clock frequency are set to 12 MHz.

The STK600-TQFP44 socket and STK600-RC044X-55 routing
cards are used for interfacing the chip with
the STK600.

The LEDs are connected to PORTA pin 0..7 outputs.
For this purpose use a 10 pin ribbon cable to connect
the PORTA and LED headers.

The STK600 switches are connected to PORTC pin 0..7 inputs.
For this purpose use a 10 pin ribbon cable to connect
the PORTC and SWITCHES headers.

The STK600 with the ATxmega128A4U chip, programmed with this
example's code, must be connected to an USB port of the PC.
The STK600's mini USB connector and an appropriate cable
must be used for this purpose.

Open a new file in the CodeVisionAVR editor
and click with the mouse in the editor window,
so that the caret will displayed.

After the HID keyboard is detected by the PC,
pressing the switches SW0..SW7 on the STK600 board will
be translated to key inputs in the editor window:

SW0 -> 'a' or 'A'
SW1 -> 'b' or 'B'
SW2 -> 'c' or 'C'
SW3 -> 'd' or 'D'
SW4 -> 'e' or 'E'
SW5 -> 'f' or 'F'
SW6 -> 'g' or 'G'
SW7 -> toggle Caps Lock on/off.

Board's LEDs 0..4 will display the keyboard status.
*******************************************************/

#include <io.h>

#include <delay.h>
#include <iobits.h>

// USB Device functions
#include <usb_device.h>

// USB Human Interface Device functions
#include <usb_hid.h>

// Chip peripherals initialization
#include "peripherals_init.h"

// USB initialization
#include "usb_init_key.h"

void main(void)
{
unsigned char k; // Current key input state
unsigned char k0; // Previous key input state
unsigned char kcnt; // Key input debounce counter

// Interrupt system initialization
interrupts_init();

// System Clocks initialization to use the internal 32MHz oscillator
system_clocks_init();

// PORTA is mapped to VPORT0
// PORTC is mapped to VPORT1
PORTCFG.VPCTRLA=PORTCFG_VP13MAP_PORTC_gc | PORTCFG_VP02MAP_PORTA_gc;

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

// PORTC inputs are not inverted and have pull-ups enabled
PORTC.PIN0CTRL=PORT_OPC_PULLUP_gc;
PORTC.PIN1CTRL=PORT_OPC_PULLUP_gc;
PORTC.PIN2CTRL=PORT_OPC_PULLUP_gc;
PORTC.PIN3CTRL=PORT_OPC_PULLUP_gc;
PORTC.PIN4CTRL=PORT_OPC_PULLUP_gc;
PORTC.PIN5CTRL=PORT_OPC_PULLUP_gc;
PORTC.PIN6CTRL=PORT_OPC_PULLUP_gc;
PORTC.PIN7CTRL=PORT_OPC_PULLUP_gc;

// Virtual Port VPORT1 pins 0..7 are inputs
VPORT1_DIR=0;

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

// Keys not pressed yet
k0=0;
while (1)
      {
      // Display keyboard status LEDs
      VPORT0_OUT=usb_keyboard_leds;
      // Wait for a key to be pressed
      // When a key is pressed the corresponding
      // VPORT1 input becomes 0
      k= ~VPORT1_IN;
      if (k!=0)
         {
         // Key pressed
         if (k0==0)
            {
            k0=k; // First press, retain new key switches state
            kcnt=100; // Init the key debounce counter
            goto continue_read;
            }
         // Compare with previous switches state
         if ((k==k0) && (kcnt!=0))
            {
            // Ok, same switches state
            if (--kcnt==0)
               {
               // The same key switches state remained for 100 ms,
               // so the key press is valid
               // Send appropriate keyscan codes to the host
               if (k & (1<<0)) // 'a', 'A'
                  usb_keyboard_keypress(KS_A,0);
               if (k & (1<<1)) // 'b', 'B'
                  usb_keyboard_keypress(KS_B,0);
               if (k & (1<<2)) // 'c', 'C'
                  usb_keyboard_keypress(KS_C,0);
               if (k & (1<<3)) // 'd', 'D'
                  usb_keyboard_keypress(KS_D,0);
               if (k & (1<<4)) // 'e', 'E'
                  usb_keyboard_keypress(KS_E,0);
               if (k & (1<<5)) // 'f', 'F'
                  usb_keyboard_keypress(KS_F,0);
               if (k & (1<<6)) // 'g', 'G'
                  usb_keyboard_keypress(KS_G,0);
               if (k & (1<<7)) // Toggle CAPS LOCK On/Off
                  usb_keyboard_keypress(KS_CAPS_LOCK,0);
               // Ensure 200 ms pause between key presses
               delay_ms(200);

               // Prepare to read new key switches state
               goto new_read;
               }
            continue_read:
            // Contact noise debouncing time not elapsed yet
            delay_ms(1); // Wait 1 ms before reading new switches state
            continue;
            }
         }
      new_read:
      // Prepare to read key switches state
      k0=0;
      }
}
