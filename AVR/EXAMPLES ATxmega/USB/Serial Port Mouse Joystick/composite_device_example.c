/*******************************************************************
Example of an USB Composite Device containing a
CDC Virtual Serial Port, a HID Mouse and Joystick
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
the contents of the \Exe\Serial Port Mouse Joystick.hex file.

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

Interfaces 0 and 1 are used by the CDC Virtual Serial Port.
Interface 2 is used by the HID Mouse.
Interface 3 is used by the HID Joystick.

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

The mouse cursor will be moved every 5 seconds on a triangle shape,
using the TCC0 Timer/Counter overflow interrupt.

Status messages will be sent to the CDC Virtual Serial Port 0.

The CDC Virtual Serial Port uses the Windows serial driver.
Please follow the installation procedures below:

***** Windows 8 and 8.1 *****
- Invoke the Charms bar and click on "Settings"
- Click on "Change PC Settings" (bottom right of the screen)
- In "PC Settings" click on "Update & recovery" (last option
  in the left column)
- In "Update & recovery" select "Recovery"
- Select in "Advanced startup" the "Restart Now" option
- In the displayed list of options select "Troubleshoot"
- In "Troubleshoot" select "Advanced Options"
- In "Advanced Options" select "Startup Settings"
- In "Startup Settings" click on the "Restart" button
- The system will restart
- When prompted, press the F7 key, which will disable the driver
  signature enforcement
- Wait for the system to boot completely
- Connect the STK600 development board to the computer's
  USB port, wait a little
- Windows tries to install the driver, but doesn't succeed
- Invoke again the Charms bar and click on "Settings"
- Click on "Change PC Settings" (bottom right of the screen)
- Click on "Control Panel" (bottom left of the screen)
- Click on "Hardware and Sound"
- Under "Device and Printers", click on "Device Manager"
- In the Device Manager window expand the "Other Devices" node
- Right click on the "CodeVisionAVR CDC Virtual Serial Port"
  node of "Other Devices"
- Select "Update Driver Software..."
- Select "Browse my computer for driver software"
- Browse to the directory where the file
  "CVAVR Composite Device.inf" is located:
  "\cvavr\Examples ATxmega\USB\Serial Port Mouse Keyboard"
- Click on the "Next" button
- Windows starts the driver installation, but displays
  a warning window "Windows can't verify the publisher of
  this driver software"
- Select "Install this driver software anyway"
- Windows finally installs the driver
- The new CDC Virtual Serial Port will be now listed in the
  Device Manager window under "Ports (COM & LPT)" as
  "CodeVisionAVR CDC Virtual Serial Port (COMn)"
- Retain the COMn number of the serial port and go to
  the CodeVisionAVR IDE "Settings|Terminal menu" and set
  the Terminal to use this COM port
- In the Terminal settings enable the "Echo Transmitted Characters"
  and disable the "Append On Reception" options
- Start the CodeVisionAVR Terminal using the "Tools|Terminal"
  menu and click on the "Connect" button.

***** Windows Vista, Seven *****
- After connecting the STK600 development board
  to the computer's USB port, Windows tries to install
  the driver, but finally displays:
  "Device driver software was not succesfully installed"
  and "No driver found".
- Click on the Windows "Start" button
- Click on "Control Panel"
- Click on "Hardware and Sound"
- Under "Device and Printers", click on "Device Manager"
- In the "Device Manager" window expand the "Other Devices" node
- Right click on the "CodeVisionAVR CDC Virtual Serial Port"
  node of "Other Devices"
- Select "Update Driver Software..."
- Select "Browse my computer for driver software"
- Browse to the directory where the file
  "CVAVR Composite Device.inf" is located:
  "\cvavr\Examples ATxmega\USB\Serial Port Mouse Keyboard"
- Click on the "Next" button
- Windows starts the driver installation, but displays
  a warning window "Windows can't verify the publisher of
  this driver software"
- Select "Install this driver software anyway"
- Windows finally installs the driver
- The new CDC Virtual Serial Port will be now listed in the
  Device Manager window under Ports (COM & LPT) as
  "CodeVisionAVR CDC Virtual Serial Port (COMn)"
- Retain the COMn number of the serial port and go to
  the CodeVisionAVR IDE "Settings|Terminal" menu and set
  the Terminal to use this COM port
- In the Terminal settings enable the "Echo Transmitted Characters"
  and disable the "Append On Reception" options
- Start the CodeVisionAVR Terminal using the "Tools|Terminal"
  menu and click on the "Connect" button.

***** Windows XP *****
- After connecting the STK600 development board
  to the computer's USB port, the "Found New Hardware Wizard"
  window will open
- Select "Install from a List or Specific Location (Advanced)"
- Click on the "Next" button
- Select the "Search for the best driver in these locations"
  and "Include this location in search" options
- Browse to the directory where the file
  "CVAVR Composite Device.inf" is located:
  "\cvavr\Examples ATxmega\USB\Serial Port Mouse Keyboard"
- Click on the "Next" button
- Windows starts the driver installation, but displays
  a warning window with the message:
  "CodeVisionAVR CDC Virtual Serial Port has not passed Windows
  Logo testing to verify its compatibility with Windows XP"
- Click on the "Continue Anyway" button
- Windows finally installs the driver
- The new CDC Virtual Serial Port will be now listed in the
  Device Manager window under Ports (COM & LPT) as
  "CodeVisionAVR CDC Virtual Serial Port (COMn)"
- Retain the COMn number of the serial port and go to
  the CodeVisionAVR IDE "Settings|Terminal" menu and set
  the Terminal to use this COM port
- In the Terminal settings enable the "Echo Transmitted Characters"
  and disable the "Append On Reception" options
- Start the CodeVisionAVR Terminal using the "Tools|Terminal"
  menu and click on the Connect button.
*******************************************************************/

#include <io.h>
#include <delay.h>
// USB Device functions
#include <usb_device.h>
// USB Human Interface Device functions
#include <usb_hid.h>
// USB CDC Virtual Serial Port functions
#include <usb_cdc.h>
// Standard Input/Output functions
#include <stdio.h>
// USB initialization
#include "usb_init.h"
// Chip peripherals initialization
#include "peripherals_init.h"

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

// Use the USB Virtual Serial Port 0 as printf output
#define _ALTERNATE_PUTCHAR_
void putchar(char c)
{
usb_serial_putchar(c);
}

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

// Initialize the USB Controller in Device mode
// Note: Virtual Serial Port 0 is selected by default for communication
usb_init_device(&usb_config);

// Timer/Counter TCC0 initialization
// Set the timer overflow interrupt for a 5000 ms period
tcc0_init();

// Global enable interrupts
#asm("sei")

// Wait for the USB device to be enumerated by the host
while (!usb_enumerated);

// Wait 1.5 seconds for the operating system to
// load any drivers needed by the USB device
delay_ms(1500);

// Light LED 0 to signal that the device was enumerated
LED0=1;

// Wait for the USB host to be ready to receive serial data
// on the CDC Virtual Serial Port 0 by setting its RS-232
// DTR signal high
while (usb_cdc_serial[0].control.dtr==0);

// Light LED 1 to signal that the CDC Virtual
// Serial Port 0 is ready to receive data
LED1=1;

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
         // Set LED 3 on
         LED3=1;
         // Display status message on the Virtual Serial Port 0
         printf("Joystick moved right\r\n");
         }
      else
      if (j & JOYSTICK_LEFT)
         {
         // Move left
         if (new_joystick.x>-100) new_joystick.x--;
         // Set LED 3 on
         LED3=1;
         // Display status message on the Virtual Serial Port 0
         printf("Joystick moved left\r\n");
         }
      if (j & JOYSTICK_UP)
         {
         // Move up
         if (new_joystick.y<100) new_joystick.y++;
         // Set LED 4 on
         LED4=1;
         // Display status message on the Virtual Serial Port 0
         printf("Joystick moved up\r\n");
         }
      else
      if (j & JOYSTICK_DOWN)
         {
         // Move down
         if (new_joystick.y>-100) new_joystick.y--;
         // Set LED 4 on
         LED4=1;
         // Display status message on the Virtual Serial Port 0
         printf("Joystick moved down\r\n");
         }
      if (j & JOYSTICK_Z_UP)
         {
         // Move up on Z axis
         if (new_joystick.z<100) new_joystick.z++;
         // Set LED 5 on
         LED5=1;
         // Display status message on the Virtual Serial Port 0
         printf("Joystick moved up on Z axis\r\n");
         }
      if (j & JOYSTICK_Z_DOWN)
         {
         // Move down on Z axis
         if (new_joystick.z>-100) new_joystick.z--;
         // Set LED 5 on
         LED5=1;
         // Display status message on the Virtual Serial Port 0
         printf("Joystick moved down on Z axys\r\n");
         }
      if (j & JOYSTICK_BTN1)
         {
         // Press button 1
         new_joystick.buttons=USB_JOYSTICK_BTN1;
         // Set LED 6 on
         LED6=1;
         // Display status message on the Virtual Serial Port 0
         printf("Joystick button 1 pressed\r\n");
         }
      if (j & JOYSTICK_BTN2)
         {
         // Press button 2
         new_joystick.buttons=new_joystick.buttons | USB_JOYSTICK_BTN2;
         // Set LED 7 on
         LED7=1;
         // Display status message on the Virtual Serial Port 0
         printf("Joystick button 2 pressed\r\n");
         }

      // If new_joystick state changed, send the
      // new_joystick movement data to the host
      if (memcmp(&usb_joystick,&new_joystick,sizeof(USB_JOYSTICK_t)))
         usb_joystick_move(&new_joystick);
      // If there's no joystick movement, all josytick signaling LEDs 3..7 must be off
      if (j==0) VPORT0_OUT&= ~0b11111000;
      else
         // Ensure a delay when incrementing/decrementing joystick axes
         delay_ms(100);
      }
}

// Timer/Counter TCC0 overflow interrupt service routine
// triggered every 5000 ms, moves the mouse cursor on a
// triangular shape
interrupt [TCC0_OVF_vect] void tcc0_overflow_isr(void)
{
static unsigned short count=0;
flash POINT_t *p;
unsigned char i;

// Don't move the mouse if the device was not yet enumerated
if (!usb_enumerated) return;

// Turn the LED 2 on while moving the mouse
LED2=1;
// Display a message on the Virtual Serial Port 0
// The port must be ready to receive data (LED1 on)
if (LED1) printf("Moving mouse, count: %u\r\n",++count);
// Move the mouse cursor in a triangular shape
p=triangle;
for (i=1; i<=POINTS; i++)
    {
    usb_mouse_move(p->x,p->y,0);
    p++;
    // 30 ms delay in moving the mouse cursor
    delay_ms(30);
    }
// Turn LED 2 off
LED2=0;
}
