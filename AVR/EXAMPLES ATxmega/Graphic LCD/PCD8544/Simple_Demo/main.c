/**********************************************************
Philips PCD8544 Graphic LCD controller demo
(C) Copyright 2011 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

The program is designed to be compiled with the Evaluation
version, so it's capabilities are limited.

The demo is designed to run on Atmel's Xplain ATxmega128A1
development board.
The ATxmega128A1 chip runs from the internal RC oscillator
at 2 MHz.

The 84x48 graphic LCD module (Nokia 3310, 5110 compatible)
is connected to PORTD:
/SCE - PORTD bit 0
/RES - PORTD bit 1
D/C  - PORTD bit 2
SDIN - PORTD bit 3
SCLK - PORTD bit 4

The connections can be modified by accessing the
Project|Configure|C Compiler|Libraries|Graphic LCD menu.
**********************************************************/

#include <glcd.h>
#include <delay.h>

void main()
{
int x;
GLCDINIT_t init;

init.font=NULL; /* No font is used */
init.temp_coef=PCD8544_DEFAULT_TEMP_COEF;
init.bias=PCD8544_DEFAULT_BIAS;
init.vlcd=PCD8544_DEFAULT_VLCD;
/* No need for reading data from external memory */
init.readxmem=NULL;

/* No need for reading data from external memory */ 
init.writexmem=NULL;

/* Initialize the LCD controller and graphics */
glcd_init(&init); 

/* Draw some lines */
for (x=0; x<84; x+=4) glcd_line(0,0,x,47);
for (x=83; x>=0; x-=4) glcd_line(83,47,x,0);

/* 4 seconds delay */
delay_ms(4000);
/* Clear display */
glcd_clear();

/* Draw various styles of lines */
/* Line thickness: 1 pixel */
glcd_line(0,10,83,10);

glcd_setlinestyle(1,GLCD_LINE_DOT_SMALL);
glcd_line(0,15,83,15);

glcd_setlinestyle(1,GLCD_LINE_DOT_LARGE);
glcd_line(0,20,83,20);

/* Line thickness: 3 pixels */
glcd_setlinestyle(3,GLCD_LINE_SOLID);
glcd_line(0,25,83,25);

glcd_setlinestyle(3,GLCD_LINE_DOT_SMALL);
glcd_line(0,30,83,30);

glcd_setlinestyle(3,GLCD_LINE_DOT_LARGE);
glcd_line(0,35,83,35);

/* 4 seconds delay */
delay_ms(4000);
/* Clear display */
glcd_clear();

/* Draw a circle with line thickness: 1 pixel */
glcd_setlinestyle(1,GLCD_LINE_SOLID);
glcd_circle(41,23,22);

}
