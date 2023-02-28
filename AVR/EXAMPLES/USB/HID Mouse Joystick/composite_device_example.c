/*******************************************************
Example of an USB Composite Device containing a
HID Mouse and Joystick
® Copyright 2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

CodeVisionAVR Compiler version 3.12 or later
is required.

The program is designed to be executed on the
Atmel AT90USBKEY development board, using
an AT90USB1287 chip running at 8.000000 MHz.

Interface 0 is used for the Mouse
and Interface 1 for the Joystick.

In order to use this example, the
'Joystick Test.exe' program must be
launched on the PC.

The AT90USBKEY, programmed with this example's
code, must be connected to an USB port of the PC.

The 'Joystick Test' program will display the
AT90USBKEY joystick movements.

The mouse cursor will be moved every 5 seconds in
a triangular shape shape, using the Timer 1
overflow interrupt.

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
#include "usb_init.h"

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

// Initialize the USB Controller in Device mode
usb_init_device(&usb_config);

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 7.813 kHz
// Mode: Normal top=0xFFFF
// Timer Period: 5.0 s
TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<COM1C1) | (0<<COM1C0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (1<<CS12) | (0<<CS11) | (1<<CS10);
TCNT1H=0x676A >> 8;
TCNT1L=0x676A & 0xff;
// Timer1 Overflow Interrupt: On
TIMSK1=(0<<ICIE1) | (0<<OCIE1C) | (0<<OCIE1B) | (0<<OCIE1A) | (1<<TOIE1);

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
         // Set LED 5.1 on
         LED5_1=1;
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
         // Turn on LEDs 2.1, 2.2, 5.1
         PORTD|=0b01110000;
         }
      // If new_joystick state changed, send the
      // new_joystick movement data to the host
      if (memcmp(&usb_joystick,&new_joystick,sizeof(USB_JOYSTICK_t)))
         usb_joystick_move(&new_joystick);
      // If there's no joystick movement, all LED 2.1, 2.2, 5.1 must be off
      if (j==0) PORTD&= ~0b01110000;
      // Ensure a delay when incrementing/decrementing joystick axes
      else delay_ms(100);
      }
}

// Timer1 overflow interrupt service routine
// triggered every 5000 ms, moves the mouse cursor on a
// triangular shape
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
flash POINT_t *p;
unsigned char i;

// Reinitialize Timer1 value
TCNT1H=0x676A >> 8;
TCNT1L=0x676A & 0xff;

// Don't move the mouse if the device was not yet enumerated
if (!usb_enumerated) return;

// The USB functions require interrupts to be enabled
#asm("sei")

// Turn the LED 5.2 on while moving the mouse
LED5_2=1;
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
LED5_2=0;
}