/**********************************************************************
Sitronix ST7920 Graphic LCD controller demo
(C) Copyright 2011-2012 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

The ST7920 controller is interfaced to the
AVR chip in 4 bit mode.

The program is designed to be compiled with the Evaluation
version, so it's capabilities are limited.

The demo is designed to run on any ATmega328P
development board with chip clock of 16 MHz.
(Arduino Uno board)

A CX12864B-1 128x64 pixels graphic LCD module was used.

The LCD module to Arduino Uno connections are:
 7 DB0  - not connected
 8 DB1  - not connected
 9 DB2  - not connected
10 DB3  - not connected
11 DB4  - PORTD bit 0 (Digital 0)
12 DB5  - PORTD bit 1 (Digital 1)
13 DB6  - PORTD bit 2 (Digital 2)
14 DB7  - PORTD bit 3 (Digital 3)

 6 E    - PORTD bit 4 (Digital 4)
 5 R/W  - PORTD bit 5 (Digital 5)
 4 RS   - PORTD bit 6 (Digital 6)
17 /RST - PORTD bit 7 (Digital 7)

15 PSB  - +5V

 1 VSS  - GND
 2 VDD  - +5V
 3 VO   - -------------| Contrast control voltage
                       |
                       V  10k
                 |-----------|
18 Vout - -------|           |--|GND
                 |-----------|

The connections can be modified by accessing the
Project|Configure|C Compiler|Libraries|Graphic LCD menu.

This program exceeds the code size limit of the Evaluation
version of CodeVisionAVR.
In this case you can program the ATmega328P FLASH with
the contents of the
\EXAMPLES\Graphic LCD\ST7920\128x64 4 bit\Full_Demo\Exe\Full_Demo.hex
file.
**********************************************************************/

#include <glcd.h>
#include <delay.h>

/* Include some fonts needed by the demo */
#include <font5x7.h>
#include "arial14.h"
#include "arial_bold14.h"
#include "courier7x14.h"
#include "courier_bold14.h"
#include "script19.h"

/* Small dots filling pattern */
flash unsigned char small_dots[]={0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55};

/* EEPROM buffer used for image storage */
eeprom unsigned char buffer[1000];

/* Hexagon point coordinates
   The last point's coordinates must match
   the first point coordinates,
   so that the polygon will be closed */
flash GLCDPOINT_t hexagon[7]=
{
{40,0},
{57,10},
{57,30},
{40,40},
{23,30},
{23,10},
{40,0}
};


void main()
{
GLCDINIT_t init;

/* Set current font */
init.font=font5x7;
/* No need for reading data from external memory */
init.readxmem=NULL;

/* No need for reading data from external memory */ 
init.writexmem=NULL;

/* Initialize the LCD controller and graphics */
glcd_init(&init); 

/* Display literal char string located in FLASH */
glcd_outtextf("ST7920 Graphics Demo\nFont samples:");

/* Display some font samples */
glcd_outtextxyf(0,20,"Standard 5x7");

glcd_setfont(arial14);
glcd_outtextxyf(0,30,"Arial 14\n");

glcd_setfont(arial_bold14);
glcd_outtextf("Arial Bold 14");

/* 4 seconds delay */
delay_ms(4000);
/* Clear display */
glcd_clear();

glcd_setfont(courier7x14);
glcd_outtextf("Courier 7x14\n");

glcd_setfont(courier_bold14);
glcd_outtextf("Courier Bold 14");

glcd_setfont(script19);
glcd_outtextxyf(0,30,"Script 19");

/* 4 seconds delay */
delay_ms(4000);
/* Clear display */
glcd_clear();

glcd_setfont(font5x7);
glcd_outtextf("Var. text spacing:\n");
glcd_setfont(courier_bold14);
glcd_outtextf("ST7920 Demo\n");
/* Set horizontal justification: 2 pixels and
   vertical justification: 1 pixel */
glcd_settextjustify(2,1);
glcd_outtextf("ST7920 Demo\n");
/* Set horizontal justification: 3 pixels and
   vertical justification: 1 pixel */
glcd_settextjustify(3,1);
glcd_outtextf("ST7920 Demo");

/* 4 seconds delay */
delay_ms(4000);
/* Clear display */
glcd_clear();

/* Set horizontal justification: 1 pixels and
   vertical justification: 1 pixel */
glcd_settextjustify(1,1);
glcd_setfont(font5x7);
glcd_outtextf("Some line styles:");

/* Draw various styles of lines */
/* Line thickness: 1 pixel */
glcd_line(0,10,127,10);

glcd_setlinestyle(1,GLCD_LINE_DOT_SMALL);
glcd_line(0,15,127,15);

glcd_setlinestyle(1,GLCD_LINE_DOT_LARGE);
glcd_line(0,20,127,20);

/* Line thickness: 2 pixels */
glcd_setlinestyle(2,GLCD_LINE_SOLID);
glcd_line(0,25,127,25);

glcd_setlinestyle(2,GLCD_LINE_DOT_SMALL);
glcd_line(0,30,127,30);

glcd_setlinestyle(2,GLCD_LINE_DOT_LARGE);
glcd_line(0,35,127,35);

/* Line thickness: 4 pixels */
glcd_setlinestyle(4,GLCD_LINE_SOLID);
glcd_line(0,40,127,40);

glcd_setlinestyle(4,GLCD_LINE_DOT_SMALL);
glcd_line(0,45,127,45);

glcd_setlinestyle(4,GLCD_LINE_DOT_LARGE);
glcd_line(0,50,127,50);

/* 3 seconds delay */
delay_ms(3000);
/* Clear display */
glcd_clear();

/* Draw a hexagon with line thickness: 1 pixel */
glcd_setlinestyle(1,GLCD_LINE_SOLID);
glcd_drawpoly(7,hexagon);

/* 1 second delay */
delay_ms(1000);

/* Fill the hexagon with solid fill */
glcd_floodfill(48,24,1);

/* Write a text inside the filled hexagon in
   reversed colors */
glcd_setcolor(0);
glcd_setbkcolor(1);
glcd_outtextxyf(30,17,"Fill");

/* Save hexagon image in EEPROM */
glcd_getimagee(22,0,37,41,buffer);
/* Restore the default foreground
   and background colors */
glcd_setcolor(1);
glcd_setbkcolor(0);
glcd_outtextxyf(0,47,"Image was saved to\nEEPROM");

/* 3 seconds delay */
delay_ms(3000);
/* Clear display */
glcd_clear();

/* Draw a circle with line thickness: 1 pixel */
glcd_circle(21,23,20);

/* Draw an arc of a circle with line thickness: 1 pixel */
glcd_arc(21,23,0,120,15);

/* Draw a circle with line thickness: 3 pixels */
glcd_setlinestyle(3,GLCD_LINE_SOLID);
glcd_circle(63,23,20);

/* Draw an arc of a circle with line thickness: 3 pixels */
glcd_arc(63,23,0,210,15);

glcd_outtextxyf(0,47,"Circles & arcs");

/* 3 seconds delay */
delay_ms(3000);
/* Clear display */
glcd_clear();

/* Draw a rectangle with line thickness: 1 pixel 
   and solid line */
glcd_setlinestyle(1,GLCD_LINE_SOLID);
glcd_rectrel(0,0,40,25);

/* Draw a rectangle with line thickness: 3 pixel
   and dotted line */
glcd_setlinestyle(3,GLCD_LINE_DOT_LARGE);
glcd_rectrel(42,0,40,25);

/* Draw a bar filled with a pattern */
glcd_setfillf(small_dots,1);
glcd_bar(14,29,70,44);

glcd_outtextxyf(0,47,"Rectangles &\nfilled bar");

/* 3 seconds delay */
delay_ms(3000);
/* Clear display */
glcd_clear();

/* Draw rounded rectangle */
glcd_rectround(5,5,74,38,8);
glcd_outtextxyf(16,16,"Rounded");
glcd_outtextxyf(16,24,"rectangle");

/* 3 seconds delay */
delay_ms(3000);
/* Clear display */
glcd_clear();

/* Set line thickness: 1 pixel */
glcd_setlinestyle(1,GLCD_LINE_SOLID);

/* Draw filled pie slice */
glcd_outtextf("Pie\nslice:");
glcd_pieslice(60,23,30,300,20);

/* 3 seconds delay */
delay_ms(3000);
/* Clear display */
glcd_clear();

glcd_outtextf("Put\nimage\nfrom\nEEPROM:");
/* Display image saved in EEPROM */
glcd_putimagee(56,0,buffer,GLCD_PUTCOPY);

/* 3 seconds delay */
delay_ms(3000);
/* Clear display */
glcd_clear();

glcd_outtextf("Put\ninverted\nimage\nfrom\nEEPROM:");
/* Display image saved in EEPROM,
   using bitwise negation */
glcd_putimagee(56,0,buffer,GLCD_PUTNOT);
}
