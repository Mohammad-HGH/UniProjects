/*******************************************************************************
Solomon Systech SSD1289 Color TFT LCD and
ADS7846 Resistive Touchscreen controller demo
(C) Copyright 2011-2013 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

This program can't be compiled with the Evaluation or
Standard versions of CodeVisionAVR, because the glcd_ssd1289.lib
file is supplied only with the Advanced license.
In this case you can program the ATxmega128A1 FLASH with
the contents of the
\EXAMPLES\Graphic LCD\Resistive Touchscreen\SSD1289\Release\Exe\demo_rtouch.hex
file.

The demo is designed to run on the STK600
development board with an ATxmega128A1
chip using the internal 32 MHz oscillator
as clock source.

A 240x320 HY32D 3.2" TFT LCD module is used for
the purpose of this demo.
It can be replaced with other similar modules with
the SSD1289 controller. 

The TFT LCD module connections are:
DB0  - PORTE bit 0
DB1  - PORTE bit 1
DB2  - PORTE bit 2
DB3  - PORTE bit 3
DB4  - PORTE bit 4
DB5  - PORTE bit 5
DB6  - PORTE bit 6
DB7  - PORTE bit 7

DB8  - PORTC bit 0
DB9  - PORTC bit 1
DB10 - PORTC bit 2
DB11 - PORTC bit 3
DB12 - PORTC bit 4
DB13 - PORTC bit 5
DB14 - PORTC bit 6
DB15 - PORTC bit 7

/WR  - PORTD bit 3
/RD  - PORTD bit 2
D/C  - PORTD bit 1
/RST - PORTD bit 4
/CS  - PORTD bit 0

Backlight connections:
BLGND - GND
BLVDD - +3.3V

Power supply connections:
GND - GND
+5V - +5V

ADS7846 touch screen controller connections:
/CS     - PORTF bit 0
DCLK    - PORTF bit 1
DIN     - PORTF bit 2
DOUT    - PORTF bit 3
/PENIRQ - PORTF bit 4

If you are using a different LCD module,
please use the connections from the module datasheet.

The connections can be modified by accessing the
Project|Configure|C Compiler|Libraries|Graphic Display and
Project|Configure|C Compiler|Libraries|Resistive Touchscreen
menus.
*******************************************************************************/

#include <io.h>
#include <glcd.h>
#include <delay.h>
#include <stdio.h>
/* Resistive touchscreen functions */
#include <rtouch.h>

/* Include the font needed by the demo */
#include <font5x7.h>

#include "init.h"

/* Resistive touchscreen calibration helper functions */
#include "calibrate.h"

/* Timer/counter TCC0 Overflow/Underflow interrupt service routine called every 10ms */
interrupt [TCC0_OVF_vect] void tcc0_overflow_isr(void)
{
/* Called to detect the ADS7846 controller's
   /PENIRQ signal becoming 0 when the screen was touched */
rt_timerproc();
}

/* Display buffer */
char display_buf[80];

void demo_message(void)
{
glcd_outtextxyf(0,0,"ADS7846 Resistive Touch Controller Demo");
}

flash RTPOINT_t test_points[]={{20,30}, {100,100}, {200,200}};

void main()
{
GLCDINIT_t glcd_init_data;
unsigned char n;
unsigned short x,y;
bit result;

/* Set the ATxmega128A1 to run from the internal 32MHz oscillator */
system_clocks_init();

/* Initialize timer TCC0 */
tcc0_init();

/* Initialize the LCD controller with the default values
   from glcd_ssd1289.h */

/* Specify the current font for displaying text */
glcd_init_data.font=font5x7;
/* No function is used for reading image data from external memory */
glcd_init_data.readxmem=NULL;
/* No function is used for writing image data to external memory */
glcd_init_data.writexmem=NULL;
/* Horizontal reverse for HY32D */
glcd_init_data.reverse_x=SSD1289_REVX_REV;
/* Normal display, no vertical reverse */
glcd_init_data.reverse_y=SSD1289_REVY_NORM;
/* Color bit writing order: BGR for HY32D ! */
glcd_init_data.cl_bits_order=SSD1289_CL_BITS_BGR;
/* Power control 1 BT0..BT2 step-up factor of the step-up circuit */
glcd_init_data.stepup_factor=SSD1289_DEFAULT_STEPUP_FACTOR;
/* Power control 1 DC0..DC3, DCT0..DCT3 step-up circuit cycle */
glcd_init_data.stepup_cycle=SSD1289_DEFAULT_STEPUP_CYCLE;
/* Power control 1 AP0..AP2 adjusts the amount of current
   from the constant current source in the
   internal op. amplifier circuit */
glcd_init_data.crt_source=SSD1289_DEFAULT_CRT_SOURCE;
/* Default value for VCIX2 voltage */
glcd_init_data.vcix2=SSD1289_DEFAULT_VCIX2;
/* Default value for VLCD63 voltage */
glcd_init_data.vlcd63=SSD1289_DEFAULT_VLCD63;
// Default value for VcomL alternating drive voltage
glcd_init_data.vcoml=SSD1289_DEFAULT_VCOML;
/* Default value for VcomH voltage */
glcd_init_data.vcomh=SSD1289_DEFAULT_VCOMH;
/* Frame frequency */
glcd_init_data.frame_freq=SSD1289_DEFAULT_FRAME_FREQ;
/* PKP00..PKP02 positive gamma micro adj. */
glcd_init_data.pkp00=SSD1289_DEFAULT_PKP00;
/* PKP10..PKP12 positive gamma micro adj. */
glcd_init_data.pkp10=SSD1289_DEFAULT_PKP10;
/* PKP20..PKP22 positive gamma micro adj. */
glcd_init_data.pkp20=SSD1289_DEFAULT_PKP20;
/* PKP30..PKP32 positive gamma micro adj. */
glcd_init_data.pkp30=SSD1289_DEFAULT_PKP30;
/* PKP40..PKP42 positive gamma micro adj. */
glcd_init_data.pkp40=SSD1289_DEFAULT_PKP40;
/* PKP50..PKP52 positive gamma micro adj. */
glcd_init_data.pkp50=SSD1289_DEFAULT_PKP50;
/* PRP00..PRP02 positive gamma gradient adj. */
glcd_init_data.prp00=SSD1289_DEFAULT_PRP00;
/* PRP10..PRP12 positive gamma gradient adj. */
glcd_init_data.prp10=SSD1289_DEFAULT_PRP10;
/* VRP00..VRP03 positive gamma amplification adj. */
glcd_init_data.vrp00=SSD1289_DEFAULT_VRP00;
/* VRP10..VRP14 positive gamma amplification adj. */
glcd_init_data.vrp10=SSD1289_DEFAULT_VRP10;
/* PKN00..PKN02 negative gamma micro adj. */
glcd_init_data.pkn00=SSD1289_DEFAULT_PKN00;
/* PKN10..PKN12 negative gamma micro adj. */
glcd_init_data.pkn10=SSD1289_DEFAULT_PKN10;
/* PKN20..PKN22 positive gamma micro adj. */
glcd_init_data.pkn20=SSD1289_DEFAULT_PKN20;
/* PKN30..PKN32 positive gamma micro adj. */
glcd_init_data.pkn30=SSD1289_DEFAULT_PKN30;
/* PKN40..PKN42 negative gamma micro adj. */
glcd_init_data.pkn40=SSD1289_DEFAULT_PKN40;
/* PKN50..PKN52 negative gamma micro adj. */
glcd_init_data.pkn50=SSD1289_DEFAULT_PKN50;
/* PRN00..PRN02 negative gamma gradient adj. */
glcd_init_data.prn00=SSD1289_DEFAULT_PRN00;
/* PRN10..PRN12 negative gamma gradient adj. */
glcd_init_data.prn10=SSD1289_DEFAULT_PRN10;
/* VRN00..VRN03 negative gamma amplification adj. */
glcd_init_data.vrn00=SSD1289_DEFAULT_VRN00;
/* VRN10..VRN14 negative gamma amplification adj. */
glcd_init_data.vrn10=SSD1289_DEFAULT_VRN10;

glcd_init(&glcd_init_data);    

demo_message();

/* Interrupt system initialization in order to be able to sense /PENIRQ
   generated by the ADS7846, using TCC0 overflow interrupt */
// Optimize for speed
#pragma optsize- 
// Low level interrupt: On
// Round-robin scheduling for low level interrupt: Off
// Medium level interrupt: Off
// High level interrupt: Off
// The interrupt vectors will be placed at the start of the Application FLASH section
n=(PMIC.CTRL & (~(PMIC_RREN_bm | PMIC_IVSEL_bm | PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm))) |
	PMIC_LOLVLEN_bm;
CCP=CCP_IOREG_gc;
PMIC.CTRL=n;
// Set the default priority for round-robin scheduling
PMIC.INTPRI=0x00;
// Restore optimization for size if needed
#pragma optsize_default

/* Initialize the ADS7846 resistive touch controller in differential mode */
if (!rt_init(RTOUCH_MODE_DIFFERENTIAL))
   {
   /* Globally enable interrupts in order to be able to sense /PENIRQ */
   #asm("sei")
   
   /* No valid touch controller calibration data found in EEPROM,
      so we need top calibrate it now */
   if (!calibrate_touchscreen())
      {
      glcd_outtextxyf(0,20,"Touch controller calibration failed");
      while (1); /* Calibration error, stop here */
      }

   glcd_outtextxyf(0,20,"Touch controller calibrated OK");
   delay_ms(3000);
   glcd_clear();
   demo_message();
   }

/* Globally enable interrupts in order to be able to sense /PENIRQ */
#asm("sei")

/* Plot some points and ask the user to touch them */
glcd_outtextxyf(0,10,"Touch cross center to read coordinates");
for (n=0; n<sizeof(test_points)/sizeof(RTPOINT_t); n++)
    {
    display_rt_point(test_points[n]);
    result=rt_getxy(&x,&y);
    erase_rt_point(test_points[n]);
    if (result)
       {
       sprintf(display_buf,"Coordinates read by the touchscreen:\nX=%d, Y=%d",x,y);
       glcd_outtextxy(0,10,display_buf);
       }
    else
       glcd_outtextxy(0,10,"Error reading coordinates");
    delay_ms(1000);
    }

delay_ms(2000);
glcd_clear();
glcd_outtextf("Touch the screen to draw...");

while (1)
    {
    if (rt_getxy(&x,&y))
       {
       glcd_setpixel(x,y);
       delay_ms(10);
       }
    }
}
