/*******************************************************
USB HID Joystick Example
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
the contents of the \Exe\HID Joystick.hex file.

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

In order to use this example, the
'Joystick Test.exe' program must be
launched on the PC.

The 'Joystick Test' program will display the joystick
movements and button states, simulated by pressing the
switches SW0..SW7 on the STK600 board:

SW0 - Move joystick right
SW1 - Move joystick left
SW2 - Move joystick up
SW3 - Move joystick down
SW4 - Move joystick Z axis up
SW5 - Move joystick Z axis down
SW6 - Press joystick button 1
SW7 - Press joystick button 2
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
#include "usb_joystick_init.h"

// Joystick input signals masks
#define JOYSTICK_RIGHT (1<<0)  // SW0
#define JOYSTICK_LEFT (1<<1)   // SW1
#define JOYSTICK_UP (1<<2)     // SW2
#define JOYSTICK_DOWN (1<<3)   // SW3
#define JOYSTICK_Z_UP (1<<4)   // SW4
#define JOYSTICK_Z_DOWN (1<<5) // SW5
#define JOYSTICK_BTN1 (1<<6)   // SW6
#define JOYSTICK_BTN2 (1<<7)   // SW7

// LEDs
#define LED0 VPORT0_OUT.0
#define LED1 VPORT0_OUT.1
#define LED2 VPORT0_OUT.2
#define LED3 VPORT0_OUT.3
#define LED4 VPORT0_OUT.4
#define LED5 VPORT0_OUT.5
#define LED6 VPORT0_OUT.6
#define LED7 VPORT0_OUT.7

void main(void)
{
unsigned char j; // Joystick inputs
USB_JOYSTICK_t new_joystick; // New new_joystick movement and button state

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

// Initially there's no joystick movement and no buttons are pressed
usb_joystick.x=0;
usb_joystick.y=0;
usb_joystick.z=0;
usb_joystick.buttons=0;

while (1)
      {
      // Wait for the new_joystick to be moved
      // When a switch is pressed the corresponding
      // VPORT1 input becomes 0
      j= ~VPORT1_IN;
      // Consider that no joystick buttons are pressed yet
      new_joystick.buttons=0;
      // Set new new_joystick state
      if (j & JOYSTICK_RIGHT)
         {
         // Move right
         if (new_joystick.x<100) new_joystick.x++;
         // Set LED 0 on
         LED0=1;
         }
      else
      if (j & JOYSTICK_LEFT)
         {
         // Move left
         if (new_joystick.x>-100) new_joystick.x--;
         // Set LED 1 on
         LED1=1;
         }
      if (j & JOYSTICK_UP)
         {
         // Move up
         if (new_joystick.y<100) new_joystick.y++;
         // Set LED 2 on
         LED2=1;
         }
      else
      if (j & JOYSTICK_DOWN)
         {
         // Move down
         if (new_joystick.y>-100) new_joystick.y--;
         // Set LED 3 on
         LED3=1;
         }
      if (j & JOYSTICK_Z_UP)
         {
         // Move up on Z axis
         if (new_joystick.z<100) new_joystick.z++;
         // Set LED 4 on
         LED4=1;
         }
      if (j & JOYSTICK_Z_DOWN)
         {
         // Move down on Z axis
         if (new_joystick.z>-100) new_joystick.z--;
         // Set LED 5 on
         LED5=1;
         }
      if (j & JOYSTICK_BTN1)
         {
         // Press button 1
         new_joystick.buttons=USB_JOYSTICK_BTN1;
         // Set LED 6 on
         LED6=1;
         }
      if (j & JOYSTICK_BTN2)
         {
         // Press button 2
         new_joystick.buttons=new_joystick.buttons | USB_JOYSTICK_BTN2;
         // Set LED 7 on
         LED7=1;
         }

      // If new_joystick state changed, send the
      // new_joystick movement data to the host
      if (memcmp(&usb_joystick,&new_joystick,sizeof(USB_JOYSTICK_t)))
         usb_joystick_move(&new_joystick);
      // If there's no joystick movement, all LEDs must be off
      if (j==0) VPORT0_OUT=0;
      // Ensure a delay when incrementing/decrementing joystick axes
      else delay_ms(100);
      }
}
