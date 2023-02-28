/**********************************************************
Samsung KS0108 Graphic LCD controller demo
(C) Copyright 2011 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

The program is designed to be compiled with the Evaluation
version, so it's capabilities are limited.

The demo is designed to run on any ATmega128A
development board with chip clock of 16 MHz.

The 128x64 graphic LCD module connections are:
DB0    - PORTA bit 0
DB1    - PORTA bit 1
DB2    - PORTA bit 2
DB3    - PORTA bit 3
DB4    - PORTA bit 4
DB5    - PORTA bit 5
DB6    - PORTA bit 6
DB7    - PORTA bit 7

E      - PORTC bit 0
R/W    - PORTC bit 1
RS     - PORTC bit 2
RESET  - PORTC bit 3
CS1    - PORTC bit 4
CS2    - PORTC bit 5

Important Note: There are several kinds of 128x64
pixels KS0108 LCD modules.
Some of them have active high Chip Select signals
(CS1, CS2), while others have active low Chip
Select signals (/CS1, /CS2).
Make sure to select the correct display type in the
Project|Configure|C Compiler|Libraries|Graphic LCD menu.

The LCD module contrast control voltage must be supplied
as specified in it's datasheet.
**********************************************************/

#include <glcd.h>
#include <delay.h>

void main()
{
int x;
GLCDINIT_t init;

/* No font is used */
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
