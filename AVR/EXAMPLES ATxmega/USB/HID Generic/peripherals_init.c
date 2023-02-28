/*******************************************************
Chip peripherals initialization created by the
CodeWizardAVR V3.13 Advanced
Automatic Program Generator
© Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project: USB Generic HID Example
*******************************************************/

#include <io.h>

// Standard definitions
#include <stddef.h>

// Interrupt system initialization
void interrupts_init(void)
{
unsigned char n;
// Optimize for speed
#pragma optsize-
// Make sure the interrupts are disabled
#asm("cli")
// Low level interrupt: On - used by Timer/Counter TCC0 overflow interrupt
// Round-robin scheduling for low level interrupt: Off
// Medium level interrupt: Off
// High level interrupt: On - used by the USB
// The interrupt vectors will be placed at the start of the Application FLASH section
n=PMIC_LOLVLEN_bm | PMIC_HILVLEN_bm;
CCP=CCP_IOREG_gc;
PMIC.CTRL=n;
// Set the default priority for round-robin scheduling
PMIC.INTPRI=0x00;
// Restore optimization for size if needed
#pragma optsize_default
}

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

// Internal 32 MHz RC oscillator initialization
// Internal 32 MHz RC osc. calibration reference clock source: USB Start Of Frame

// Oscillator calibration not enabled yet
DFLLRC32M.CTRL=0<<DFLL_ENABLE_bp;

// Read the calibration values for 48 MHz from the signature row
// and calibrate the oscillator
NVM.CMD=NVM_CMD_READ_CALIB_ROW_gc;
DFLLRC32M.CALA=*((flash unsigned char *) offsetof(NVM_PROD_SIGNATURES_t,USBRCOSCA));
DFLLRC32M.CALB=*((flash unsigned char *) offsetof(NVM_PROD_SIGNATURES_t,USBRCOSC));
NVM.CMD=NVM_CMD_NO_OPERATION_gc;

// Enable the internal 32 MHz RC oscillator
OSC.CTRL|=OSC_RC32MEN_bm;
// Wait for the internal 32 MHz RC oscillator to stabilize
while ((OSC.STATUS & OSC_RC32MRDY_bm)==0);

// Adjust and calibrate the 32 MHz RC oscillator to 48 MHz
DFLLRC32M.COMP1=(48000000/1024)&0xFF;
DFLLRC32M.COMP2=(48000000/1024)>>8;
// Use the USB Start Of Frame as DFLL clock reference
OSC.DFLLCTRL=(OSC.DFLLCTRL & (~OSC_RC32MCREF_gm)) | OSC_RC32MCREF_USBSOF_gc;

// Enable the auto-calibration of the internal 32 MHz RC oscillator
DFLLRC32M.CTRL=1<<DFLL_ENABLE_bp;

// System Clock prescaler A division factor: 1
// System Clock prescalers B & C division factors: B:4, C:1
// ClkPer4: 48000.000 kHz
// ClkPer2: 12000.000 kHz
// ClkPer:  12000.000 kHz
// ClkCPU:  12000.000 kHz
n=(CLK.PSCTRL & (~(CLK_PSADIV_gm | CLK_PSBCDIV1_bm | CLK_PSBCDIV0_bm))) |
	CLK_PSADIV_1_gc | CLK_PSBCDIV_4_1_gc;
CCP=CCP_IOREG_gc;
CLK.PSCTRL=n;

// Select the system clock source: 32 MHz Internal RC Osc.
n=(CLK.CTRL & (~CLK_SCLKSEL_gm)) | CLK_SCLKSEL_RC32M_gc;
CCP=CCP_IOREG_gc;
CLK.CTRL=n;

// Disable the unused oscillators: 32 kHz, 2 MHz, external clock/crystal oscillator, PLL
OSC.CTRL&= ~(OSC_RC32KEN_bm | OSC_RC2MEN_bm | OSC_XOSCEN_bm | OSC_PLLEN_bm);

// ClkPer output disabled
PORTCFG.CLKEVOUT&= ~(PORTCFG_CLKOUTSEL_gm | PORTCFG_CLKOUT_gm);
// Restore interrupts enabled/disabled state
SREG=s;
// Restore optimization for size if needed
#pragma optsize_default
}

// Disable a Timer/Counter type TC0
void tc0_disable(TC0_t *ptc)
{
// Timer/Counter off
ptc->CTRLA=TC_CLKSEL_OFF_gc;
// Issue a reset command
ptc->CTRLFSET=TC_CMD_RESET_gc;
}

// Timer/Counter TCC0 initialization
void tcc0_init(void)
{
unsigned char s;
unsigned char n;

// Save interrupts enabled/disabled state
s=SREG;
// Disable interrupts
#asm("cli")

// Disable and reset the timer/counter just to be sure
tc0_disable(&TCC0);
// Clock source: ClkPer/1024
TCC0.CTRLA=TC_CLKSEL_DIV1024_gc;
// Mode: Normal Operation, Overflow Int./Event on TOP
// Compare/Capture on channel A: Off
// Compare/Capture on channel B: Off
// Compare/Capture on channel C: Off
// Compare/Capture on channel D: Off
TCC0.CTRLB=(0<<TC0_CCDEN_bp) | (0<<TC0_CCCEN_bp) | (0<<TC0_CCBEN_bp) | (0<<TC0_CCAEN_bp) |
    TC_WGMODE_NORMAL_gc;
// Capture event source: None
// Capture event action: None
TCC0.CTRLD=TC_EVACT_OFF_gc | TC_EVSEL_OFF_gc;

// Set Timer/Counter in Normal mode
TCC0.CTRLE=TC_BYTEM_NORMAL_gc;

// Overflow interrupt: Disabled
// Error interrupt: Disabled
TCC0.INTCTRLA=TC_ERRINTLVL_OFF_gc | TC_OVFINTLVL_OFF_gc;

// Compare/Capture channel A interrupt: Disabled
// Compare/Capture channel B interrupt: Disabled
// Compare/Capture channel C interrupt: Disabled
// Compare/Capture channel D interrupt: Disabled
TCC0.INTCTRLB=TC_CCDINTLVL_OFF_gc | TC_CCCINTLVL_OFF_gc | TC_CCBINTLVL_OFF_gc | TC_CCAINTLVL_OFF_gc;

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
AWEXC.CTRL=(0<<AWEX_PGM_bp) | (0<<AWEX_CWCM_bp) | (0<<AWEX_DTICCDEN_bp) | (0<<AWEX_DTICCCEN_bp) |
    (0<<AWEX_DTICCBEN_bp) | (0<<AWEX_DTICCAEN_bp);

// Fault protection initialization
// Fault detection on OCD Break detection: On
// Fault detection restart mode: Latched Mode
// Fault detection action: None (Fault protection disabled)
AWEXC.FDCTRL=(AWEXC.FDCTRL & (~(AWEX_FDDBD_bm | AWEX_FDMODE_bm | AWEX_FDACT_gm))) |
    (0<<AWEX_FDDBD_bp) | (0<<AWEX_FDMODE_bp) | AWEX_FDACT_NONE_gc;
// Fault detect events:
// Event channel 0: Off
// Event channel 1: Off
// Event channel 2: Off
// Event channel 3: Off
// Event channel 4: Off
// Event channel 5: Off
// Event channel 6: Off
// Event channel 7: Off
AWEXC.FDEMASK=0b00000000;
// Make sure the fault detect flag is cleared
AWEXC.STATUS|=AWEXC.STATUS & AWEX_FDF_bm;

// Clear the interrupt flags
TCC0.INTFLAGS=TCC0.INTFLAGS;
// Set Counter register
TCC0.CNT=0x0000;
// Set Period register for the overflow interrupt
// to occur every 1000ms
TCC0.PER=_MCU_CLOCK_FREQUENCY_/1024;
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

