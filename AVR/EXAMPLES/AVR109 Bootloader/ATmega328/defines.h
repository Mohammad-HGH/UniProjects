//******************************************************************************
// Bootloader based on Atmel application note AVR109 communication protocol
// Target chips: ATmega168/328
//
// (C) 2010-2012 Pavel Haiduc, HP InfoTech s.r.l.,
// All rights reserved
//
// Compiler: CodeVisionAVR V2.60+
// Version: 1.00
//******************************************************************************

#ifndef	_DEFINES_INCLUDED_
#define	_DEFINES_INCLUDED_	
	
#include	<io.h>	

/* definitions for SPM control */		
#define	SPMCR_REG	SPMCSR
#asm
     #define WR_SPMCR_REG_R22 out 0x37,r22
#endasm

/* definitions for device recognition */		
#define	PARTCODE	0x44

#if defined _CHIP_ATMEGA168_ | defined _CHIP_ATMEGA168V_ | \
    defined _CHIP_ATMEGA168P_ | defined _CHIP_ATMEGA168PV_

#define	_ATMEGA168	// device select: _ATMEGAxxxx
#define	_B1024	// boot size select: _Bxxxx (words), powers of two only

#define	PAGESIZE	128
#define	APP_END	0x1C00

#define	SIGNATURE_BYTE_1	0x1E
#define	SIGNATURE_BYTE_2	0x94
#define	SIGNATURE_BYTE_3	0x06

#elif defined _CHIP_ATMEGA168A_ | defined _CHIP_ATMEGA168PA_

#define	_ATMEGA168	// device select: _ATMEGAxxxx
#define	_B1024	// boot size select: _Bxxxx (words), powers of two only

#define	PAGESIZE	128
#define	APP_END	0x1C00

#define	SIGNATURE_BYTE_1	0x1E
#define	SIGNATURE_BYTE_2	0x94
#define	SIGNATURE_BYTE_3	0x0B

#elif defined _CHIP_ATMEGA328_ | defined _CHIP_ATMEGA328P_ | defined _CHIP_ATMEGA328PV_

#define	_ATMEGA328	// device select: _ATMEGAxxxx
#define	_B1024	// boot size select: _Bxxxx (words), powers of two only

#define	PAGESIZE	128
#define	APP_END	0x3C00

#define	SIGNATURE_BYTE_1	0x1E
#define	SIGNATURE_BYTE_2	0x94
#define	SIGNATURE_BYTE_3	0x0F

#endif

#ifdef _MODEL_SMALL_
typedef unsigned int ADDR_t; // for ATmega328 16bit word FLASH address is enough
#else
typedef unsigned long ADDR_t;
#endif

#endif		
