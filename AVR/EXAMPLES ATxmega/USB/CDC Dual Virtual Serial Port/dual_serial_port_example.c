/*******************************************************************
USB CDC Dual Virtual Serial Port Example
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
the contents of the \Exe\Dual Serial Port Example.hex file.

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

The CDC Dual Virtual Serial Port uses the Windows serial driver.
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
  "CVAVR CDC Dual Virtual Serial Port.inf" is located:
  "\cvavr\Examples ATxmega\USB\CDC Dual Virtual Serial Port"
- Click on the "Next" button
- Windows starts the driver installation, but displays
  a warning window "Windows can't verify the publisher of
  this driver software"
- Select "Install this driver software anyway"
- Windows finally installs the driver
- The two new CDC Virtual Serial Ports will be now listed in the
  Device Manager window under "Ports (COM & LPT)" as
  "CodeVisionAVR CDC Dual Virtual Serial Port (COMn)"
- You need to identify which COM port number was allocated by the
  operating system for Virtual Serial Port 0 and 1
- Right click on each "CodeVisionAVR CDC Dual Virtual Serial Port"
  node and select Properties|Details|Device Instance Id
- The COM port associated with Virtual Serial Port 0 will
  have the Id: "USB\VID_03EB&PID_311&MI_01\..."
- The COM port associated with Virtual Serial Port 1 will
  have the Id: "USB\VID_03EB&PID_311&MI_02\..."
- Retain the COMn numbers of the serial ports and go to
  the CodeVisionAVR IDE "Settings|Terminal" menu and set
  the Terminal to use the COM port associated with
  Virtual Serial Port 0
- In the Terminal settings enable the "Echo Transmitted Characters"
  and disable the "Append On Reception" options
- Start the CodeVisionAVR Terminal using the "Tools|Terminal"
  menu and click on the Connect button
- Download and install the freeware PuTTY terminal program from:
  http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html
- Launch PuTTY and in its "Configuration" window go to the
  "Session|Specify the destination you want to connect to" option
- Select "Connection Type: Serial" and at "Serial line" specify
  the COM port associated with Virtual Serial Port 1
- Clik on the Open button
- Click inside the CodeVisionAVR's Terminal window and press any key
- The pressed key will be displayed in the Terminal window
  along with the text "Received char with ASCII code: 0x.."
- The received characters count will be displayed in PuTTY.

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
  "CVAVR CDC Dual Virtual Serial Port.inf" is located:
  "\cvavr\Examples ATxmega\USB\CDC Dual Virtual Serial Port"
- Click on the "Next" button
- Windows starts the driver installation, but displays
  a warning window "Windows can't verify the publisher of
  this driver software"
- Select "Install this driver software anyway"
- Windows finally installs the driver
- The two new CDC Virtual Serial Ports will be now listed in the
  Device Manager window under Ports (COM & LPT) as
  "CodeVisionAVR CDC Dual Virtual Serial Port (COMn)"
- You need to identify which COM port number was allocated by the
  operating system for Virtual Serial Port 0 and 1
- Right click on each "CodeVisionAVR CDC Dual Virtual Serial Port"
  node and select Properties|Details|Device Instance Id
- The COM port associated with Virtual Serial Port 0 will
  have the Id: "USB\VID_03EB&PID_311&MI_01\..."
- The COM port associated with Virtual Serial Port 1 will
  have the Id: "USB\VID_03EB&PID_311&MI_02\..."
- Retain the COMn numbers of the serial ports and go to
  the CodeVisionAVR IDE "Settings|Terminal" menu and set
  the Terminal to use the COM port associated with
  Virtual Serial Port 0
- In the Terminal settings enable the "Echo Transmitted Characters"
  and disable the "Append On Reception" options
- Start the CodeVisionAVR Terminal using the "Tools|Terminal"
  menu and click on the Connect button
- Download and install the freeware PuTTY terminal program from:
  http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html
- Launch PuTTY and in its "Configuration" window go to the
  "Session|Specify the destination you want to connect to" option
- Select "Connection Type: Serial" and at "Serial line" specify
  the COM port associated with Virtual Serial Port 1
- Clik on the Open button
- Click inside the CodeVisionAVR's Terminal window and press any key
- The pressed key will be displayed in the Terminal window
  along with the text "Received char with ASCII code: 0x.."
- The received characters count will be displayed in PuTTY.

***** Windows XP *****
- After connecting the STK600 development board
  to the computer's USB port, the "Found New Hardware Wizard"
  window will open
- Select "Install from a List or Specific Location (Advanced)"
- Click on the "Next" button
- Select the "Search for the best driver in these locations"
  and "Include this location in search" options
- Browse to the directory where the file
  "CVAVR CDC Dual Virtual Serial Port.inf" is located:
  "\cvavr\Examples ATxmega\USB\CDC Dual Virtual Serial Port"
- Click on the "Next" button
- Windows starts the driver installation, but displays
  a warning window with the message:
  "CodeVisionAVR CDC Virtual Serial Port has not passed Windows
  Logo testing to verify its compatibility with Windows XP"
- Click on the "Continue Anyway" button
- Windows finally installs the driver
- The two new CDC Virtual Serial Ports will be now listed in the
  Device Manager window under Ports (COM & LPT) as
  "CodeVisionAVR CDC Dual Virtual Serial Port (COMn)"
- You need to identify which COM port number was allocated by the
  operating system for Virtual Serial Port 0 and 1
- Right click on each "CodeVisionAVR CDC Dual Virtual Serial Port"
  node and select Properties|Details|Device Instance Id
- The COM port associated with Virtual Serial Port 0 will
  have the Id: "USB\VID_03EB&PID_311&MI_01\..."
- The COM port associated with Virtual Serial Port 1 will
  have the Id: "USB\VID_03EB&PID_311&MI_02\..."
- Retain the COMn numbers of the serial ports and go to
  the CodeVisionAVR IDE "Settings|Terminal" menu and set
  the Terminal to use the COM port associated with
  Virtual Serial Port 0
- In the Terminal settings enable the "Echo Transmitted Characters"
  and disable the "Append On Reception" options
- Start the CodeVisionAVR Terminal using the "Tools|Terminal"
  menu and click on the Connect button
- Launch the Windows XP HyperTerminal program by pressing
  Start|Programs|Accessories|Communications|HyperTerminal
- Create a new connection in HyperTerminal
- In the "Connect To" window's "Connect using:" list box
  select the COM port associated with Virtual Serial Port 1
  and press OK
- Click inside the CodeVisionAVR's Terminal window and press any key
- The pressed key will be displayed in the Terminal window
  along with the text "Received char with ASCII code: 0x.."
- The received characters count will be displayed in HyperTerminal.
*******************************************************************/

#include <io.h>
#include <delay.h>
// USB Device functions
#include <usb_device.h>
// USB CDC Virtual Serial Port functions
#include <usb_cdc.h>
// Standard Input/Output functions
#include <stdio.h>
// USB initialization
#include "usb_init.h"
// Chip peripherals initialization
#include "peripherals_init.h"

// LEDs
#define LED0 VPORT0_OUT.0
#define LED1 VPORT0_OUT.1
#define LED2 VPORT0_OUT.2
#define LED3 VPORT0_OUT.3
#define LED4 VPORT0_OUT.4
#define LED5 VPORT0_OUT.5

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
         // Set error LED 5 off
         LED5=0;
         // Received OK, return the char from LSB
         return ((unsigned char *) &c)[0];
         }
      // Display the error code from MSB
      printf("\r\nReceive error, code: 0x%02X\r\n",((unsigned char *) &c)[1]);
      // Light LED 5 to signal the error
      LED5=1;
      }
}

void main(void)
{
char k; // received character
unsigned short rx_count; // # of received characters

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

// USB Controller initialization

// Use the internal 32 MHz RC oscillator, adjusted and
// calibrated by the the DFLL and USB Start of Frame
// to 48 MHz, as USB clock source
// USB clock prescaler division factor: 1
CLK.USBCTRL=CLK_USBSRC_RC32M_gc | CLK_USBSEN_bm | CLK_USBPSDIV_1_gc;

// Initialize the USB Controller in Device mode
// By default the usb_putchar output will be directed
// to the Virtual Serial Port 0
usb_init_device(&usb_config);

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

// Wait for the USB host to be ready to receive serial data
// on the CDC Virtual Serial Port 1 by setting its RS-232
// DTR signal high
while (usb_cdc_serial[1].control.dtr==0);

// Light LED 2 to signal that the CDC Virtual
// Serial Port 1 is ready to receive data
LED2=1;

printf("CodeVisionAVR USB Dual Virtual Serial Port demo\r\n"
       "Type some characters...\r\n");

rx_count=0; // no characters received yet

while (1)
      {
      // Receive a character from the Virtual Serial Port 0
      usb_serial_select(0);
      k=getchar();
      // Display the ASCII code of the received char
      // in a message sent to Virtual Serial Port 0
      printf("\r\nReceived char with ASCII code: 0x%02X\r\n",k);

      // Toggle LED 3 to signal communication on Virtual Serial Port 0
      LED3^=1;

      // Display the received characters count
      // in a message sent to Virtual Serial Port 1
      usb_serial_select(1);
      printf("%u received characters\r\n",++rx_count);

      // Toggle LED 4 to signal communication on Virtual Serial Port 1
      LED4^=1;
      }
}
