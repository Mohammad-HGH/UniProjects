/**********************************************************
Solomon Systech SSD1963 Color TFT LCD controller demo
(C) Copyright 2011-2012 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

This program can't be compiled with the Evaluation or
Standard versions of CodeVisionAVR, because the glcd_ssd1963.lib
file is supplied only with the Advanced license.
In this case you can program the ATxmega128A1 FLASH with
the contents of the
\EXAMPLES\Graphic LCD\SSD1963\Exe\demo_tft43.hex
file.

The demo is designed to run on the Atmel Xplain
development board with an ATxmega128A1
chip using the internal 32 MHz oscillator
as clock source.

A large portion of the program (51712 bytes)
consists of the uncompressed 186x139 64k color
image from the file image.c
Due to the limited number of I/O pins of the Xplain
board, there was no possibility to connect a SD CARD
and store this image on it, so for the purpose of this demo,
it was located in chip's FLASH memory.
The image was left uncompressed in order to allow fast loading.

In this demo the SSD1963 controller operates in 
8 bit interface mode.
For better performance the 16 bit operating mode can
be selected by accessing the
Project|Configure|C Compiler|Libraries|Graphic LCD menu.
In this situation the DB8-DB15 signals must be also
connected to one of the ATxmega128A1 ports.   

A 480x272 4.3" TFT LCD module is used for
the purpose of this demo.
It can be replaced with other similar modules with
the SSD1963 controller. 

The TFT LCD module connections are:
DB0  - PORTA bit 0
DB1  - PORTA bit 1
DB2  - PORTA bit 2
DB3  - PORTA bit 3
DB4  - PORTA bit 4
DB5  - PORTA bit 5
DB6  - PORTA bit 6
DB7  - PORTA bit 7

DB8  - not connected
DB9  - not connected
DB10 - not connected
DB11 - not connected
DB12 - not connected
DB13 - not connected
DB14 - not connected
DB15 - not connected

/CS  - PORTD bit 0
D/C  - PORTD bit 1
/RD  - PORTD bit 2
/WR  - PORTD bit 3
/RST - PORTD bit 4

Backlight PWM - PORTD bit 5

GND   - GND
+3.3V - +3.3V
+5V   - +5V

The connections can be modified by accessing the
Project|Configure|C Compiler|Libraries|Graphic LCD menu.
**********************************************************/

#include <io.h>
#include <glcd.h>
#include <delay.h>
#include <iobits.h>

/* Meter functions */
#include "meter.h"
/* Bar-graph gauge functions */
#include "bargraph.h"
/* Button functions */
#include "button.h"
/* Glyphs for buttons */
#include "ok_glyph.h"
#include "cancel_glyph.h"

/* Include some fonts needed by the demo */
#include <font5x7.h>
#include "arial14.h"
#include "arial_bold14.h"
#include "courier7x14.h"
#include "courier_bold14.h"
#include "la_bamba17.h"

/* Image to be displayed */
#include "image.h"

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

flash char * flash button_state[2]=
      {"Buttons not pressed",
       "Buttons pressed     "};

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
GLCDINIT_t tft_init_data;
unsigned char i;
int x,y;
meter_t meter;
bargraph_t bargraph;
button_t button_ok,button_cancel;

/* Set the ATxmega128A1 to run from the internal 32MHz oscillator */
system_clocks_init();

/* Turn on backlight */
SETBIT(PORTD.DIR,5);
SETBIT(PORTD.OUT,5);

#asm("sei")

/* LCD controller initialization data */
/* Set default font 5x7 */
tft_init_data.font=font5x7;
/* No external mem. read function */ 
tft_init_data.readxmem=NULL;
/* No external mem. write function */ 
tft_init_data.writexmem=NULL;
/* SSD1963 crystal [kHz] */ 
tft_init_data.ctrl_clk=10000; 
/* 4.3" 480x272 TFT display parameters from the datasheet */
/* TFT pixel clock [kHz] */
tft_init_data.tft_pixel_clk=9000; 
tft_init_data.hpulse_width=41;
tft_init_data.hfront_porch=2;
tft_init_data.hback_porch=2;
tft_init_data.vpulse_width=10;
tft_init_data.vfront_porch=2;
tft_init_data.vback_porch=2;
/* 18 bit panel data width */
tft_init_data.tft24bit=SSD1963_TFT_DATA_WIDTH18;
/* dot clock pulse polarity */
tft_init_data.lshift=SSD1963_DEFAULT_LSHIFT;
/* Normal display, no horizontal reverse */ 
tft_init_data.reverse_x=SSD1963_REVX_NORM;
/* Normal display, no vertical reverse */ 
tft_init_data.reverse_y=SSD1963_REVY_NORM;
/* Pixel order: RGB */
tft_init_data.cl_bits_order=SSD1963_DEFAULT_CL_BITS; 

/* Initialize the LCD controller */
glcd_init(&tft_init_data);

/* Display literal char string located in FLASH */
glcd_outtextf("Solomon Systech SSD1963 Graphics Demo\n"
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
glcd_outtextf("SSD1963 TFT Controller\n");
/* Set horizontal justification: 2 pixels and
   vertical justification: 1 pixel */
glcd_settextjustify(2,1);
glcd_outtextf("SSD1963 TFT Controller\n");
/* Set horizontal justification: 3 pixels and
   vertical justification: 1 pixel */
glcd_settextjustify(3,1);
glcd_outtextf("SSD1963 TFT Controller");

/* Set horizontal justification: 1 pixels and
   vertical justification: 1 pixel */
glcd_settextjustify(1,1);
glcd_setfont(arial_bold14);
glcd_setcolor(GLCD_CL_CYAN);
glcd_outtextxyf(0,190,"Line styles:");

/* Draw various styles of lines */
glcd_setcolor(GLCD_CL_LIME);
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
glcd_outtextxyf(240,0,"Coloured lines:");

/* Draw some coloured lines */
glcd_setlinestyle(1,GLCD_LINE_SOLID);
i=1; /* Don't use black color for lines */
for (x=244; x<480; x+=4)
    {
    glcd_setcolor(colors[i++].col);
    if (i>=(sizeof(colors)/sizeof(color_data))) i=1;
    glcd_line(244,20,x,271);
    }

i=1;
for (x=479; x>=244; x-=4) 
    {
    glcd_setcolor(colors[i++].col);
    if (i>=(sizeof(colors)/sizeof(color_data))) i=1;
    glcd_line(479,271,x,20);
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

glcd_outtextxyf(0,170,"Display\nan image\nfrom\nRAM:");

/* Display image saved in RAM
   without transparency */
glcd_putimage(50,170,buffer,GLCD_PUTCOPY);

glcd_outtextxyf(0,215,"Non-transparent mode");

/* Set transparent color */
glcd_settpcolor(GLCD_CL_RED);

/* Display image saved in RAM
   with transparency */
glcd_putimage(150,170,buffer,GLCD_PUTTP);

glcd_outtextxyf(130,215,"Transparent mode");

glcd_setfont(arial_bold14);
glcd_setcolor(GLCD_CL_CYAN);

glcd_outtextxyf(265,100,"Display an image from FLASH:");

/* Display image stored in FLASH */
glcd_putimagef(272,120,img,GLCD_PUTCOPY);

/* Set fill pattern and color */
glcd_setfillf(solid_fill,GLCD_CL_WHITE);

glcd_outtextxyf(270,0,"Meter & Bar-graph gauges:");

/* Display a meter gauge */
meter.xc=300;
meter.yc=305-225;
meter.radius=50;
meter.min_value=0;
meter.max_value=100;
meter.ticks=11;

meter_init(&meter);

/* Display a bar-graph gauge */
bargraph.xt=373;
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
    
/* 2 seconds delay */
delay_ms(2000);

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

/* 2 seconds delay */
delay_ms(2000);

glcd_transparent(false);

glcd_setfont(arial_bold14);
glcd_setcolor(GLCD_CL_CYAN);
glcd_outtextxyf(250,0,"Entering sleep mode...");

/* 2 seconds delay */
delay_ms(2000);

/* Turn off backlight */
CLRBIT(PORTD.OUT,5);

/* Enter sleep mode */
ssd1963_sleep(true);

/* 4 seconds delay */
delay_ms(4000);

/* Exit from sleep mode */
ssd1963_sleep(false);

/* Turn on backlight */
SETBIT(PORTD.OUT,5);

glcd_outtextxyf(250,0,"Exit from sleep mode  ");

/* 2 seconds delay */
delay_ms(2000);

glcd_outtextxyf(250,0,"Button examples:     ");

/* Create OK button */
button_ok.left=250;
button_ok.top=30;
button_ok.width=80;
button_ok.height=30;
button_ok.border_color=GLCD_CL_BLUE;
button_ok.bg_color=GLCD_CL_LIGHT_GRAY;
button_ok.pressed_color=GLCD_CL_WHITE;
button_ok.text_color=GLCD_CL_BLACK;
button_ok.glyph_tp_color=GLCD_CL_WHITE;
button_ok.glyph=ok_glyph;
button_ok.caption="OK";
button_ok.font=arial_bold14;

button_init(&button_ok);

/* Create Cancel button */
/* Most of the parameters are the same for both buttons, */
button_cancel=button_ok;
/* except left coordinate, */ 
button_cancel.left=340;
/* glyph */
button_cancel.glyph=cancel_glyph;
/* and caption */
button_cancel.caption="Cancel";

button_init(&button_cancel);

/* Toggle buttons */
while (1)
      {
      button_ok.pressed=!button_ok.pressed;
      button_update(&button_ok);
      
      button_cancel.pressed=button_ok.pressed;
      button_update(&button_cancel);

      glcd_setcolor(GLCD_CL_CYAN);
      glcd_outtextxyf(250,75,button_state[button_ok.pressed]);
      delay_ms(500);
      }
}
