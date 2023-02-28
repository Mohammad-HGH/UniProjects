/**********************************************************
Ultrachip UC1608 Graphic LCD controller demo
(C) Copyright 2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

The program is designed to be compiled with the Evaluation
version, so it's capabilities are limited.

The demo is designed to run on an ATxmega128A1
XPlain development board with chip clock of 2 MHz.

An AGG240128A05-FHW-R 240x128 graphic LCD module
from Shenzen AV-Display Co. Ltd. is used for the
purpose of this demo.
The UC1608 controller is used in 4-bit parallel
mode connection.

The graphic LCD module connections are:

Pin 2  VB1-   ---
                 |
               ===== 4.7uF
               =====
                 | +
Pin 3  VB1+   ---

Pin 4  VB0-   ---
                 |
               ===== 4.7uF
               =====
                 | +
Pin 5  VB0+   ---

Pin 6  VLCD   ---
                 |
               ===== 0.1uF
               =====
                 |
                === GND

Pin 7  VBIAS  ---
                 |
               ===== 0.1uF
               =====
                 |
                === GND

Pin 8  VSS    - GND
Pin 9  VDD    - +3.3V

Pin 17 DB0    - PORTA bit 0
Pin 16 DB1    - PORTA bit 1
Pin 15 DB2    - PORTA bit 2
Pin 14 DB3    - PORTA bit 3
Pin 13 DB4    - GND (must be connected !)
Pin 12 DB5    - GND (must be connected !)
Pin 11 DB6    - GND (must be connected !)
Pin 10 DB7    - GND (must be connected !)

Pin 18 /RD    - PORTD bit 0
Pin 19 /WR    - PORTD bit 1
Pin 20 CD     - PORTD bit 2
Pin 21 /RST   - PORTD bit 3
Pin 22 CS     - PORTD bit 4

Pin 23 BM0    - GND
Pin 24 BM1    - GND

Make sure to specify the correct connections in the
Project|Configure|C Compiler|Libraries|Graphic LCD menu.
**********************************************************/

#include <io.h>
#include <glcd.h>
#include <delay.h>

void main()
{
int x;
GLCDINIT_t init;


/* no font is used */
init.font=NULL;

/* No need for reading data from external memory */
init.readxmem=NULL;

/* No need for reading data from external memory */
init.writexmem=NULL;

/* Set horizontal scan direction: reversed */
init.reverse_x=UC1608_REVX_REV;
/* Set vertical scan direction: normal */
init.reverse_y=UC1608_REVY_NORM;
/* Set LCD temperature compensation coeficient */
init.temp_comp=UC1608_DEFAULT_TEMP_COMP;
/* Set LCD panel capacitance */
init.panel_cap=UC1608_DEFAULT_PANEL_CAP;
/* Set LCD bias voltage ratio */
init.lcd_bias=UC1608_DEFAULT_LCD_BIAS;
/* Set LCD contrast */
init.contrast=UC1608_DEFAULT_LCD_CONTRAST;

/* Initialize the LCD controller and graphics */
glcd_init(&init);

/* Draw some lines */
for (x=0; x<80; x+=4) glcd_line(0,0,x,63);
for (x=79; x>=0; x-=4) glcd_line(79,63,x,0);

/* Draw various styles of lines */
/* Line thickness: 1 pixel */
glcd_line(85,10,127,10);

glcd_setlinestyle(1,GLCD_LINE_DOT_SMALL);
glcd_line(85,15,127,15);

glcd_setlinestyle(1,GLCD_LINE_DOT_LARGE);
glcd_line(85,20,127,20);

/* Line thickness: 2 pixels */
glcd_setlinestyle(2,GLCD_LINE_SOLID);
glcd_line(85,25,127,25);

glcd_setlinestyle(2,GLCD_LINE_DOT_SMALL);
glcd_line(85,30,127,30);

glcd_setlinestyle(2,GLCD_LINE_DOT_LARGE);
glcd_line(85,35,127,35);

/* Line thickness: 4 pixels */
glcd_setlinestyle(4,GLCD_LINE_SOLID);
glcd_line(85,40,127,40);

glcd_setlinestyle(4,GLCD_LINE_DOT_SMALL);
glcd_line(85,45,127,45);

glcd_setlinestyle(4,GLCD_LINE_DOT_LARGE);
glcd_line(85,50,127,50);

/* Draw a circle with line thickness: 1 pixel */
glcd_setlinestyle(1,GLCD_LINE_SOLID);
glcd_circle(183,31,30);

/* Draw a circle with line thickness: 4 pixels */
glcd_setlinestyle(4,GLCD_LINE_SOLID);
glcd_circle(183,31,25);
}
