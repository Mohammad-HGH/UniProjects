//******************************************************************************
// Bootloader based on Atmel application note AVR109 communication protocol
//
// (C) 2010-2012 Pavel Haiduc, HP InfoTech s.r.l.,
// All rights reserved
//
// Compiler: CodeVisionAVR V2.60+
// Version: 1.00
//******************************************************************************

#include "defines.h"

#pragma warn-

unsigned char __AddrToZByteToSPMCR_LPM(void flash *addr, unsigned char ctrl)
{
#asm
     ldd  r30,y+1
     ldd  r31,y+2
     ld   r22,y
     WR_SPMCR_REG_R22
     lpm
     mov  r30,r0
#endasm
}

void __DataToR0ByteToSPMCR_SPM(unsigned char data, unsigned char ctrl)
{
#asm
     ldd  r0,y+1
     ld   r22,y
     WR_SPMCR_REG_R22
     spm
#endasm
}

void __AddrToZWordToR1R0ByteToSPMCR_SPM(void flash *addr, unsigned int data, unsigned char ctrl)
{
#asm
     ldd  r30,y+3
     ldd  r31,y+4
     ldd  r0,y+1
     ldd  r1,y+2
     ld   r22,y
     WR_SPMCR_REG_R22
     spm
#endasm
}

void __AddrToZByteToSPMCR_SPM(void flash *addr, unsigned char ctrl)
{
#asm
     ldd  r30,y+1
     ldd  r31,y+2
     ld   r22,y
     WR_SPMCR_REG_R22
     spm
#endasm
}

void __AddrToZ24WordToR1R0ByteToSPMCR_SPM(void flash *addr, unsigned int data, unsigned char ctrl)
{
#asm
     ldd  r30,y+3
     ldd  r31,y+4
     ldd  r22,y+5
     out  rampz,r22
     ldd  r0,y+1
     ldd  r1,y+2
     ld   r22,y
     WR_SPMCR_REG_R22
     spm
#endasm
}

void __AddrToZ24ByteToSPMCR_SPM(void flash *addr, unsigned char ctrl)
{
#asm
     ldd  r30,y+1
     ldd  r31,y+2
     ldd  r22,y+3
     out  rampz,r22
     ld   r22,y
     WR_SPMCR_REG_R22
     spm
#endasm
}

#ifdef _WARNINGS_ON_
#pragma warn+
#endif

