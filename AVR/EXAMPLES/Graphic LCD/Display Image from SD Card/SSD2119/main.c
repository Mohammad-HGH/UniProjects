/**********************************************************
Solomon Systech SSD2119 Color TFT LCD controller demo.
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
Standard versions of CodeVisionAVR, because the glcd_ssd2119.lib
file is supplied only with the Advanced license.
In this case you can program the ATxmega128A1 FLASH with
the contents of the
\EXAMPLES ATXmega\Graphic LCD\Display Image from SD Card\SSD2119\Exe\Display image from SD card.hex
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

A generic 320x240 3.5" TFT LCD module is used for
the purpose of this demo.
It can be replaced with other similar modules with
the SSD2119 controller. 

The TFT LCD module connections are:
DB0  - not connected
DB1  - not connected
DB2  - not connected
DB3  - not connected
DB4  - not connected
DB5  - not connected
DB6  - not connected
DB7  - not connected

DB8  - PORTA bit 0
DB9  - PORTA bit 1
DB10 - PORTA bit 2
DB11 - PORTA bit 3
DB12 - PORTA bit 4
DB13 - PORTA bit 5
DB14 - PORTA bit 6
DB15 - PORTA bit 7

/CS           - PORTD bit 0
RS (D/C)      - PORTD bit 1
/WR           - PORTD bit 2
/RD           - PORTD bit 3
/RESET (/RST) - PORTD bit 4

8 bit interface selection inputs:
PS0  - +3.3V
PS1  - +3.3V
PS2  - GND
PS3  - GND

Power supply connections:
GND  - GND
3V3  - +3.3V

If you are using a different LCD module,
please use the connections from the module datasheet.

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

/* initialize TCC0 overflow interrupt */
tcc0_init();

/* Initialize the LCD controller with the default values
   from glcd_ssd2119.h */

/* Specify the current font for displaying text */
glcd_init_data.font=font5x7;

/* Specify the function used for reading image data
   from external memory: SD card */
glcd_init_data.readxmem=read_sd;

/* No function is used for writing image data to external memory */
glcd_init_data.writexmem=NULL;

/* Normal display, no horizontal reverse */
glcd_init_data.reverse_x=SSD2119_REVX_NORM;
/* Normal display, no vertical reverse */
glcd_init_data.reverse_y=SSD2119_REVY_NORM;
/* First output gate */
glcd_init_data.first_out_gate=SSD2119_DEFAULT_FIRST_OUT_GATE;
/* Color bits writing order to display RAM: RGB */
glcd_init_data.cl_bits_order=SSD2119_DEFAULT_CL_BITS;
/* Power control 1 BT0..BT2 step-up factor of the step-up circuit */
glcd_init_data.stepup_factor=SSD2119_DEFAULT_STEPUP_FACTOR;
/* Power control 1 DC0..DC3, DCT0..DCT3 step-up circuit cycle */
glcd_init_data.stepup_cycle=SSD2119_DEFAULT_STEPUP_CYCLE;
/* Power control 1 AP0..AP2 adjusts the amount of current
   from the constant current source in the
   internal op. amplifier circuit */
glcd_init_data.crt_source=SSD2119_DEFAULT_CRT_SOURCE;
/* Default value for VCIX2 voltage */
glcd_init_data.vcix2=SSD2119_DEFAULT_VCIX2;
/* Default value for VLCD63 voltage */
glcd_init_data.vlcd63=SSD2119_DEFAULT_VLCD63;
// Default value for VcomL alternating drive voltage
glcd_init_data.vcoml=SSD2119_DEFAULT_VCOML;
/* Default value for VcomH voltage */
glcd_init_data.vcomh=SSD2119_DEFAULT_VCOMH;
/* Frame frequency */
glcd_init_data.frame_freq=SSD2119_DEFAULT_FRAME_FREQ;
/* PKP00..PKP02 positive gamma micro adj. */
glcd_init_data.pkp00=SSD2119_DEFAULT_PKP00;
/* PKP10..PKP12 positive gamma micro adj. */
glcd_init_data.pkp10=SSD2119_DEFAULT_PKP10;
/* PKP20..PKP22 positive gamma micro adj. */
glcd_init_data.pkp20=SSD2119_DEFAULT_PKP20;
/* PKP30..PKP32 positive gamma micro adj. */
glcd_init_data.pkp30=SSD2119_DEFAULT_PKP30;
/* PKP40..PKP42 positive gamma micro adj. */
glcd_init_data.pkp40=SSD2119_DEFAULT_PKP40;
/* PKP50..PKP52 positive gamma micro adj. */
glcd_init_data.pkp50=SSD2119_DEFAULT_PKP50;
/* PRP00..PRP02 positive gamma gradient adj. */
glcd_init_data.prp00=SSD2119_DEFAULT_PRP00;
/* PRP10..PRP12 positive gamma gradient adj. */
glcd_init_data.prp10=SSD2119_DEFAULT_PRP10;
/* VRP00..VRP03 positive gamma amplification adj. */
glcd_init_data.vrp00=SSD2119_DEFAULT_VRP00;
/* VRP10..VRP14 positive gamma amplification adj. */
glcd_init_data.vrp10=SSD2119_DEFAULT_VRP10;
/* PKN00..PKN02 negative gamma micro adj. */
glcd_init_data.pkn00=SSD2119_DEFAULT_PKN00;
/* PKN10..PKN12 negative gamma micro adj. */
glcd_init_data.pkn10=SSD2119_DEFAULT_PKN10;
/* PKN20..PKN22 positive gamma micro adj. */
glcd_init_data.pkn20=SSD2119_DEFAULT_PKN20;
/* PKN30..PKN32 positive gamma micro adj. */
glcd_init_data.pkn30=SSD2119_DEFAULT_PKN30;
/* PKN40..PKN42 negative gamma micro adj. */
glcd_init_data.pkn40=SSD2119_DEFAULT_PKN40;
/* PKN50..PKN52 negative gamma micro adj. */
glcd_init_data.pkn50=SSD2119_DEFAULT_PKN50;
/* PRN00..PRN02 negative gamma gradient adj. */
glcd_init_data.prn00=SSD2119_DEFAULT_PRN00;
/* PRN10..PRN12 negative gamma gradient adj. */
glcd_init_data.prn10=SSD2119_DEFAULT_PRN10;
/* VRN00..VRN03 negative gamma amplification adj. */
glcd_init_data.vrn00=SSD2119_DEFAULT_VRN00;
/* VRN10..VRN14 negative gamma amplification adj. */
glcd_init_data.vrn10=SSD2119_DEFAULT_VRN10;

glcd_init(&glcd_init_data);

/* Display literal char string located in FLASH */
glcd_outtextf("Solomon Systech SSD2119 Graphics Demo\n"
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

/* No data in the SD card read buffer yet */
data_buffer_bytes=0;

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
