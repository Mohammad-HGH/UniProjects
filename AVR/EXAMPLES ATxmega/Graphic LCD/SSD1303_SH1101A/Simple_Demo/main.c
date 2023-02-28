/*******************************************************************
Solomon Systech SSD1303/Sino Wealth SH1101A Graphic controller demo
(C) Copyright 2011-2012 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

The program is designed to be compiled with the Evaluation
version, so it's capabilities are limited.

The demo is designed to run on an ATxmega128A1
XPlain development board with chip clock of 2 MHz.

A Bolymin BL12864G 128x64 OLED display module was
used for the purpose of this demo.

The display module connections are:
4  DB0    - PORTA bit 0
5  DB1    - PORTA bit 1
6  DB2    - PORTA bit 2
7  DB3    - PORTA bit 3
8  DB4    - PORTA bit 4
9  DB5    - PORTA bit 5
10 DB6    - PORTA bit 6
11 DB7    - PORTA bit 7

16 D/C    - PORTD bit 3
14 /RES   - PORTD bit 4
12 /CS    - PORTD bit 5
17 /RD    - PORTD bit 6
15 /WR    - PORTD bit 7

1  Vdd    - +3.3V from power supply (140mA)
2  Vss    - GND

19 DISP   - +3.3V

Make sure to specify the correct connections in the
Project|Configure|C Compiler|Libraries|Graphic LCD menu.
*******************************************************************/

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

/* Set horizontal scan direction for Bolymin BL12864G: reversed */
init.reverse_x=SSD1303_REVX_REV;

/* Set reversed X offset for Bolymin BL12864G
   For other display types must be changed
   to 132-_GLCD_MAXX_ when scan direction is reversed */
init.reverse_xoffs=2;

/* Set vertical scan direction for Bolymin BL12864G: normal */
init.reverse_y=SSD1303_REVY_NORM;

/* Set value for COM outputs configuration
   for Bolymin BL12864G: interlaced */
init.interlaced=SSD1303_INTERLACED;

/* Set contrast */
init.contrast=SSD1303_DEFAULT_CONTRAST;

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

/* 4 seconds delay */
delay_ms(4000);

/* Clear display */
glcd_clear();

/* Draw a circle with line thickness: 1 pixel */
glcd_setlinestyle(1,GLCD_LINE_SOLID);
glcd_circle(63,31,30);

/* Draw a circle with line thickness: 4 pixels */
glcd_setlinestyle(4,GLCD_LINE_SOLID);
glcd_circle(63,31,25);
}
