/**********************************************************
SPLC501C Graphic LCD controller demo
(C) Copyright 2011 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

The program is designed to be compiled with the Evaluation
version, so it's capabilities are limited.

The demo is designed to run on an ATxmega128A1
XPlain development board with chip clock of 2 MHz.

The 128x64 graphic LCD module connections are:
DB0    - PORTA bit 0
DB1    - PORTA bit 1
DB2    - PORTA bit 2
DB3    - PORTA bit 3
DB4    - PORTA bit 4
DB5    - PORTA bit 5
DB6    - PORTA bit 6
DB7    - PORTA bit 7

A0     - PORTD bit 3
/RES   - PORTD bit 4
/CS    - PORTD bit 5
/RD    - PORTD bit 6
/WR    - PORTD bit 7

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

/* No font is used */
init.font=NULL;
 
/* No need for reading data from external memory */
init.readxmem=NULL;

/* No need for reading data from external memory */ 
init.writexmem=NULL;

/* Set horizontal scan direction */
init.reverse_x=SPLC501_DEFAULT_REVX;
init.rev132_x0=SPLC501_DEFAULT_REV132_X0;

/* Set vertical scan direction */
init.reverse_y=SPLC501_DEFAULT_REVY;

/* V5 voltage regulator internal resistor ratio */
init.volt_reg_v5=SPLC501_DEFAULT_VOLT_REG_V5;

/* LCD bias initialization */
init.lcd_bias=SPLC501_DEFAULT_LCD_BIAS;

/* LCD contrast */
init.lcd_contrast=SPLC501_DEFAULT_CONTRAST;

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
