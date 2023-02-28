/**********************************************************
Epson SED1335 Graphic LCD controller demo
(C) Copyright 2011 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

The program shows the capabilities of some of the most
important functions of the CodeVisionAVR's graphic library.

The demo is designed to run on any ATmega128A
development board with chip clock of 16 MHz.

The 320x240 graphic LCD module connections are:
DB0  - PORTA bit 0
DB1  - PORTA bit 1
DB2  - PORTA bit 2
DB3  - PORTA bit 3
DB4  - PORTA bit 4
DB5  - PORTA bit 5
DB6  - PORTA bit 6
DB7  - PORTA bit 7

A0   - PORTC bit 0
/CS  - PORTC bit 1
/RST - PORTC bit 7
/WR  - PORTG bit 0
/RD  - PORTG bit 1

The connections can be modified by accessing the
Project|Configure|C Compiler|Libraries|Graphic LCD menu.

The LCD module contrast control voltage must be supplied
as specified in it's datasheet.

This program exceeds the code size limit of the Evaluation
version of CodeVisionAVR.
In this case you can program the ATmega128A FLASH with
the contents of the
\EXAMPLES\Graphic LCD\SED1335\Full_Demo\Exe\Full_Demo.hex
file.
**********************************************************/

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
glcd_outtextf("Epson SED1335 Graphics Demo\nHere are some font samples:");

/* Display some font samples */
glcd_outtextxyf(0,20,"Standard, 5x7 pixels");

glcd_setfont(arial14);
glcd_outtextxyf(0,30,"Arial, 14 pixels height\n");

glcd_setfont(arial_bold14);
glcd_outtextf("Arial Bold, 14 pixels height\n");

glcd_setfont(courier7x14);
glcd_outtextf("Courier, 7x14 pixels\n");

glcd_setfont(courier_bold14);
glcd_outtextf("Courier Bold, 14 pixels height\n");

glcd_setfont(script19);
glcd_outtextf("Script, 19 pixels height\n");

glcd_setfont(font5x7);
glcd_outtextf("\nVariable text spacing:\n");
glcd_setfont(courier_bold14);
glcd_outtextf("Epson SED1335\n");
/* Set horizontal justification: 2 pixels and
   vertical justification: 1 pixel */
glcd_settextjustify(2,1);
glcd_outtextf("Epson SED1335\n");
/* Set horizontal justification: 3 pixels and
   vertical justification: 1 pixel */
glcd_settextjustify(3,1);
glcd_outtextf("Epson SED1335");

/* Set horizontal justification: 1 pixels and
   vertical justification: 1 pixel */
glcd_settextjustify(1,1);
glcd_setfont(font5x7);
glcd_outtextxyf(0,180,"Line styles:");

/* Draw various styles of lines */
/* Line thickness: 1 pixel */
glcd_line(0,190,319,190);

glcd_setlinestyle(1,GLCD_LINE_DOT_SMALL);
glcd_line(0,195,319,195);

glcd_setlinestyle(1,GLCD_LINE_DOT_LARGE);
glcd_line(0,200,319,200);

/* Line thickness: 2 pixels */
glcd_setlinestyle(2,GLCD_LINE_SOLID);
glcd_line(0,205,319,205);

glcd_setlinestyle(2,GLCD_LINE_DOT_SMALL);
glcd_line(0,210,319,210);

glcd_setlinestyle(2,GLCD_LINE_DOT_LARGE);
glcd_line(0,215,319,215);

/* Line thickness: 4 pixels */
glcd_setlinestyle(4,GLCD_LINE_SOLID);
glcd_line(0,220,319,220);

glcd_setlinestyle(4,GLCD_LINE_DOT_SMALL);
glcd_line(0,225,319,225);

glcd_setlinestyle(4,GLCD_LINE_DOT_LARGE);
glcd_line(0,230,319,230);

/* 8 seconds delay */
delay_ms(8000);
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
glcd_outtextxyf(0,47,"Image was saved to EEPROM");

/* Draw a circle with line thickness: 1 pixel */
glcd_circle(21,83,20);

/* Draw an arc of a circle with line thickness: 1 pixel */
glcd_arc(21,83,0,120,15);

/* Draw a circle with line thickness: 3 pixels */
glcd_setlinestyle(3,GLCD_LINE_SOLID);
glcd_circle(63,83,20);

/* Draw an arc of a circle with line thickness: 3 pixels */
glcd_arc(63,83,0,210,15);

glcd_outtextxyf(0,107,"Circles & arcs");

/* Draw a rectangle with line thickness: 1 pixel 
   and solid line */
glcd_setlinestyle(1,GLCD_LINE_SOLID);
glcd_rectrel(100,60,40,25);

/* Draw a rectangle with line thickness: 3 pixel
   and dotted line */
glcd_setlinestyle(3,GLCD_LINE_DOT_LARGE);
glcd_rectrel(142,60,40,25);

/* Draw a bar filled with a pattern */
glcd_setfillf(small_dots,1);
glcd_bar(114,89,170,104);

glcd_outtextxyf(100,107,"Rectangles & filled bar");

/* Draw rounded rectangle */
glcd_rectround(5,125,74,38,8);
glcd_outtextxyf(16,136,"Rounded");
glcd_outtextxyf(16,144,"rectangle");

/* Set line thickness: 1 pixel */
glcd_setlinestyle(1,GLCD_LINE_SOLID);

/* Draw filled pie slice */
glcd_outtextxyf(100,120,"Pie");
glcd_outtextxyf(100,127,"slice:");
glcd_pieslice(160,143,30,300,20);

glcd_outtextxyf(0,170,"Put\nimage\nfrom\nEEPROM:");
/* Display image saved in EEPROM */
glcd_putimagee(46,170,buffer,GLCD_PUTCOPY);

glcd_outtextxyf(100,170,"Put");
glcd_outtextxyf(100,177,"inverted");
glcd_outtextxyf(100,185,"image");
glcd_outtextxyf(100,193,"from");
glcd_outtextxyf(100,201,"EEPROM:");
/* Display image saved in EEPROM,
   using bitwise negation */
glcd_putimagee(156,170,buffer,GLCD_PUTNOT);
}
