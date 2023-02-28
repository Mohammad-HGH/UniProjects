/*********************************************************************
Solomon Systech SSD1322 Graphic controller demo
(C) Copyright 2011-2013 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

The demo is designed to run on an ATxmega128A1
XPlained development board with chip clock of 32 MHz.

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

This program exceeds the code size limit of the Evaluation
version of CodeVisionAVR.
In this case you can program the ATxmega128A1 FLASH with
the contents of the
\EXAMPLES\Graphic LCD\SSD1322\Full Demo\Release\Exe\Full Demo.hex
file.
*********************************************************************/

#include <io.h>
#include <stdio.h>
#include <glcd.h>
#include <delay.h>

/* Include some fonts needed by the demo */
#include <font5x7.h>
#include "arial14.h"
#include "arial_bold14.h"
#include "courier7x14.h"
#include "courier_bold14.h"
#include "script19.h"

/* Gray scale table for the Densitron DD-12864YW-13A display
   Used for gray level display linearization
   Modify accordingly for your display, based on the datasheet */
flash unsigned char gray_scale_tbl[15]=
	  {0x01,0x0D,0x19,0x25,0x31,0x3D,0x49,0x55,
	  0x61,0x6D,0x79,0x85,0x9D,0xA9,0xB4};

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

/* System Clocks initialization */
void system_clocks_init(void)
{
unsigned char n,s;

// Optimize for speed
#pragma optsize- 
// Save interrupts enabled/disabled state
s=SREG;
// Disable interrupts
#asm("cli")

// Internal 32 kHz RC oscillator initialization
// Enable the internal 32 kHz RC oscillator
OSC.CTRL|=OSC_RC32KEN_bm;
// Wait for the internal 32 kHz RC oscillator to stabilize
while ((OSC.STATUS & OSC_RC32KRDY_bm)==0);

// Internal 32 MHz RC oscillator initialization
// Enable the internal 32 MHz RC oscillator
OSC.CTRL|=OSC_RC32MEN_bm;

// System Clock prescaler A division factor: 1
// System Clock prescalers B & C division factors: B:1, C:1
// ClkPer4: 32000.000 kHz
// ClkPer2: 32000.000 kHz
// ClkPer:  32000.000 kHz
// ClkCPU:  32000.000 kHz
n=(CLK.PSCTRL & (~(CLK_PSADIV_gm | CLK_PSBCDIV1_bm | CLK_PSBCDIV0_bm))) |
    CLK_PSADIV_1_gc | CLK_PSBCDIV_1_1_gc;
CCP=CCP_IOREG_gc;
CLK.PSCTRL=n;

// Internal 32 MHz RC osc. calibration reference clock source: 32.768 kHz Internal Osc.
OSC.DFLLCTRL&= ~(OSC_RC32MCREF_bm | OSC_RC2MCREF_bm);
// Enable the autocalibration of the internal 32 MHz RC oscillator
DFLLRC32M.CTRL|=DFLL_ENABLE_bm;

// Wait for the internal 32 MHz RC oscillator to stabilize
while ((OSC.STATUS & OSC_RC32MRDY_bm)==0);

// Select the system clock source: 32 MHz Internal RC Osc.
n=(CLK.CTRL & (~CLK_SCLKSEL_gm)) | CLK_SCLKSEL_RC32M_gc;
CCP=CCP_IOREG_gc;
CLK.CTRL=n;

// Disable the unused oscillators: 2 MHz, external clock/crystal oscillator, PLL
OSC.CTRL&= ~(OSC_RC2MEN_bm | OSC_XOSCEN_bm | OSC_PLLEN_bm);

// Peripheral Clock output: Disabled
PORTCFG.CLKEVOUT=(PORTCFG.CLKEVOUT & (~PORTCFG_CLKOUT_gm)) | PORTCFG_CLKOUT_OFF_gc;

// Restore interrupts enabled/disabled state
SREG=s;
// Restore optimization for size if needed
#pragma optsize_default
}

void main()
{
unsigned char i;
GLCDINIT_t glcd_init_data;
char text[64];

system_clocks_init();

/* 5x7 font is used */
glcd_init_data.font=font5x7;
/* No need for reading data from external memory */
glcd_init_data.readxmem=NULL;
/* No need for reading data from external memory */ 
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

/* Display literal char string located in FLASH */
glcd_outtextf("SSD1322 Graphics Demo\nFont samples:");

/* Display some font samples */
glcd_outtextxyf(0,20,"Standard 5x7");

glcd_setfont(arial14);
glcd_outtextxyf(0,30,"Arial 14\n");

glcd_setfont(arial_bold14);
glcd_outtextf("Arial Bold 14");

/* 4 seconds delay */
delay_ms(4000);
/* Clear display */
glcd_clear();

glcd_setfont(courier7x14);
glcd_outtextf("Courier 7x14\n");

glcd_setfont(courier_bold14);
glcd_outtextf("Courier Bold 14");

glcd_setfont(script19);
glcd_outtextxyf(0,30,"Script 19");

/* 4 seconds delay */
delay_ms(4000);
/* Clear display */
glcd_clear();

glcd_setfont(font5x7);
glcd_outtextf("Var. text spacing:\n");
glcd_setfont(courier_bold14);
glcd_outtextf("SSD1322 Demo\n");
/* Set horizontal justification: 2 pixels and
   vertical justification: 1 pixel */
glcd_settextjustify(2,1);
glcd_outtextf("SSD1322 Demo\n");
/* Set horizontal justification: 3 pixels and
   vertical justification: 1 pixel */
glcd_settextjustify(3,1);
glcd_outtextf("SSD1322 Demo");

/* 4 seconds delay */
delay_ms(4000);
/* Clear display */
glcd_clear();

/* Set horizontal justification: 1 pixels and
   vertical justification: 1 pixel */
glcd_settextjustify(1,1);
glcd_setfont(font5x7);
glcd_outtextf("Some line styles:");

/* Draw various styles of lines */
/* Line thickness: 1 pixel */
glcd_line(0,10,127,10);

glcd_setlinestyle(1,GLCD_LINE_DOT_SMALL);
glcd_line(0,15,127,15);

glcd_setlinestyle(1,GLCD_LINE_DOT_LARGE);
glcd_line(0,20,127,20);

/* Line thickness: 2 pixels */
glcd_setlinestyle(2,GLCD_LINE_SOLID);
glcd_line(0,25,127,25);

glcd_setlinestyle(2,GLCD_LINE_DOT_SMALL);
glcd_line(0,30,127,30);

glcd_setlinestyle(2,GLCD_LINE_DOT_LARGE);
glcd_line(0,35,127,35);

/* Line thickness: 4 pixels */
glcd_setlinestyle(4,GLCD_LINE_SOLID);
glcd_line(0,40,127,40);

glcd_setlinestyle(4,GLCD_LINE_DOT_SMALL);
glcd_line(0,45,127,45);

glcd_setlinestyle(4,GLCD_LINE_DOT_LARGE);
glcd_line(0,50,127,50);

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
glcd_floodfill(48,24,1);

/* Write a text inside the filled hexagon in
   reversed colors */
glcd_setcolor(GLCD_CL_BLACK);
glcd_setbkcolor(GLCD_CL_WHITE);
glcd_outtextxyf(30,17,"Fill");

/* Save hexagon image in EEPROM */
glcd_getimagee(22,0,37,41,buffer);
/* Restore the default foreground
   and background colors */
glcd_setcolor(GLCD_CL_WHITE);
glcd_setbkcolor(GLCD_CL_BLACK);
glcd_outtextxyf(0,47,"Image was saved to\nEEPROM");

/* 3 seconds delay */
delay_ms(3000);
/* Clear display */
glcd_clear();

/* Draw a circle with line thickness: 1 pixel */
glcd_circle(21,23,20);

/* Draw an arc of a circle with line thickness: 1 pixel */
glcd_arc(21,23,0,120,15);

/* Draw a circle with line thickness: 3 pixels */
glcd_setlinestyle(3,GLCD_LINE_SOLID);
glcd_circle(63,23,20);

/* Draw an arc of a circle with line thickness: 3 pixels */
glcd_arc(63,23,0,210,15);

glcd_outtextxyf(0,47,"Circles & arcs");

/* 3 seconds delay */
delay_ms(3000);
/* Clear display */
glcd_clear();

/* Draw a rectangle with line thickness: 1 pixel 
   and solid line */
glcd_setlinestyle(1,GLCD_LINE_SOLID);
glcd_rectrel(0,0,40,25);

/* Draw a rectangle with line thickness: 3 pixel
   and dotted line */
glcd_setlinestyle(3,GLCD_LINE_DOT_LARGE);
glcd_rectrel(42,0,40,25);

/* Draw a bar filled with a pattern */
glcd_setfillf(small_dots,GLCD_CL_DARK_GRAY);
glcd_bar(14,29,70,44);

glcd_outtextxyf(0,47,"Rectangles &\nfilled bar");

/* 3 seconds delay */
delay_ms(3000);
/* Clear display */
glcd_clear();

/* Draw rounded rectangle */
glcd_rectround(5,5,74,38,8);
glcd_outtextxyf(16,16,"Rounded");
glcd_outtextxyf(16,24,"rectangle");

/* 3 seconds delay */
delay_ms(3000);
/* Clear display */
glcd_clear();

/* Set line thickness: 1 pixel */
glcd_setlinestyle(1,GLCD_LINE_SOLID);

/* Draw filled pie slice */
glcd_outtextf("Pie\nslice:");
glcd_pieslice(60,23,30,300,20);

/* 3 seconds delay */
delay_ms(3000);
/* Clear display */
glcd_clear();

glcd_outtextf("Put\nimage\nfrom\nEEPROM:");
/* Display image saved in EEPROM */
glcd_putimagee(56,0,buffer,GLCD_PUTCOPY);

/* 1 second delay */
delay_ms(1000);

/* Contrast control example */
for (i=0; i<=127; i++)
    {
    sprintf(text,"Display contrast\ncontrol: %d",i);
    glcd_outtextxy(0,46,text);
    ssd1322_setcontrast(i);
    delay_ms(50); /* 50ms delay */
    }

/* 1 second delay */
delay_ms(1000);

/* Power-save mode test */
glcd_outtextxyf(0,46,"Entering power-save\nmode...     ");

/* 1 second delay */
delay_ms(1000);

/* Turn display off and enter power-save mode */
glcd_display(false);

/* 2 seconds delay */
delay_ms(2000);

/* Exit power-save mode and turn display on */
glcd_display(true);
glcd_outtextxyf(0,46,"Exit power-save mode\n           ");

}
