/*********************************************************************
Solomon Systech SSD1322 Graphic controller demo
(C) Copyright 2011-2013 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

The program is designed to be compiled with the Evaluation
version, so it's capabilities are limited.

The demo is designed to run on an ATxmega128A1
XPlained development board with chip clock of 2 MHz.

A Densitron DD-12864YW-13A 128x64 OLED display module was
used for the purpose of this demo.

The display module connections using the 8080 parallel interface are:
DB0    - PORTA bit 0
DB1    - PORTA bit 1
DB2    - PORTA bit 2
DB3    - PORTA bit 3
DB4    - PORTA bit 4
DB5    - PORTA bit 5
DB6    - PORTA bit 6
DB7    - PORTA bit 7

/RD    - PORTD bit 0
/WR    - PORTD bit 1
D/C    - PORTD bit 2
/CS    - PORTD bit 3
/RES   - PORTD bit 4
Vcc ON - PORTD bit 5 enables the 15V Vcc DC/DC converter

BS0    - GND
BS1    - +3.3V

Make sure to specify the correct connections in the
Project|Configure|C Compiler|Libraries|Graphic LCD menu.
*********************************************************************/

/* I/O Registers definitions */
#include <io.h>
#include <glcd.h>
#include <delay.h>

/* Gray scale table for the Densitron DD-12864YW-13A display
   Used for gray level display linearization
   Modify accordingly for your display, based on the datasheet */
flash unsigned char gray_scale_tbl[15]=
	  {0x01,0x0D,0x19,0x25,0x31,0x3D,0x49,0x55,
	  0x61,0x6D,0x79,0x85,0x9D,0xA9,0xB4};

void main(void)
{
GLCDINIT_t glcd_init_data;
signed char x;

/* Don't use any font */
glcd_init_data.font=NULL;
/* Don't use any external memory read function */
glcd_init_data.readxmem=NULL;
/* Don't use any external memory write function */
glcd_init_data.writexmem=NULL;
/* Set the Front Clock Divider */
glcd_init_data.divset=SSD1322_DEFAULT_DIVSET;
/* Set the Oscillator Frequency */
glcd_init_data.osc_freq=SSD1322_DEFAULT_OSC_FREQ;
/* Set the Phase 1 Period */
glcd_init_data.phase1=SSD1322_DEFAULT_PHASE1;
/* Set the Phase 2 Period */
glcd_init_data.phase2=SSD1322_DEFAULT_PHASE2;
/* Set the Pre-charge Voltage */
glcd_init_data.precharge_v=SSD1322_DEFAULT_PRECHARGE_V;
/* Set the VCOMH voltage */
glcd_init_data.vcomh=SSD1322_DEFAULT_VCOMH;
/* Set the horizontal segment offset of pixel with x=0
   for the Densitron DD-12864YW-13A display
   Modify accordingly for your display, based on the datasheet
   For most displays it should be 0 */
glcd_init_data.offset_x0=112;
/* The horizontal coordinates are reversed for the
   Densitron DD-12864YW-13A display
   Modify accordingly for your display, based on the datasheet */
glcd_init_data.reverse_x=SSD1322_REVX_REV;
/* The vertical coordinates are reversed for the
   Densitron DD-12864YW-13A display
   Modify accordingly for your display, based on the datasheet */
glcd_init_data.reverse_y=SSD1322_REVY_REV;
/* Horizontal COM connections are not splitted odd/even
   for the Densitron DD-12864YW-13A display
   Modify accordingly for your display, based on the datasheet */
glcd_init_data.com_split=SSD1322_COM_SPLIT_OFF;
/* The Densitron DD-12864YW-13A display uses Dual COM mode
   Modify accordingly for your display, based on the datasheet */
glcd_init_data.dual_com=SSD1322_DUAL_COM_ON;
/* Set the Second Pre-charge Period */
glcd_init_data.precharge_t2=SSD1322_DEFAULT_PRECHARGE_T2;
/* Set the Contrast Current */
glcd_init_data.contrast=SSD1322_DEFAULT_CONTRAST;
/* Set the Master Contrast Current */
glcd_init_data.master_contrast=SSD1322_DEFAULT_MASTER_CONTRAST;
/* The General Purpose Input/Outputs are not used */
glcd_init_data.gpio0=SSD1322_GPIO_HIZ;
glcd_init_data.gpio1=SSD1322_GPIO_HIZ;
/* Specify an user defined gray scale table for the
   Densitron DD-12864YW-13A display */
glcd_init_data.gray_scale_table=gray_scale_tbl;

/* Initialize the SSD1322 graphic controller */
glcd_init(&glcd_init_data);

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

while (1);
}
