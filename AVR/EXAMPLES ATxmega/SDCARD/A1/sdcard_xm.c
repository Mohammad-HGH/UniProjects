/******************************************************************************
Creation, writing, appending, reading, attribute and time stamp changing
of a file on a MMC/SD/SD HC card example for the CodeVisionAVR V2.05.9+
compiler.
(C) 2009-2014 HP InfoTech s.r.l., Pavel Haiduc

The example is intended to be run on the Atmel STK600 or other
development boards that use an ATxmega128A1 chip.
The chip is clocked from the internal 32MHz oscillator.

A Micro SD card socket is connected to the development board
as outlined below:

Card Socket      STK600 ATxmega128A1
/CS ------------ PORTD bit4
SI  ------------ PORTD bit5 MOSI
SO  ------------ PORTD bit6 MISO
SCK ------------ PORTD bit7 SCK
GND ------------ GND
+3.3V ---------- VTG +3.3V

The /CD and WP signals are disabled in the project configuration,
because a Micro SD card socket without these signals was used.

For other I/O port connections, you need to make the appropriate changes in the
"Project|Configure|C Compiler|Libraries|MMC/SD/SD HC Card and FAT Support"
menu.

The ATxmega128A1 USARTC0 is used for communication.
Make the following connections:

STK600 RS232     STK600 ATxmega128A1
Spare
RXD ------------ PORTC bit2
TXD ------------ PORTC bit3

The CodeVisionAVR Terminal is used for displaying data received from the
development board.
The Terminal must be configured for:

Baud Rate: 9600
Data Bits: 8
Parity: None
Stop Bits: 1
Emulation: TTY
Handshaking: None
Append LF on Reception: OFF
Appearance|Rows: 40
Appearance|Columns: 80

If you are using the CodeVisionAVR Evaluation version, you may program into
the FLASH the precompiled SDCARD_XM.HEX file found in the
\Examples ATxmega\SDCARD\EXE directory
******************************************************************************/

/* ATxmega128A1 I/O register definitions */
#include <io.h>
/* FAT on MMC/SD/SD HC card support */
#include <ff.h>
/* printf */
#include <stdio.h>

/* FAT function result */
FRESULT res;
/* number of bytes written/read to the file */
unsigned int nbytes;
/* will hold the information for logical drive 0: */
FATFS fat;
/* will hold the file information */
FIL file;
/* will hold file attributes, time stamp information */
FILINFO finfo;
/* file path */
char path[]="0:/file.txt";
/* text to be written to the file */
char text1[]="I like CodeVisionAVR! ";
char text2[]="Here's some more text...";
/* file read buffer */
char buffer[256];

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

/* System Clocks initialization */
void system_clocks_init(void)
{
unsigned char n;

/* Optimize for speed */
#pragma optsize-

/* Internal 32 kHz RC oscillator initialization
   Enable the internal 32 kHz RC oscillator */
OSC.CTRL|=OSC_RC32KEN_bm;
/* Wait for the internal 32 kHz RC oscillator to stabilize */
while ((OSC.STATUS & OSC_RC32KRDY_bm)==0);

/* Internal 32 MHz RC oscillator initialization
   Enable the internal 32 MHz RC oscillator */
OSC.CTRL|=OSC_RC32MEN_bm;

/* System Clock prescaler A division factor: 1
   System Clock prescalers B & C division factors: B:4, C:1
   ClkPer4: 32000.000 kHz
   ClkPer2: 8000.000 kHz
   ClkPer:  8000.000 kHz
   ClkCPU:  8000.000 kHz */
n=(CLK.PSCTRL & (~(CLK_PSADIV_gm | CLK_PSBCDIV1_bm | CLK_PSBCDIV0_bm))) |
    CLK_PSADIV_1_gc | CLK_PSBCDIV_4_1_gc;
CCP=CCP_IOREG_gc;
CLK.PSCTRL=n;

/* Internal 32 MHz RC osc. calibration reference clock source: 32.768 kHz Internal Osc. */
OSC.DFLLCTRL&= ~(OSC_RC32MCREF_bm | OSC_RC2MCREF_bm);
/* Enable the autocalibration of the internal 32 MHz RC oscillator */
DFLLRC32M.CTRL|=DFLL_ENABLE_bm;

/* Wait for the internal 32 MHz RC oscillator to stabilize */
while ((OSC.STATUS & OSC_RC32MRDY_bm)==0);

/* Select the system clock source: 32 MHz Internal RC Osc. */
n=(CLK.CTRL & (~CLK_SCLKSEL_gm)) | CLK_SCLKSEL_RC32M_gc;
CCP=CCP_IOREG_gc;
CLK.CTRL=n;

/* Disable the unused oscillators: 2 MHz, external clock/crystal oscillator, PLL */
OSC.CTRL&= ~(OSC_RC2MEN_bm | OSC_XOSCEN_bm | OSC_PLLEN_bm);

/* ClkPer output: Disabled bit 7 */
PORTCFG.CLKEVOUT=(PORTCFG.CLKEVOUT & (~PORTCFG_CLKOUT_gm)) | PORTCFG_CLKOUT_OFF_gc;

/* Restore optimization for size if needed */
#pragma optsize_default
}

/* Timer/Counter TCC0 initialization */
void tcc0_init(void)
{
/* Clock source: ClkPer/2 */
TCC0.CTRLA=(TCC0.CTRLA & (~TC0_CLKSEL_gm)) | TC_CLKSEL_DIV2_gc;
/* Mode: Normal Operation, Overflow Int./Event on TOP */
TCC0.CTRLB=(TCC0.CTRLB & (~(TC0_CCAEN_bm | TC0_CCBEN_bm | TC0_CCCEN_bm | TC0_CCDEN_bm | TC0_WGMODE_gm))) |
    TC_WGMODE_NORMAL_gc;

/* Capture event source: None
   Capture event action: None */
TCC0.CTRLD=(TCC0.CTRLD & (~(TC0_EVACT_gm | TC0_EVSEL_gm))) |
    TC_EVACT_OFF_gc | TC_EVSEL_OFF_gc;

/* Overflow interrupt: Low Level
   Error interrupt: Disabled */
TCC0.INTCTRLA=(TCC0.INTCTRLA & (~(TC0_ERRINTLVL_gm | TC0_OVFINTLVL_gm))) |
    TC_ERRINTLVL_OFF_gc | TC_OVFINTLVL_LO_gc;

/* Compare/Capture channels A,B,C,D interrupts: Disabled */
TCC0.INTCTRLB=(TCC0.INTCTRLB & (~(TC0_CCDINTLVL_gm | TC0_CCCINTLVL_gm | TC0_CCBINTLVL_gm | TC0_CCAINTLVL_gm))) |
    TC_CCDINTLVL_OFF_gc | TC_CCCINTLVL_OFF_gc | TC_CCBINTLVL_OFF_gc | TC_CCAINTLVL_OFF_gc;

/* High resolution extension: Off */
HIRESC.CTRLA&= ~HIRES_HREN0_bm;

/* Clear the interrupt flags */
TCC0.INTFLAGS=TCC0.INTFLAGS;
/* Set counter register */
TCC0.CNT=0x0000;
/* Set period register for 10ms overflow interrupt for ClkPer: 8MHz */
TCC0.PER=0x9C3F;
}

/* Timer/counter TCC0 Overflow interrupt service routine
   Occurs every 10ms */
interrupt [TCC0_OVF_vect] void tcc0_overflow_isr(void)
{
/* Card access low level timing function */
disk_timerproc();
}

// USARTC0 initialization
void usartc0_init(void)
{
/* Bit0: Input
   Bit1: Input
   Bit2: Input
   Bit3: Output for USARTC0 TXD
   Bit4: Input
   Bit5: Input
   Bit6: Input
   Bit7: Input */
PORTC.DIR=0x08;

/* Set TxD=1 */
PORTC.OUTSET=0x08;

/* Communication mode: Asynchronous USART
   Data bits: 8
   Stop bits: 1
   Parity: Disabled */
USARTC0.CTRLC=USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc;

/* Receive complete interrupt: Disabled
   Transmit complete interrupt: Disabled
   Data register empty interrupt: Disabled */
USARTC0.CTRLA=(USARTC0.CTRLA & (~(USART_RXCINTLVL_gm | USART_TXCINTLVL_gm | USART_DREINTLVL_gm))) |
	USART_RXCINTLVL_OFF_gc | USART_TXCINTLVL_OFF_gc | USART_DREINTLVL_OFF_gc;

/* Required Baud rate: 9600 (x1 Mode) */
USARTC0.BAUDCTRLA=0xC5;
USARTC0.BAUDCTRLB=((0x0A << USART_BSCALE_bp) & USART_BSCALE_gm) | 0x0C;

/* Receiver: Off
   Transmitter: On
   Double transmission speed mode: Off
   Multi-processor communication mode: Off */
USARTC0.CTRLB=(USARTC0.CTRLB & (~(USART_RXEN_bm | USART_TXEN_bm | USART_CLK2X_bm | USART_MPCM_bm | USART_TXB8_bm))) |
	USART_TXEN_bm;
}

/* Write a character to the USARTC0 Transmitter
   USARTC0 is used as the default output device by the 'putchar' function */
#define _ALTERNATE_PUTCHAR_

#pragma used+
void putchar(char c)
{
while ((USARTC0.STATUS & USART_DREIF_bm) == 0);
USARTC0.DATA=c;
}
#pragma used-

/* display error message and stop */
void error(FRESULT res)
{
if ((res>=FR_DISK_ERR) && (res<=FR_TIMEOUT))
   printf("ERROR: %p\r\n",error_msg[res]);
/* stop here */
while(1);
}

/* display file's attribute, size and time stamp */
void display_status(char *file_name)
{
if ((res=f_stat(file_name,&finfo))==FR_OK)
   printf("File: %s, Attributes: %c%c%c%c%c\r\n"
          "Date: %02u/%02u/%u, Time: %02u:%02u:%02u\r\n"
          "Size: %lu bytes\r\n",
          finfo.fname,
          (finfo.fattrib & AM_DIR) ? 'D' : '-',
          (finfo.fattrib & AM_RDO) ? 'R' : '-',
          (finfo.fattrib & AM_HID) ? 'H' : '-',
          (finfo.fattrib & AM_SYS) ? 'S' : '-',
          (finfo.fattrib & AM_ARC) ? 'A' : '-',
          finfo.fdate & 0x1F, (finfo.fdate >> 5) & 0xF,
          (finfo.fdate >> 9) + 1980,
          (finfo.ftime >> 11), (finfo.ftime >> 5) & 0x3F,
          (finfo.ftime & 0xF) << 1,
          finfo.fsize);
else
   /* an error occurred, display it and stop */
   error(res);
}

/* read and display the file's content */
void display_file_contents(void)
{
/* open the file in read mode */
if ((res=f_open(&file,path,FA_READ))==FR_OK)
   printf("File %s opened OK\r\n",path);
else
   /* an error occurred, display it and stop */
   error(res);

/* make sure to leave space for a NULL terminator
in the buffer, so maximum sizeof(buffer)-1 bytes
can be read */
if ((res=f_read(&file,buffer,sizeof(buffer)-1,&nbytes))==FR_OK)
   {
   printf("%u bytes read\r\n",nbytes);
   /* NULL terminate the char string in the buffer */
   buffer[nbytes+1]=NULL;
   /* display the buffer contents */
   printf("Read text: \"%s\"\r\n",buffer);
   }
else
   /* an error occurred, display it and stop */
   error(res);

/* close the file */
if ((res=f_close(&file))==FR_OK)
   printf("File %s closed OK\r\n",path);
else
   /* an error occurred, display it and stop */
   error(res);
}

void main(void)
{
unsigned char n;

/* Interrupt system initialization
   Optimize for speed */
#pragma optsize-
/* Low level interrupt: On
   Round-robin scheduling for low level interrupt: Off
   Medium level interrupt: Off
   High level interrupt: Off
   The interrupt vectors will be placed at the start of the Application FLASH section */
n=(PMIC.CTRL & (~(PMIC_RREN_bm | PMIC_IVSEL_bm | PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm))) |
	PMIC_LOLVLEN_bm;
CCP=CCP_IOREG_gc;
PMIC.CTRL=n;
/* Set the default priority for round-robin scheduling */
PMIC.INTPRI=0x00;
/* Restore optimization for size if needed */
#pragma optsize_default

/* System clocks initialization to use the internal 32MHz oscillator */
system_clocks_init();

/* initialize TCC0 overflow interrupt */
tcc0_init();

/* initialize the USARTC0 TX, 8N1, Baud rate: 9600 */
usartc0_init();

/* We have no RTC on the development board, so we will
   init the pointer to the RTC function used for reading time to NULL
   The time used by the file system when creating a new file
   will be 00:00:00 */
prtc_get_time=NULL;

/* We have no RTC on the development board, so we will
   init the pointer to the RTC function used for reading date to NULL
   The date used by the file system when creating a new file
   will be 01/01/2009 */
prtc_get_date=NULL;

/* globally enable interrupts */
#asm("sei")

/* check if we have a SD card inserted without using the /CD signal */
if (!sdcard_present()) error(FR_NOT_READY);

/* mount logical drive 0: */
if ((res=f_mount(0,&fat))==FR_OK)
   printf("Logical drive 0: mounted OK\r\n");
else
   /* an error occurred, display it and stop */
   error(res);

/* delete an eventual old file with the same name */
f_unlink(path);

/* create a new file in the root of drive 0:
and set write access mode */
if ((res=f_open(&file,path,FA_CREATE_ALWAYS | FA_WRITE))==FR_OK)
   printf("File %s created OK\r\n",path);
else
   /* an error occurred, display it and stop */
   error(res);

/* write some text to the file,
without the NULL string terminator sizeof(data)-1 */
printf("Write text1 to the file\r\n");
if ((res=f_write(&file,text1,sizeof(text1)-1,&nbytes))==FR_OK)
   printf("%u bytes written of %u\r\n",nbytes,sizeof(text1)-1);
else
   /* an error occurred, display it and stop */
   error(res);

/* close the file */
if ((res=f_close(&file))==FR_OK)
   printf("File %s closed OK\r\n",path);
else
   /* an error occurred, display it and stop */
   error(res);

/* read and display the file's content */
display_file_contents();

/* append some more text to the end of the file */
/* open the file in read mode */
if ((res=f_open(&file,path,FA_WRITE))==FR_OK)
   printf("File %s opened OK\r\n",path);
else
   /* an error occurred, display it and stop */
   error(res);

/* seek to the end of the file for appending */
if ((res=f_lseek(&file,file.fsize))==FR_OK)
   printf("Seek to the end of the file OK\r\n",path);
else
   /* an error occurred, display it and stop */
   error(res);

/* write some more text to the file,
without the NULL string terminator sizeof(data)-1 */
printf("Append text2 to the file\r\n");
if ((res=f_write(&file,text2,sizeof(text2)-1,&nbytes))==FR_OK)
   printf("%u bytes appended of %u\r\n",nbytes,sizeof(text2)-1);
else
   /* an error occurred, display it and stop */
   error(res);

/* close the file */
if ((res=f_close(&file))==FR_OK)
   printf("File %s closed OK\r\n",path);
else
   /* an error occurred, display it and stop */
   error(res);

/* read and display the file's content */
display_file_contents();

/* display file's attribute, size and time stamp */
display_status(path);

/* change file's attributes, set the file to be Read-Only */
if ((res=f_chmod(path,AM_RDO,AM_RDO))==FR_OK)
   printf("Read-Only attribute set OK\r\n",path);
else
   /* an error occurred, display it and stop */
   error(res);

/* change file's time stamp */
#define DAY (6)
#define MONTH (3)
#define YEAR (2000)
#define SECOND (0)
#define MINUTE (40)
#define HOUR (14)

finfo.fdate=DAY | (MONTH<<5) | ((YEAR-1980)<<9);
finfo.ftime=(SECOND>>1) | (MINUTE<<5) | (HOUR<<11);
if ((res=f_utime(path,&finfo))==FR_OK)
   printf("New time stamp %02u/%02u/%u %02u:%02u:%02u set OK\r\n",
   DAY,MONTH,YEAR,HOUR,MINUTE,SECOND);
else
   /* an error occurred, display it and stop */
   error(res);

/* display file's new attribute and time stamp */
display_status(path);

/* change file's attributes, clear the Read-Only attribute */
if ((res=f_chmod(path,0,AM_RDO))==FR_OK)
   printf("Read-Only attribute cleared OK\r\n",path);
else
   /* an error occurred, display it and stop */
   error(res);

/* display file's new attribute and time stamp */
display_status(path);

/* stop here */
while(1);
}

