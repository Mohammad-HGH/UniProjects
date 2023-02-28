/*******************************************************************
USB CDC Virtual Serial Port Example
© Copyright 2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

CodeVisionAVR Compiler version 3.11 or later
is required.

The program is designed to be executed on the
Atmel AT90USBKEY development board, using
an AT90USB1287 chip running at 8.000000 MHz.

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
- Connect the AT90USBKEY development board to the computer's
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
  "CVAVR CDC Virtual Serial Port.inf" is located:
  "\cvavr\Examples\USB\CDC Virtual Serial Port"
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
  menu and click on the "Connect" button
- Click inside the Terminal window and press any key
- The pressed key will be displayed in the Terminal window
  along with the text "Received char with ASCII code: 0x.."

***** Windows Vista, Seven *****
- After connecting the AT90USBKEY development board
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
  "CVAVR CDC Virtual Serial Port.inf" is located:
  "\cvavr\Examples\USB\CDC Virtual Serial Port"
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
  menu and click on the "Connect" button
- Click inside the Terminal window and press any key
- The pressed key will be displayed in the Terminal window
  along with the text "Received char with ASCII code: 0x.."

***** Windows XP *****
- After connecting the AT90USBKEY development board
  to the computer's USB port, the "Found New Hardware Wizard"
  window will open
- Select "Install from a List or Specific Location (Advanced)"
- Click on the "Next" button
- Select the "Search for the best driver in these locations"
  and "Include this location in search" options
- Browse to the directory where the file
  "CVAVR CDC Virtual Serial Port.inf" is located:
  "\cvavr\Examples\USB\CDC Virtual Serial Port"
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
  menu and click on the Connect button
- Click inside the Terminal window and press any key
- The pressed key will be displayed in the Terminal window
  along with the text "Received char with ASCII code: 0x.."
*******************************************************************/

#include <90usb1287.h>
#include <delay.h>
// USB Device functions
#include <usb_device.h>
// USB CDC Virtual Serial Port functions
#include <usb_cdc.h>
// Standard Input/Output functions
#include <stdio.h>
// USB initialization
#include "usb_init.h"

// Use the USB Virtual Serial Port as printf output
#define _ALTERNATE_PUTCHAR_
void putchar(char c)
{
usb_serial_putchar(c);
}

// Use the USB Virtual Serial Port for input
#define _ALTERNATE_GETCHAR_
char getchar(void)
{
unsigned short c;
while (1)
      {
      if ((c=usb_serial_getchar())<0x100)
         {
         // Set error LED5.2 (PORTD bit 7) off
         PORTD.7=0;
         // Received OK, return the char from LSB
         return ((unsigned char *) &c)[0];
         }
      // Display the error code from MSB
      printf("\r\nReceive error, code: 0x%02X\r\n",((unsigned char *) &c)[1]);
      // Light LED 5.2 (PORTD bit 7) to signal the error
      PORTD.7=1;
      }
}

void main(void)
{
char k;

// Crystal Oscillator division factor: 1
#pragma optsize-
CLKPR=(1<<CLKPCE);
CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif

// Enable LED output on PORTD bits 4..7
DDRD=0xF0;
// All LEDs are off
PORTD=0;

// USB Controller initialization in device mode
usb_init_device(&usb_config);

// Global enable interrupts
#asm("sei")

// Wait for the USB device to be enumerated by the host
while (!usb_enumerated);

// Wait 1.5 seconds for the operating system to
// load any drivers needed by the USB device
delay_ms(1500);

// Light LED D2.1 to signal that the device was enumerated
PORTD.4=1;

// Wait for the USB host to be ready to receive serial data
// on the CDC Virtual Serial Port by setting its RS-232
// DTR signal high
while (usb_cdc_serial.control.dtr==0);

// Light LED D2.2 to signal that the CDC Virtual
// Serial Port is ready to receive data
PORTD.5=1;

while (1)
      {
      // Receive a char from the USB Virtual Serial Port
      k=getchar();
      // Display the ASCII code of the received char
      printf("\r\nReceived char with ASCII code: 0x%02X\r\n",k);
      // Toggle LED D5.1 (PORTD bit 6)
      PORTD^=1<<6;
      }
}
