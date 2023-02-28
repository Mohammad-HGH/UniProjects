/**********************************************************************
Sitronix ST7920 Graphic LCD controller demo
(C) Copyright 2011-2012 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

The ST7920 controller is interfaced to the
AVR chip in 8 bit mode.

The demo is designed to run on any ATmega328P
development board with chip clock of 16 MHz.
(Arduino Uno board)

A Newhaven Display NHD-19232WG-BTMI-V#T
192x32 pixels graphic LCD module was used.

The LCD module to Arduino Uno connections are:
 7 DB0  - PORTD bit 0 (Digital 0)
 8 DB1  - PORTD bit 1 (Digital 1)
 9 DB2  - PORTD bit 2 (Digital 2)
10 DB3  - PORTD bit 3 (Digital 3)
11 DB4  - PORTD bit 4 (Digital 4)
12 DB5  - PORTD bit 5 (Digital 5)
13 DB6  - PORTD bit 6 (Digital 6)
14 DB7  - PORTD bit 7 (Digital 7)

 6 E    - PORTB bit 0 (Digital 8)
 5 R/W  - PORTB bit 1 (Digital 9)
 4 RS   - PORTB bit 2 (Digital 10)

 1 VSS  - GND
 2 VDD  - +5V
 3 VO   - -------------| Contrast control voltage
                       |
                       V  10k
                 |-----------|
15 Vout - -------|           |--|GND
                 |-----------|

The connections can be modified by accessing the
Project|Configure|C Compiler|Libraries|Graphic LCD menu.

This program exceeds the code size limit of the Evaluation
version of CodeVisionAVR.
In this case you can program the ATmega328P FLASH with
the contents of the
\EXAMPLES\Graphic LCD\ST7920\192x32 8 bit\Full_Demo\Exe\Full_Demo.hex
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
{32,1},
{46,8},
{46,24},
{33,31},
{19,24},
{19,8},
{32,1}
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

/* 4 seconds delay */
delay_ms(3000);
/* Clear display */
glcd_clear();

glcd_setfont(arial14);
glcd_outtextxyf(0,0,"Arial 14\n");

glcd_setfont(arial_bold14);
glcd_outtextf("Arial Bold 14");

/* 4 seconds delay */
delay_ms(3000);
/* Clear display */
glcd_clear();

glcd_setfont(courier7x14);
glcd_outtextf("Courier 7x14\n");

glcd_setfont(courier_bold14);
glcd_outtextf("Courier Bold 14");

/* 3 seconds delay */
delay_ms(3000);
/* Clear display */
glcd_clear();

glcd_setfont(script19);
glcd_outtextf("Script 19");

/* 3 seconds delay */
delay_ms(3000);
/* Clear display */
glcd_clear();

glcd_setfont(font5x7);
glcd_outtextf("Variable text spacing:\n");
glcd_setfont(courier_bold14);
glcd_outtextf("ST7920 Demo");
/* Set horizontal justification: 3 pixels and
   vertical justification: 1 pixel */
glcd_settextjustify(3,1);
glcd_outtextxyf(0,20,"ST7920 Demo");

/* 3 seconds delay */
delay_ms(3000);
/* Clear display */
glcd_clear();

/* Set horizontal justification: 1 pixels and
   vertical justification: 1 pixel */
glcd_settextjustify(1,1);
glcd_setfont(font5x7);
glcd_outtextf("Some line styles:");

/* Draw various styles of lines */
/* Line thickness: 1 pixel */
glcd_line(110,0,190,0);

glcd_setlinestyle(1,GLCD_LINE_DOT_SMALL);
glcd_line(110,4,190,4);

glcd_setlinestyle(1,GLCD_LINE_DOT_LARGE);
glcd_line(110,8,190,8);

/* Line thickness: 2 pixels */
glcd_setlinestyle(2,GLCD_LINE_SOLID);
glcd_line(110,11,190,11);

glcd_setlinestyle(2,GLCD_LINE_DOT_SMALL);
glcd_line(110,15,190,15);

glcd_setlinestyle(2,GLCD_LINE_DOT_LARGE);
glcd_line(110,19,190,19);

/* Line thickness: 4 pixels */
glcd_setlinestyle(4,GLCD_LINE_SOLID);
glcd_line(110,22,190,22);

glcd_setlinestyle(4,GLCD_LINE_DOT_SMALL);
glcd_line(110,27,190,27);

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
glcd_floodfill(24,12,1);

/* Write a text inside the filled hexagon in
   reversed colors */
glcd_setcolor(0);
glcd_setbkcolor(1);
glcd_outtextxyf(22,13,"Fill");

/* Save hexagon image in EEPROM */
glcd_getimagee(17,0,32,32,buffer);

/* Restore the default foreground
   and background colors */
glcd_setcolor(1);
glcd_setbkcolor(0);
glcd_outtextxyf(50,9,"Image was saved to");
glcd_outtextxyf(50,17,"EEPROM");

/* 3 seconds delay */
delay_ms(3000);
/* Clear display */
glcd_clear();

/* Draw a circle with line thickness: 1 pixel */
glcd_circle(16,16,15);

/* Draw an arc of a circle with line thickness: 1 pixel */
glcd_arc(16,16,0,120,11);

/* Draw a circle with line thickness: 3 pixels */
glcd_setlinestyle(3,GLCD_LINE_SOLID);
glcd_circle(63,16,15);

/* Draw an arc of a circle with line thickness: 3 pixels */
glcd_arc(63,16,0,210,10);

glcd_outtextxyf(85,13,"Circles & arcs");

/* 3 seconds delay */
delay_ms(3000);
/* Clear display */
glcd_clear();

/* Draw a rectangle with line thickness: 1 pixel 
   and solid line */
glcd_setlinestyle(1,GLCD_LINE_SOLID);
glcd_rectrel(0,0,40,20);

/* Draw a rectangle with line thickness: 3 pixel
   and dotted line */
glcd_setlinestyle(3,GLCD_LINE_DOT_LARGE);
glcd_rectrel(50,0,40,20);

/* Draw a bar filled with a pattern */
glcd_setfillf(small_dots,1);
glcd_bar(100,0,140,20);

glcd_outtextxyf(0,24,"Rectangles & filled bar");

/* 3 seconds delay */
delay_ms(3000);
/* Clear display */
glcd_clear();

/* Draw rounded rectangle */
glcd_rectround(5,1,74,30,8);
glcd_outtextxyf(16,8,"Rounded");
glcd_outtextxyf(16,16,"rectangle");

/* 3 seconds delay */
delay_ms(3000);
/* Clear display */
glcd_clear();

/* Set line thickness: 1 pixel */
glcd_setlinestyle(1,GLCD_LINE_SOLID);

/* Draw filled pie slice */
glcd_outtextf("Pie\nslice:");
glcd_pieslice(60,15,14,300,15);

/* 3 seconds delay */
delay_ms(3000);
/* Clear display */
glcd_clear();

glcd_outtextf("Put image\nfrom EEPROM:");
/* Display image saved in EEPROM */
glcd_putimagee(75,0,buffer,GLCD_PUTCOPY);

/* 3 seconds delay */
delay_ms(3000);
/* Clear display */
glcd_clear();

glcd_outtextf("Put inverted\nimage from EEPROM:");
/* Display image saved in EEPROM,
   using bitwise negation */
glcd_putimagee(115,0,buffer,GLCD_PUTNOT);
}
