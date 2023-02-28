/**********************************************************
Solomon Systech SSD1963 Color TFT LCD controller demo.
Display an image stored in a binary file on a SD card.

(C) Copyright 2011-2013 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

The image was imported from a JPG PC file and
converted to a binary format, compatible with the
CodeVisionAVR graphic library, using the LCD Vision software.

The converted "image.bin" file from the project directory
must be copied to the SD card, so that it can be read
by this example program. 

This program can't be compiled with the Evaluation or
Standard versions of CodeVisionAVR, because the glcd_ssd1963.lib
file is supplied only with the Advanced license.
In this case you can program the ATxmega128A1 FLASH with
the contents of the
\EXAMPLES ATXmega\Graphic LCD\Display Image from SD Card\SSD1963\Exe\Display image from SD card.hex
file.

The demo is designed to run on the STK600
development board with an ATxmega128A1
chip using the internal 32 MHz oscillator
as clock source.

A Micro SD card socket is connected to the development board
as outlined below:

Card Socket
/CS ------------ PORTF bit 4
SI  ------------ PORTF bit 5 MOSI
SO  ------------ PORTF bit 6 MISO
SCK ------------ PORTF bit 7 SCK
GND ------------ GND
+3.3V ---------- +3.3V

The /CD and WP signals are disabled in the project configuration,
because a Micro SD card socket without these signals was used.

For other I/O port connections, you need to make the appropriate changes in the
"Project|Configure|C Compiler|Libraries|MMC/SD/SD HC Card and FAT Support"
menu.

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
/* FAT on MMC/SD/SD HC card support */
#include <ff.h>
/* Graphic LCD functions */
#include <glcd.h>
/* Include the font needed by the demo */
#include <font5x7.h>
/* Initialization functions */
#include "init.h"

/* FAT function result */
FRESULT res;
/* Will hold the information for logical drive 0: */
FATFS fat;
/* Will hold the file information */
FIL file;
/* Image file path */
char path[]="0:/image.bin";

/* error message list */
flash char * flash error_msg[]=
{
"", /* not used */
"FR_DISK_ERR",
"FR_INT_ERR",
"FR_NOT_READY",
"FR_NO_FILE",
"FR_NO_PATH",
"FR_INVALID_NAME",
"FR_DENIED",
"FR_EXIST",
"FR_INVALID_OBJECT",
"FR_WRITE_PROTECTED",
"FR_INVALID_DRIVE",
"FR_NOT_ENABLED",
"FR_NO_FILESYSTEM",
"FR_MKFS_ABORTED",
"FR_TIMEOUT"
};

/* Timer/counter TCC0 Overflow interrupt service routine
   Occurs every 10ms */
interrupt [TCC0_OVF_vect] void tcc0_overflow_isr(void)
{
/* Card access low level timing function */
disk_timerproc();
}

/* display error message and stop */
void error(FRESULT res)
{
if ((res>=FR_DISK_ERR) && (res<=FR_TIMEOUT))
   {
   glcd_outtextf("ERROR: ");
   glcd_outtextf(error_msg[res]);
   }
/* stop here */
while(1);
}

#define BUFFER_SIZE 512 /* Buffer size must be a multiple of 2 */
#define PAGE_MASK (0xffffffffUL-BUFFER_SIZE+1)
#define BUFFER_ADDR_MASK (BUFFER_SIZE-1)

unsigned long data_buffer_page; /* Current data page for the buffer contents */
unsigned int data_buffer_bytes; /* # of bytes in the data buffer */
unsigned char data_buffer[BUFFER_SIZE];

/* Function used for reading image data
   from external memory: SD card 
   It is called by the graphic library function
   'glcd_putimagex' using the 'glcd_init_data.readxmem'
   pointer */
unsigned char read_sd(GLCDMEMADDR_t addr)
{
unsigned int buffer_index;
unsigned long page=addr & PAGE_MASK; /* Get the required page */
if ((data_buffer_bytes==0) || (data_buffer_page!=page))
   {
   /* The required data is not in the buffer,
      we must read it from the SD card */
      
   /* Position the file read pointer to the
      required page file position */
   f_lseek(&file,page);

   /* Read BUFFER_SIZE bytes of data from the file */
   if (res=f_read(&file,data_buffer,BUFFER_SIZE,&data_buffer_bytes)!=FR_OK)
      /* if an error occured, display it and stop */
      error(res);
   /* Remember new buffer page value */
   data_buffer_page=page;
   }

buffer_index=addr & BUFFER_ADDR_MASK;
if (buffer_index>=data_buffer_bytes)
   {
   glcd_outtextf("ERROR: Buffer underflow");
   while (1);
   } 
return data_buffer[buffer_index];
}

void main()
{

GLCDY_t y;
GLCDINIT_t glcd_init_data;

/* Interrupt system initialization */
interrupt_init();

/* Set the ATxmega128A1 to run from the internal 32MHz oscillator */
system_clocks_init();

/* Turn on backlight */
SETBIT(PORTD.DIR,5);
SETBIT(PORTD.OUT,5);

/* initialize TCC0 overflow interrupt */
tcc0_init();

/* Initialize the LCD controller with the default values
   from glcd_ssd1963.h */

/* Specify the current font for displaying text */
glcd_init_data.font=font5x7;

/* Specify the function used for reading image data
   from external memory: SD card */
glcd_init_data.readxmem=read_sd;

/* No function is used for writing image data to external memory */
glcd_init_data.writexmem=NULL;

/* SSD1963 crystal [kHz] */ 
glcd_init_data.ctrl_clk=10000; 
/* 4.3" 480x272 TFT display parameters from the datasheet */
/* TFT pixel clock [kHz] */
glcd_init_data.tft_pixel_clk=9000; 
glcd_init_data.hpulse_width=41;
glcd_init_data.hfront_porch=2;
glcd_init_data.hback_porch=2;
glcd_init_data.vpulse_width=10;
glcd_init_data.vfront_porch=2;
glcd_init_data.vback_porch=2;
/* 18 bit panel data width */
glcd_init_data.tft24bit=SSD1963_TFT_DATA_WIDTH18;
/* dot clock pulse polarity */
glcd_init_data.lshift=SSD1963_DEFAULT_LSHIFT;
/* Normal display, no horizontal reverse */ 
glcd_init_data.reverse_x=SSD1963_REVX_NORM;
/* Normal display, no vertical reverse */ 
glcd_init_data.reverse_y=SSD1963_REVY_NORM;
/* Pixel order: RGB */
glcd_init_data.cl_bits_order=SSD1963_DEFAULT_CL_BITS; 

glcd_init(&glcd_init_data);

/* Display literal char string located in FLASH */
glcd_outtextf("Solomon Systech SSD1963 Graphics Demo\n"
              "Display an image stored in a binary\n"
              "file on a SD card.\n\n");
/* globally enable interrupts */
#asm("sei")

/* check if we have a SD card inserted without using the /CD signal */
if (!sdcard_present()) error(FR_NOT_READY);

/* mount logical drive 0: */
if ((res=f_mount(0,&fat))==FR_OK)
   glcd_outtextf("Logical drive 0: mounted OK.\n");
else
   /* an error occured, display it and stop */
   error(res);

/* Open image file from SD card */
if ((res=f_open(&file,path,FA_READ))==FR_OK)
   glcd_outtextf("Image file opened OK.\n");
else
   /* an error occured, display it and stop */
   error(res);

glcd_outtextf("Displaying image from SD card:\n");

/* Get the free vertical coordinate where the image
   can be displayed under the text, with the space of 2 lines */
y=glcd_gety()+2*glcd_textheight();

data_buffer_bytes=0; /* no data yet in the disk read buffer */

/* Read the file and display the image using the 'read_sd' function */   
glcd_putimagex(10 /* top left image corner X coordinate */,
               y /* top left image corner Y coordinate */,
               0 /* file start reading position */,
               GLCD_PUTCOPY /* copy a bitmap from memory to LCD
                            overwriting previous display data */
               );

/* Close the file */
f_close(&file);

/* Stop here */
while (1);
}
