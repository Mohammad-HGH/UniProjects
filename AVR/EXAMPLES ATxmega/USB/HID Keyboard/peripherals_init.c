/*******************************************************
Chip peripherals initialization created by the
CodeWizardAVR V3.13 Advanced
Automatic Program Generator
© Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project: USB HID Keyboard
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
// Low level interrupt: Off
// Round-robin scheduling for low level interrupt: Off
// Medium level interrupt: Off
// High level interrupt: On - used by the USB
// The interrupt vectors will be placed at the start of the Application FLASH section
n=PMIC_HILVLEN_bm;
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
