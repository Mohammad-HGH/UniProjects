/**********************************************************
Solomon Systech SSD1289 Color TFT LCD controller demo
(C) Copyright 2011-2012 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

This program can't be compiled with the Evaluation or
Standard versions of CodeVisionAVR, because the glcd_ssd1289.lib
file is supplied only with the Advanced license.
In this case you can program the ATxmega128A1 FLASH with
the contents of the
\EXAMPLES\Graphic LCD\SSD1289\Exe\demo_hy32d.hex
file.

The demo is designed to run on the STK600
development board with an ATxmega128A1
chip using the internal 32 MHz oscillator
as clock source.

A 240x320 HY32D 3.2" TFT LCD module is used for
the purpose of this demo.
It can be replaced with other similar modules with
the SSD1289 controller. 

The TFT LCD module connections are:
DB0  - PORTE bit 0
DB1  - PORTE bit 1
DB2  - PORTE bit 2
DB3  - PORTE bit 3
DB4  - PORTE bit 4
DB5  - PORTE bit 5
DB6  - PORTE bit 6
DB7  - PORTE bit 7

DB8  - PORTC bit 0
DB9  - PORTC bit 1
DB10 - PORTC bit 2
DB11 - PORTC bit 3
DB12 - PORTC bit 4
DB13 - PORTC bit 5
DB14 - PORTC bit 6
DB15 - PORTC bit 7

/WR  - PORTD bit 3
/RD  - PORTD bit 2
D/C  - PORTD bit 1
/RST - PORTD bit 4
/CS  - PORTD bit 0

Backlight connections:
BLGND - GND
BLVDD - +3.3V

Power supply connections:
GND - GND
+5V - +5V

If you are using a different LCD module than HY32D,
please use the connections from the module datasheet.

The connections can be modified by accessing the
Project|Configure|C Compiler|Libraries|Graphic LCD menu.
**********************************************************/

#include <io.h>
#include <glcd.h>
#include <delay.h>

/* Meter functions */
#include "meter.h"
/* Bar-graph gauge functions */
#include "bargraph.h"

/* Include some fonts needed by the demo */
#include <font5x7.h>
#include "arial14.h"
#include "arial_bold14.h"
#include "courier7x14.h"
#include "courier_bold14.h"
#include "la_bamba17.h"

/* Small dots filling pattern */
flash unsigned char small_dots[]={0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55};

/* Solid filling pattern */
flash unsigned char solid_fill[]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

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

typedef struct 
      {
      flash char *name;
      GLCDCOL_t col;
      } color_data;
      
flash color_data colors[]=
{
"Black",GLCD_CL_BLACK, 
"White",GLCD_CL_WHITE, 
"Gray",GLCD_CL_GRAY,
"Light Gray",GLCD_CL_LIGHT_GRAY,     
"Green",GLCD_CL_GREEN,      
"Lime",GLCD_CL_LIME, 
"Blue",GLCD_CL_BLUE, 
"Red",GLCD_CL_RED, 
"Aqua",GLCD_CL_AQUA, 
"Yellow",GLCD_CL_YELLOW, 
"Magenta",GLCD_CL_MAGENTA,
"Cyan",GLCD_CL_CYAN,
"Dark Cyan",GLCD_CL_DARK_CYAN,      
"Orange",GLCD_CL_ORANGE, 
"Pink",GLCD_CL_PINK,
"Brown",GLCD_CL_BROWN,
"Violet",GLCD_CL_VIOLET,
"Silver",GLCD_CL_SILVER,
"Gold",GLCD_CL_GOLD,
"Navy",GLCD_CL_NAVY,     
"Maroon",GLCD_CL_MAROON,      
"Purple",GLCD_CL_PURPLE,    
"Olive",GLCD_CL_OLIVE     
};

/* Buffer used for image storage */
unsigned char buffer[3100];

// System Clocks initialization
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
GLCDINIT_t glcd_init_data;
unsigned char i;
int x,y;
meter_t meter;
bargraph_t bargraph;

/* Set the ATxmega128A1 to run from the internal 32MHz oscillator */
system_clocks_init();

/* Initialize the LCD controller with the default values
   from glcd_ssd1289.h */

/* Specify the current font for displaying text */
glcd_init_data.font=font5x7;
/* No function is used for reading image data from external memory */
glcd_init_data.readxmem=NULL;
/* No function is used for writing image data to external memory */
glcd_init_data.writexmem=NULL;
/* Horizontal reverse for HY32D */
glcd_init_data.reverse_x=SSD1289_REVX_REV;
/* Normal display, no vertical reverse */
glcd_init_data.reverse_y=SSD1289_REVY_NORM;
/* Color bit writing order: BGR for HY32D ! */
glcd_init_data.cl_bits_order=SSD1289_CL_BITS_BGR;
/* Power control 1 BT0..BT2 step-up factor of the step-up circuit */
glcd_init_data.stepup_factor=SSD1289_DEFAULT_STEPUP_FACTOR;
/* Power control 1 DC0..DC3, DCT0..DCT3 step-up circuit cycle */
glcd_init_data.stepup_cycle=SSD1289_DEFAULT_STEPUP_CYCLE;
/* Power control 1 AP0..AP2 adjusts the amount of current
   from the constant current source in the
   internal op. amplifier circuit */
glcd_init_data.crt_source=SSD1289_DEFAULT_CRT_SOURCE;
/* Default value for VCIX2 voltage */
glcd_init_data.vcix2=SSD1289_DEFAULT_VCIX2;
/* Default value for VLCD63 voltage */
glcd_init_data.vlcd63=SSD1289_DEFAULT_VLCD63;
// Default value for VcomL alternating drive voltage
glcd_init_data.vcoml=SSD1289_DEFAULT_VCOML;
/* Default value for VcomH voltage */
glcd_init_data.vcomh=SSD1289_DEFAULT_VCOMH;
/* Frame frequency */
glcd_init_data.frame_freq=SSD1289_DEFAULT_FRAME_FREQ;
/* PKP00..PKP02 positive gamma micro adj. */
glcd_init_data.pkp00=SSD1289_DEFAULT_PKP00;
/* PKP10..PKP12 positive gamma micro adj. */
glcd_init_data.pkp10=SSD1289_DEFAULT_PKP10;
/* PKP20..PKP22 positive gamma micro adj. */
glcd_init_data.pkp20=SSD1289_DEFAULT_PKP20;
/* PKP30..PKP32 positive gamma micro adj. */
glcd_init_data.pkp30=SSD1289_DEFAULT_PKP30;
/* PKP40..PKP42 positive gamma micro adj. */
glcd_init_data.pkp40=SSD1289_DEFAULT_PKP40;
/* PKP50..PKP52 positive gamma micro adj. */
glcd_init_data.pkp50=SSD1289_DEFAULT_PKP50;
/* PRP00..PRP02 positive gamma gradient adj. */
glcd_init_data.prp00=SSD1289_DEFAULT_PRP00;
/* PRP10..PRP12 positive gamma gradient adj. */
glcd_init_data.prp10=SSD1289_DEFAULT_PRP10;
/* VRP00..VRP03 positive gamma amplification adj. */
glcd_init_data.vrp00=SSD1289_DEFAULT_VRP00;
/* VRP10..VRP14 positive gamma amplification adj. */
glcd_init_data.vrp10=SSD1289_DEFAULT_VRP10;
/* PKN00..PKN02 negative gamma micro adj. */
glcd_init_data.pkn00=SSD1289_DEFAULT_PKN00;
/* PKN10..PKN12 negative gamma micro adj. */
glcd_init_data.pkn10=SSD1289_DEFAULT_PKN10;
/* PKN20..PKN22 positive gamma micro adj. */
glcd_init_data.pkn20=SSD1289_DEFAULT_PKN20;
/* PKN30..PKN32 positive gamma micro adj. */
glcd_init_data.pkn30=SSD1289_DEFAULT_PKN30;
/* PKN40..PKN42 negative gamma micro adj. */
glcd_init_data.pkn40=SSD1289_DEFAULT_PKN40;
/* PKN50..PKN52 negative gamma micro adj. */
glcd_init_data.pkn50=SSD1289_DEFAULT_PKN50;
/* PRN00..PRN02 negative gamma gradient adj. */
glcd_init_data.prn00=SSD1289_DEFAULT_PRN00;
/* PRN10..PRN12 negative gamma gradient adj. */
glcd_init_data.prn10=SSD1289_DEFAULT_PRN10;
/* VRN00..VRN03 negative gamma amplification adj. */
glcd_init_data.vrn00=SSD1289_DEFAULT_VRN00;
/* VRN10..VRN14 negative gamma amplification adj. */
glcd_init_data.vrn10=SSD1289_DEFAULT_VRN10;

glcd_init(&glcd_init_data);

/* Use transparent displaying mode for text */
glcd_transparent(1);

/* Display literal char string located in FLASH */
glcd_outtextf("Solomon Systech SSD1289 Graphics Demo\n"
              "Here are some font samples:");

/* Display some font samples */
glcd_setcolor(GLCD_CL_YELLOW);
glcd_outtextxyf(0,20,"Standard, 5x7 pixels");

glcd_setfont(arial14);
glcd_setcolor(GLCD_CL_RED);
glcd_outtextxyf(0,30,"Arial, 14 pixels height\n");

glcd_setfont(arial_bold14);
glcd_setcolor(GLCD_CL_MAGENTA);
glcd_outtextf("Arial Bold, 14 pixels height\n");

glcd_setfont(courier7x14);
glcd_setcolor(GLCD_CL_GREEN);
glcd_outtextf("Courier, 7x14 pixels\n");

glcd_setfont(courier_bold14);
glcd_setcolor(GLCD_CL_LIME);
glcd_outtextf("Courier Bold,14 pixels height\n");

glcd_setfont(la_bamba17);
glcd_setcolor(GLCD_CL_AQUA);
glcd_outtextf("La Bamba,17 pixels height\n");

glcd_setfont(arial_bold14);
glcd_setcolor(GLCD_CL_CYAN);
glcd_outtextf("\nVariable text spacing:\n");

glcd_setfont(courier_bold14);
glcd_setcolor(GLCD_CL_YELLOW);
glcd_outtextf("SSD1289 TFT Controller\n");
/* Set horizontal justification: 2 pixels and
   vertical justification: 1 pixel */
glcd_settextjustify(2,1);
glcd_outtextf("SSD1289 TFT Controller\n");
/* Set horizontal justification: 3 pixels and
   vertical justification: 1 pixel */
glcd_settextjustify(3,1);
glcd_outtextf("SSD1289 TFT Controller");

/* Set horizontal justification: 1 pixels and
   vertical justification: 1 pixel */
glcd_settextjustify(1,1);
glcd_setfont(arial_bold14);
glcd_setcolor(GLCD_CL_CYAN);
glcd_outtextxyf(0,190,"Line styles:");

/* Draw various styles of lines */
glcd_setcolor(GLCD_CL_ORANGE);
/* Line thickness: 1 pixel */
glcd_line(0,210,239,210);

glcd_setlinestyle(1,GLCD_LINE_DOT_SMALL);
glcd_line(0,215,239,215);

glcd_setlinestyle(1,GLCD_LINE_DOT_LARGE);
glcd_line(0,220,239,220);

/* Line thickness: 2 pixels */
glcd_setlinestyle(2,GLCD_LINE_SOLID);
glcd_line(0,225,239,225);

glcd_setlinestyle(2,GLCD_LINE_DOT_SMALL);
glcd_line(0,230,239,230);

glcd_setlinestyle(2,GLCD_LINE_DOT_LARGE);
glcd_line(0,235,239,235);

/* Line thickness: 4 pixels */
glcd_setlinestyle(4,GLCD_LINE_SOLID);
glcd_line(0,240,239,240);

glcd_setlinestyle(4,GLCD_LINE_DOT_SMALL);
glcd_line(0,245,239,245);

glcd_setlinestyle(4,GLCD_LINE_DOT_LARGE);
glcd_line(0,250,239,250);

glcd_setcolor(GLCD_CL_CYAN);
glcd_outtextxyf(0,260,"Coloured lines:");

/* Draw some coloured lines */
glcd_setlinestyle(1,GLCD_LINE_SOLID);
i=1; /* Don't use black color for lines */
for (x=0; x<240; x+=4)
    {
    glcd_setcolor(colors[i++].col);
    if (i>=(sizeof(colors)/sizeof(color_data))) i=1;
    glcd_line(0,275,x,319);
    }

i=1;
for (x=239; x>=0; x-=4) 
    {
    glcd_setcolor(colors[i++].col);
    if (i>=(sizeof(colors)/sizeof(color_data))) i=1;
    glcd_line(239,319,x,275);
    }

/* 5 seconds delay */
delay_ms(5000);
/* Clear display */
glcd_clear();

/* Draw a hexagon with line thickness: 1 pixel */
glcd_setcolor(GLCD_CL_YELLOW);
glcd_setlinestyle(1,GLCD_LINE_SOLID);
glcd_drawpoly(7,hexagon);

/* Write a text inside the hexagon */
glcd_setfont(font5x7);
glcd_outtextxyf(30,17,"Fill");

/* Fill the hexagon with solid red color fill */
glcd_setfillcolor(GLCD_CL_RED);
glcd_floodfill(48,24,GLCD_CL_YELLOW /* border color */);

/* Save hexagon image in RAM */
glcd_getimage(22,0,37,41,buffer);

glcd_setfont(font5x7);
glcd_outtextxyf(0,47,"Image was saved to RAM");

/* Draw a circle with line thickness: 1 pixel */
glcd_circle(21,83,20);

/* Draw an arc of a circle with line thickness: 1 pixel */
glcd_setcolor(GLCD_CL_RED);
glcd_arc(21,83,0,120,15);

/* Draw a circle with line thickness: 3 pixels */
glcd_setlinestyle(3,GLCD_LINE_SOLID);
glcd_setcolor(GLCD_CL_LIME);
glcd_circle(63,83,20);

/* Draw an arc of a circle with line thickness: 3 pixels */
glcd_setcolor(GLCD_CL_MAGENTA);
glcd_arc(63,83,0,210,15);

glcd_setcolor(GLCD_CL_YELLOW);
glcd_outtextxyf(0,107,"Circles & arcs");

/* Draw a rectangle with line thickness: 1 pixel 
   and solid line */
glcd_setcolor(GLCD_CL_RED);
glcd_setlinestyle(1,GLCD_LINE_SOLID);
glcd_rectrel(100,60,40,25);

/* Draw a rectangle with line thickness: 3 pixel
   and dotted line */
glcd_setcolor(GLCD_CL_AQUA);
glcd_setlinestyle(3,GLCD_LINE_DOT_LARGE);
glcd_rectrel(142,60,40,25);

/* Draw a bar filled with a pattern of green color */
glcd_setfillf(small_dots,GLCD_CL_GREEN);
glcd_bar(114,89,170,104);

glcd_setcolor(GLCD_CL_YELLOW);
glcd_outtextxyf(100,107,"Rectangles & filled bar");

/* Draw rounded rectangle */
glcd_setcolor(GLCD_CL_RED);
glcd_rectround(5,125,74,38,8);

glcd_setcolor(GLCD_CL_YELLOW);
glcd_outtextxyf(16,136,"Rounded");
glcd_outtextxyf(16,144,"rectangle");

/* Set line thickness: 1 pixel */
glcd_setlinestyle(1,GLCD_LINE_SOLID);

glcd_setcolor(GLCD_CL_YELLOW);
glcd_outtextxyf(100,120,"Pie");
glcd_outtextxyf(100,127,"slice:");

/* Draw a pie slice filled with blue color */
glcd_setfillcolor(GLCD_CL_BLUE);
glcd_pieslice(170,143,30,300,20);

glcd_outtextxyf(0,170,"Put\nimage\nfrom\nRAM:");

/* Display image saved in RAM
   without transparency */
glcd_putimage(46,170,buffer,GLCD_PUTCOPY);

glcd_outtextxyf(0,215,"Non-transparent mode");

/* Set transparent color */
glcd_settpcolor(GLCD_CL_RED);

/* Display image saved in RAM
   with transparency */
glcd_putimage(150,170,buffer,GLCD_PUTTP);

glcd_outtextxyf(130,215,"Transparent mode");

/* Set fill pattern and color */
glcd_setfillf(solid_fill,GLCD_CL_WHITE);

glcd_setfont(arial_bold14);
glcd_setcolor(GLCD_CL_CYAN);
glcd_outtextxyf(30,225,"Meter & Bar-graph gauges:");

/* Display a meter gauge */
meter.xc=60;
meter.yc=305;
meter.radius=50;
meter.min_value=0;
meter.max_value=100;
meter.ticks=11;

meter_init(&meter);

/* Display a bar-graph gauge */
bargraph.xt=133;
bargraph.yt=meter.yc-50;
bargraph.width=90;
bargraph.height=40;
bargraph.min_value=meter.min_value;
bargraph.max_value=meter.max_value;
bargraph.ticks=meter.ticks;

bargraph_init(&bargraph);

for (i=0; i<4; i++)
    {
    for (x=meter.min_value; x<=meter.max_value; x+=5)
        {
        meter_update(&meter,x);
        bargraph_update(&bargraph,x);
        delay_ms(100);
        }
    for (x=meter.max_value; x>=meter.min_value; x-=5)
        {
        meter_update(&meter,x);
        bargraph_update(&bargraph,x);
        delay_ms(100);
        }
    }
    
/* 5 seconds delay */
delay_ms(5000);
/* Clear display */
glcd_clear();

glcd_setfont(arial_bold14);
glcd_setcolor(GLCD_CL_CYAN);
glcd_outtextf("Predefined colors:");

/* Display the predefined colors */

/* Set fill pattern and color */
glcd_setfillf(solid_fill,GLCD_CL_WHITE);

glcd_setfont(font5x7);
glcd_setcolor(GLCD_CL_WHITE);

for (i=0; i<(sizeof(colors)/sizeof(color_data));i++)
    {
    glcd_setfillcolor(colors[i].col);
    x=(i%2)*120;
    y=(i/2)*20+20;
    glcd_outtextxyf(x,y+6,colors[i].name);
    glcd_barrel(x+65,y,45,19);
    };

/* Stop here */
while (1);
}
