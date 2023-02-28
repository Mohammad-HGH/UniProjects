/**********************************************************
Sitronix ST7920 Graphic LCD controller demo
(C) Copyright 2011-2012 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

The ST7920 controller is interfaced to the
AVR chip in 8 bit mode.

The program is designed to be compiled with the Evaluation
version, so it's capabilities are limited.

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
**********************************************************/

#include <graphics.h>
#include <delay.h>

void main()
{
int x;
unsigned char k,c;
GLCDINIT_t init;

/* The internal character generator font is used */
init.font=NULL; 
/* No need for reading data from external memory */
init.readxmem=NULL;

/* No need for reading data from external memory */ 
init.writexmem=NULL;

/* Initialize the LCD controller and graphics */
glcd_init(&init); 

/* Display text using the built-in character generator */
lcd_putsf("Sitronix ST7920\nCodeVisionAVR Demo");

/* 2 seconds delay */          
delay_ms(2000);

lcd_clear();
lcd_putsf("Text is displayed using\n"
          "the internal char. gen.");

/* 2 seconds delay */          
delay_ms(2000);

/* Display the internal character set */
for (x=2; x<=0x7F; x+=48)
    {
    lcd_clear();
    for (c=0; c<=47; c++)
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
glcd_line(85,0,150,0);

glcd_setlinestyle(1,GLCD_LINE_DOT_SMALL);
glcd_line(85,4,150,4);

glcd_setlinestyle(1,GLCD_LINE_DOT_LARGE);
glcd_line(85,8,150,8);

/* Line thickness: 2 pixels */
glcd_setlinestyle(2,GLCD_LINE_SOLID);
glcd_line(85,11,150,11);

glcd_setlinestyle(2,GLCD_LINE_DOT_SMALL);
glcd_line(85,15,150,15);

glcd_setlinestyle(2,GLCD_LINE_DOT_LARGE);
glcd_line(85,19,150,19);

/* Line thickness: 4 pixels */
glcd_setlinestyle(4,GLCD_LINE_SOLID);
glcd_line(85,22,150,22);

glcd_setlinestyle(4,GLCD_LINE_DOT_SMALL);
glcd_line(85,27,150,27);

/* Draw a circle with line thickness: 1 pixel */
glcd_setlinestyle(1,GLCD_LINE_SOLID);
glcd_circle(170,15,14);

/* Draw a circle with line thickness: 4 pixels */
glcd_setlinestyle(4,GLCD_LINE_SOLID);
glcd_circle(170,15,12);
}
