/**********************************************************************
 ATxmega EBI SDRAM example for the Atmel XPLAIN and XMEGA-A1 Xplained 
 boards and the CodeVisionAVR V3.04 or later compilers
 (C) Copyright 2010-2013 Pavel Haiduc, HP InfoTech s.r.l.  
 
 Chip Type: ATxmega128A1 running from the 32MHz internal oscillator
 The EBI configuration is specified in the
 Project|Configure|C Compiler|EBI Settings menu
 
 The SDRAM settings are:
 
 CAS Latency: 3 x2 ClkPer cycles 
 Row bits: 12 
 Column bits: 10 
 Mode Register Delay: 2 x2 ClkPer cycles  
 Row Cycle Delay: 7 x2 ClkPer cycles  
 Row to Pre-charge Delay: 7 x2 ClkPer cycles  
 Write Recovery Delay: 1 x2 ClkPer cycles  
 Exit Self Refresh to Active Delay: 7 x2 ClkPer cycles  
 Row to Column Delay: 7 x2 ClkPer cycles  
 Refresh Period: 1023 x2 ClkPer cycles (64 us @ 32MHz). 
 Initialization Delay: 4096 x2 ClkPer cycles (256 us @ 32MHz). 
 
 All EBI initialization is performed automatically by
 CodeVisionAVR in the startup code
 
 The SDRAM connections are:
 
 PORTK[7:0] - A[7:0]
 PORTJ[7:0] - {A[11:8],D[3:0]}
 PORTH[7:0] - {WE,CAS,RAS,DQM,BA0,BA1,CKE,CLK}   
 Note: The SDRAM /CS signal is permanently connected to GND 
**********************************************************************/

#include <stdbool.h>
#include <io.h>
#include <delay.h>

/*
Address where we want SDRAM test to start. 
This address must be above 0xFFFF so that the compiler's hardware stack
will not be overwritten
*/
#define SDRAM_TEST_START_ADDR 0x1E000

/* Number of test bytes to store to SDRAM */
#define TEST_SIZE (128*1024L) // 128kbytes

/* # of bytes for which a LED will be set ON during SDRAM write and
OFF during SDRAM read check */
#define BLOCK_SIZE (TEST_SIZE/8)

#if __CODEVISIONAVR__ < 3040
#error CVAVR V3.04 or later is required
#endif

#pragma optsize-
/* External memory >64k write function */
void mem_write(unsigned long addr, unsigned char data)
{
unsigned char s,r;
s=SREG;
#asm("cli")
r=RAMPX;
RAMPX=((unsigned char *) &addr)[2];
*((unsigned char*) addr)=data;
RAMPX=r;
SREG=s;
}

/* External memory >64k read function */
unsigned char mem_read(unsigned long addr)
{
unsigned char s,r,data;
s=SREG;
#asm("cli")
r=RAMPX;
RAMPX=((unsigned char *) &addr)[2];
data=*((unsigned char*) addr);
RAMPX=r;
SREG=s;
return data;
}
#pragma optsize_default

/* System Clocks initialization is executed immediatelly after chip RESET */
__reset void system_clocks_init(void)
{
unsigned char n,s;

// Optimize for speed
#pragma optsize- 
// Save interrupts enabled/disabled state
s=SREG;
// Disable interrupts
#asm("cli")

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

// Disable the autocalibration of the internal 32 MHz RC oscillator
DFLLRC32M.CTRL&= ~DFLL_ENABLE_bm;

// Wait for the internal 32 MHz RC oscillator to stabilize
while ((OSC.STATUS & OSC_RC32MRDY_bm)==0);

// Select the system clock source: 32 MHz Internal RC Osc.
n=(CLK.CTRL & (~CLK_SCLKSEL_gm)) | CLK_SCLKSEL_RC32M_gc;
CCP=CCP_IOREG_gc;
CLK.CTRL=n;

// Disable the unused oscillators: 2 MHz, internal 32 kHz, external clock/crystal oscillator, PLL
OSC.CTRL&= ~(OSC_RC2MEN_bm | OSC_RC32KEN_bm | OSC_XOSCEN_bm | OSC_PLLEN_bm);

// Peripheral Clock output: Disabled
PORTCFG.CLKEVOUT=(PORTCFG.CLKEVOUT & (~PORTCFG_CLKOUT_gm)) | PORTCFG_CLKOUT_OFF_gc;

// Restore interrupts enabled/disabled state
SREG=s;
// Restore optimization for size if needed
#pragma optsize_default
}

void main( void )
{
unsigned long a;
unsigned char led_mask;

/* Flag indicating SDRAM check error */
bool error = false;

/* Configure the LED driving outputs */
PORTE.DIR = 0xFF;
/* All LEDs are OFF (the outputs are inverted) */
PORTE.OUT = 0xFF;

led_mask = 0xFF;
/* Fill the SDRAM with data. */
for ( a = SDRAM_TEST_START_ADDR; a < (SDRAM_TEST_START_ADDR+TEST_SIZE); a++)
    {
    mem_write(a,(unsigned char) a & 0xFF);
    /* Light one LED for each block of written data */
    if ((a % BLOCK_SIZE) == 0)
       {
       led_mask <<=1;
       PORTE.OUT &= led_mask;
       }
    }

led_mask = 0xFF;
/* Read back from SDRAM and verify */
for ( a = SDRAM_TEST_START_ADDR; a < (SDRAM_TEST_START_ADDR+TEST_SIZE); a++)
    {
    /* Turn off one LED for each block of read & verified data */
    if ((a % BLOCK_SIZE) == 0)
       {
       led_mask <<=1;
       PORTE.OUT |= ~led_mask;
       }
    if (mem_read(a) != ((unsigned char) a & 0xFF))
       {
       error = true;
       break;
       }
    }

/* Display result on LEDs */
if (error)
   {
   while(true)
        {
	    /* SDRAM error, flash the LEDs fast (5Hz) */
        PORTE.OUTTGL=0xFF;
        delay_ms(100);
	    }
   }
else
   {
   while(true)
        {
	    /* SDRAM OK, flash the LEDs slowly (0.5Hz) */
        PORTE.OUTTGL=0xFF;
        delay_ms(1000);
	    }
   }
}
