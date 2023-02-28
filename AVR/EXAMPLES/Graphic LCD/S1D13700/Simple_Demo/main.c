/**********************************************************
Epson S1D13700 Graphic LCD controller demo
(C) Copyright 2011 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

The program is designed to be compiled with the Evaluation
version, so it's capabilities are limited.

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

/WR  - PORTC bit 0
/RD  - PORTC bit 1
A0   - PORTC bit 2
/RST - PORTC bit 3
/CS  - PORTC bit 4

The connections can be modified by accessing the
Project|Configure|C Compiler|Libraries|Graphic LCD menu.

The LCD module contrast control voltage must be supplied
as specified in it's datasheet.
**********************************************************/

#include <glcd.h>

void main()
{
int x;
GLCDINIT_t init;

/* The internal character generator font is used */
init.font=NULL; 
/* No need for reading data from external memory */
init.readxmem=NULL;

/* No need for reading data from external memory */ 
init.writexmem=NULL;

/* Initialize the LCD controller and graphics */
glcd_init(&init); 

/* Draw some lines */
for (x=0; x<80; x+=4) glcd_line(0,0,x,63);
for (x=79; x>=0; x-=4) glcd_line(79,63,x,0);

/* Draw various styles of lines */
/* Line thickness: 1 pixel */
glcd_line(85,10,170,10);

glcd_setlinestyle(1,GLCD_LINE_DOT_SMALL);
glcd_line(85,15,170,15);

glcd_setlinestyle(1,GLCD_LINE_DOT_LARGE);
glcd_line(85,20,170,20);

/* Line thickness: 2 pixels */
glcd_setlinestyle(2,GLCD_LINE_SOLID);
glcd_line(85,25,170,25);

glcd_setlinestyle(2,GLCD_LINE_DOT_SMALL);
glcd_line(85,30,170,30);

glcd_setlinestyle(2,GLCD_LINE_DOT_LARGE);
glcd_line(85,35,170,35);

/* Line thickness: 4 pixels */
glcd_setlinestyle(4,GLCD_LINE_SOLID);
glcd_line(85,40,170,40);

glcd_setlinestyle(4,GLCD_LINE_DOT_SMALL);
glcd_line(85,45,170,45);

glcd_setlinestyle(4,GLCD_LINE_DOT_LARGE);
glcd_line(85,50,170,50);

/* Draw a circle with line thickness: 1 pixel */
glcd_setlinestyle(1,GLCD_LINE_SOLID);
glcd_circle(205,31,30);

/* Draw a circle with line thickness: 4 pixels */
glcd_setlinestyle(4,GLCD_LINE_SOLID);
glcd_circle(205,31,25);
}
