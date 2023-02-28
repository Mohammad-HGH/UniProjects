/*
Sample program to test the ATxmega128A1 DACA in dual
channel mode, using the internal 1V reference.

(C) Copyright 2010-2011 Pavel Haiduc, HP InfoTech srl

Use a STK600 development board with STK600-TQFP100 and
STK600-RC100X-13 addapters.

The STK600 programmer must be set in JTAG programming mode
in the Tools|Programmer menu.

Make sure that the VTARGET and VREF voltages are set to 3.6V
using AVR Studio.
The VTARGET LED on the STK600 board must be lighted.

The program can be also run on the Xplain development board.

The following 100Hz, 1V peak to peak, waveforms will be
generated:
- sinusoidal on DACA channel 0 output DAC0 on PORTA pin 2.
- sawtooth on DACA channel 1 output DAC1 on PORTA pin 3.
*/

// I/O Registers definitions
#include <io.h>
// sin function
#include <math.h>

// Look-up tables dimension
#define TABLE_DIM 100

// Look-up table for channel 0, filled with values
// needed to obtain a sinusoidal waveform
unsigned int table0[TABLE_DIM];
// Look-up table for channel 1, filled with values
// needed to obtain a sawtooth waveform
unsigned int table1[TABLE_DIM];
// Look-up table index
#if TABLE_DIM>255
unsigned int index;
#else
unsigned char index;
#endif

// Disable a Timer/Counter type 0
void tc0_disable(TC0_t *ptc)
{
// Timer/Counter off
ptc->CTRLA=(ptc->CTRLA & (~TC0_CLKSEL_gm)) | TC_CLKSEL_OFF_gc;
// Issue a reset command
ptc->CTRLFSET=TC_CMD_RESET_gc;
}

// Timer/Counter TCC0 initialization
void tcc0_init(void)
{
unsigned char s;
unsigned char n;

// Note: the correct PORTC direction for the Compare Channels outputs
// is configured in the ports_init function

// Save interrupts enabled/disabled state
s=SREG;
// Disable interrupts
#asm("cli")

// Disable and reset the timer/counter just to be sure
tc0_disable(&TCC0);
// Clock source: Peripheral Clock/1
TCC0.CTRLA=(TCC0.CTRLA & (~TC0_CLKSEL_gm)) | TC_CLKSEL_DIV1_gc;
// Mode: Normal Operation, Overflow Int./Event on TOP
// Compare/Capture on channel A: Off
// Compare/Capture on channel B: Off
// Compare/Capture on channel C: Off
// Compare/Capture on channel D: Off
TCC0.CTRLB=(TCC0.CTRLB & (~(TC0_CCAEN_bm | TC0_CCBEN_bm | TC0_CCCEN_bm | TC0_CCDEN_bm | TC0_WGMODE_gm))) |
	TC_WGMODE_NORMAL_gc;

// Capture event source: None
// Capture event action: None
TCC0.CTRLD=(TCC0.CTRLD & (~(TC0_EVACT_gm | TC0_EVSEL_gm))) |
	TC_EVACT_OFF_gc | TC_EVSEL_OFF_gc;

// Overflow interrupt: Low Level
// Error interrupt: Disabled
TCC0.INTCTRLA=(TCC0.INTCTRLA & (~(TC0_ERRINTLVL_gm | TC0_OVFINTLVL_gm))) |
	TC_ERRINTLVL_OFF_gc | TC_OVFINTLVL_LO_gc;

// Compare/Capture channel A interrupt: Disabled
// Compare/Capture channel B interrupt: Disabled
// Compare/Capture channel C interrupt: Disabled
// Compare/Capture channel D interrupt: Disabled
TCC0.INTCTRLB=(TCC0.INTCTRLB & (~(TC0_CCDINTLVL_gm | TC0_CCCINTLVL_gm | TC0_CCBINTLVL_gm | TC0_CCAINTLVL_gm))) |
	TC_CCDINTLVL_OFF_gc | TC_CCCINTLVL_OFF_gc | TC_CCBINTLVL_OFF_gc | TC_CCAINTLVL_OFF_gc;

// High resolution extension: Off
HIRESC.CTRLA&= ~HIRES_HREN0_bm;

// Advanced Waveform Extension initialization
// Optimize for speed
#pragma optsize- 
// Disable locking the AWEX configuration registers just to be sure
n=MCU.AWEXLOCK & (~MCU_AWEXCLOCK_bm);
CCP=CCP_IOREG_gc;
MCU.AWEXLOCK=n;
// Restore optimization for size if needed
#pragma optsize_default

// Pattern generation: Off
// Dead time insertion: Off
AWEXC.CTRL&= ~(AWEX_PGM_bm | AWEX_CWCM_bm | AWEX_DTICCDEN_bm | AWEX_DTICCCEN_bm | AWEX_DTICCBEN_bm | AWEX_DTICCAEN_bm);

// Fault protection initialization
// Fault detection on OCD Break detection: On
// Fault detection restart mode: Latched Mode
// Fault detection action: None (Fault protection disabled)
AWEXC.FDCTRL=(AWEXC.FDCTRL & (~(AWEX_FDDBD_bm | AWEX_FDMODE_bm | AWEX_FDACT_gm))) |
	AWEX_FDACT_NONE_gc;
// Fault detect events: 
// Event channel 0: Off
// Event channel 1: Off
// Event channel 2: Off
// Event channel 3: Off
// Event channel 4: Off
// Event channel 5: Off
// Event channel 6: Off
// Event channel 7: Off
AWEXC.FDEVMASK=0b00000000;
// Make sure the fault detect flag is cleared
AWEXC.STATUS|=AWEXC.STATUS & AWEX_FDF_bm;

// Clear the interrupt flags
TCC0.INTFLAGS=TCC0.INTFLAGS;
// Set counter register
TCC0.CNT=0x0000;
// Set period register for overflows to occur every 100 us
TCC0.PER=0x00C7;
// Set channel A Compare/Capture register
TCC0.CCA=0x0000;
// Set channel B Compare/Capture register
TCC0.CCB=0x0000;
// Set channel C Compare/Capture register
TCC0.CCC=0x0000;
// Set channel D Compare/Capture register
TCC0.CCD=0x0000;

// Restore interrupts enabled/disabled state
SREG=s;
}

void daca_write(unsigned char ch, unsigned int data);

// Timer/counter TCC0 Overflow/Underflow interrupt service routine
// occurs every 100us
interrupt [TCC0_OVF_vect] void tcc0_overflow_isr(void)
{
// Prevent look-up table index overflow
if (index>=TABLE_DIM) index=0;
// Output the value from the sinusoidal
// look-up table to DACA channel 0
daca_write(0,table0[index]);
// Output the value from the sawtooth
// look-up table to DACA channel 1
// and increment the index
daca_write(1,table1[index++]);
}

// DACA initialization
void daca_init(void)
{
// Operating mode: Dual Channel (Sample/Hold for Ch0 & Ch1)
// Channel 0 triggered by the event system: Off
// Channel 1 triggered by the event system: Off
DACA.CTRLB=(DACA.CTRLB & (~(DAC_CHSEL_gm | DAC_CH0TRIG_bm | DAC_CH1TRIG_bm))) |
	DAC_CHSEL_DUAL_gc;

// Reference: Internal 1.00V
// Left adjust value: Off
DACA.CTRLC=(DACA.CTRLC & (~(DAC_REFSEL_gm | DAC_LEFTADJ_bm))) |
	DAC_REFSEL_INT1V_gc;

// Conversion interval: 1.500us (3 ClkPer cycles)
// Refresh interval: 8.000us (16 ClkPer cycles)
DACA.TIMCTRL=(DACA.TIMCTRL & (~(DAC_CONINTVAL_gm | DAC_REFRESH_gm))) |
	DAC_CONINTVAL_2CLK_gc | DAC_REFRESH_16CLK_gc;

// DACA is enabled
// Low power mode: Off
// Channel 0 output: On
// Channel 1 output: On
// Internal output connected to the ADCA and Analog Comparator MUX-es: Off
DACA.CTRLA=(DACA.CTRLA & (~(DAC_IDOEN_bm | DAC_CH0EN_bm | DAC_CH1EN_bm))) |
	DAC_CH0EN_bm | DAC_CH1EN_bm | DAC_ENABLE_bm;
}

// Function used to write data to a DACA channel ch
void daca_write(unsigned char ch, unsigned int data)
{
register unsigned char m=ch ? DAC_CH1DRE_bm : DAC_CH0DRE_bm;
// Wait for the channel data register to be ready for new data
while ((DACA.STATUS & m)==0);
// Write new data to the channel data register
if (m==DAC_CH1DRE_bm) DACA.CH1DATA=data;
else DACA.CH0DATA=data;
}

void main(void)
{
unsigned char n;

// Initialize the lookup tables...
for (index=0; index<TABLE_DIM; index++)
    {
    // for sinusoidal waveform
    table0[index]=(unsigned int) (2047.0*sin(index*((2*PI)/TABLE_DIM))+2047.0);
    // for sawtooth waveform
    table1[index]=(unsigned int) ((4095L*index)/TABLE_DIM);
    }

// Waveform generation will start with 0 phase
index=0;
    
// Interrupt system initialization
// Optimize for speed
#pragma optsize- 
// Make sure the interrupts are disabled
#asm("cli")
// Low level interrupt: On
// Round-robin scheduling for low level interrupt: Off
// Medium level interrupt: Off
// High level interrupt: Off
// The interrupt vectors will be placed at the start of the Application FLASH section
n=(PMIC.CTRL & (~(PMIC_RREN_bm | PMIC_IVSEL_bm | PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm))) |
	PMIC_LOLVLEN_bm;
CCP=CCP_IOREG_gc;
PMIC.CTRL=n;
// Set the default priority for round-robin scheduling
PMIC.INTPRI=0x00;
// Restore optimization for size if needed
#pragma optsize_default

// Timer/Counter TCC0 initialization
tcc0_init();

// DACA initialization
daca_init();

// Globally enable interrupts
#asm("sei")

// The rest is performed by Timer/Counter TCC0 overflow interrupts
while (1);
}
