/**********************************************************
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
**********************************************************/

#include <glcd.h>
#include <delay.h>

void main()
{
int x;
unsigned char k,c;
GLCDINIT_t init;

/* No font is used */
init.font=NULL;
 
/* No need for reading data from external memory */
init.readxmem=NULL;

/* No need for reading data from external memory */ 
init.writexmem=NULL;

/* Initialize the LCD controller and graphics */
glcd_init(&init); 

/* Display text using the built-in character generator */
lcd_putsf("Sitronix ST7920\nCodeVisionAVR\nDemo");

/* 2 seconds delay */          
delay_ms(2000);

lcd_clear();
lcd_putsf("Text is display-\n"
          "ed using the\n"
          "internal chara-\n"
          "cter generator");

/* 2 seconds delay */          
delay_ms(2000);
lcd_clear();

/* Display the internal character set */
for (x=1; x<=0x7F; x+=64)
    {
    lcd_clear();
    for (c=0; c<=63;c++)
        {
        k=c+x;
        if (k!='\n') lcd_putchar(k);
        }
    /* 3 seconds delay */          
    delay_ms(3000);
    }

lcd_clear();

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
