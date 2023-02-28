/******************************************************************************
Port of Atmel Application Note AVR1316 fot the CodeVisionAVR C Compiler
Copyright (C) 2009-2010 HP InfoTech s.r.l., Pavel Haiduc

Before using, please read the AVR1316 Application Note from:
www.atmel.com/dyn/resources/prod_documents/doc8070.pdf
******************************************************************************/

/******************************************************************************
*
* XMEGA Self-programming driver source file.
*
*      This file contains the low-level implementations for the
*      XMEGA Self-programming driver. It is written for the CodeVisionAVR V2 C Compiler.
*
*      The functions must be located in the device Boot section.
*
*      None of these routines clean up the NVM Command Register after use.
*      It is therefore important to write NVM_CMD_NO_OPERATION_gc (0x00) to this
*      register when you are finished using any of the functions in this driver.
*
*      For all routines, it is important that any interrupt handlers do not
*      perform any NVM operations. The user must implement a scheme for mutually
*      exclusive access to the NVM. However, the 4-cycle timeout will work fine,
*      since writing to the Configuration Change Protection register (CCP)
*      automatically disables interrupts for 4 instruction cycles.
*
*      Note on CodeVisionAVR calling convention:
*         Scratch registers:   R0,R1 R22,R23,R24,R25,R26,R27 R30,R31
*         Return registers:    R30, R31, R22, R23
*
* Application note:
*      AVR1316: XMEGA Self-programming
*
* Documentation
*      For comprehensive code documentation, supported compilers, compiler
*      settings and supported devices see readme.html
*
*      Atmel Corporation: http:www.atmel.com \n
*      Support email: avr@atmel.com
*
* $Revision: 0001 $
* $Date: 2009-06-08 $
*
* Copyright (c) 2007, Atmel Corporation All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* 3. The name of ATMEL may not be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
* SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES
* LOSS OF USE, DATA, OR PROFITS OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

#include <io.h>
#include <stdint.h>
#include <sp_driver.h>

/* Define the size of the flash page if not defined in the header files. */
#ifndef FLASH_PAGE_SIZE
//#error  FLASH_PAGE_SIZE must be defined if not defined in header files.
#define FLASH_PAGE_SIZE 512

#asm
#ifndef FLASH_PAGE_SIZE
#define FLASH_PAGE_SIZE 512
#endif
#endasm
#endif /*FLASH_PAGE_SIZE*/

#asm
; NVM commands
.equ NVM_CMD_NO_OPERATION_gc=0x00 ; Noop/Ordinary LPM
.equ NVM_CMD_READ_CALIB_ROW_gc=0x02 ; Read calibration row
.equ NVM_CMD_READ_USER_SIG_ROW_gc=0x03 ; Read user signature row
.equ NVM_CMD_READ_EEPROM_gc=0x06 ; Read EEPROM
.equ NVM_CMD_READ_FUSES_gc=0x07 ; Read fuse byte
.equ NVM_CMD_WRITE_LOCK_BITS_gc=0x08 ; Write lock bits
.equ NVM_CMD_ERASE_USER_SIG_ROW_gc=0x18 ; Erase user signature row
.equ NVM_CMD_WRITE_USER_SIG_ROW_gc=0x1A ; Write user signature row
.equ NVM_CMD_ERASE_APP_gc=0x20 ; Erase Application Section
.equ NVM_CMD_ERASE_APP_PAGE_gc=0x22 ; Erase Application Section page
.equ NVM_CMD_LOAD_FLASH_BUFFER_gc=0x23 ; Load Flash page buffer
.equ NVM_CMD_WRITE_APP_PAGE_gc=0x24 ; Write Application Section page
.equ NVM_CMD_ERASE_WRITE_APP_PAGE_gc=0x25 ; Erase-and-write Application Section page
.equ NVM_CMD_ERASE_FLASH_BUFFER_gc=0x26 ; Erase/flush Flash page buffer
.equ NVM_CMD_ERASE_BOOT_PAGE_gc=0x2A ; Erase Boot Section page
.equ NVM_CMD_WRITE_BOOT_PAGE_gc=0x2C ; Write Boot Section page
.equ NVM_CMD_ERASE_WRITE_BOOT_PAGE_gc=0x2D ; Erase-and-write Boot Section page
.equ NVM_CMD_ERASE_EEPROM_gc=0x30 ; Erase EEPROM
.equ NVM_CMD_ERASE_EEPROM_PAGE_gc=0x32 ; Erase EEPROM page
.equ NVM_CMD_LOAD_EEPROM_BUFFER_gc=0x33 ; Load EEPROM page buffer
.equ NVM_CMD_WRITE_EEPROM_PAGE_gc=0x34 ; Write EEPROM page
.equ NVM_CMD_ERASE_WRITE_EEPROM_PAGE_gc=0x35 ; Erase-and-write EEPROM page
.equ NVM_CMD_ERASE_EEPROM_BUFFER_gc=0x36 ; Erase/flush EEPROM page buffer
.equ NVM_CMD_APP_CRC_gc=0x38 ; Generate Application section CRC
.equ NVM_CMD_BOOT_CRC_gc=0x39 ; Generate Boot Section CRC
.equ NVM_CMD_FLASH_RANGE_CRC_gc=0x3A ; Generate Flash Range CRC

;CCP signature
.equ CCP_SPM_gc=0x9D ; SPM Instruction Protection
.equ CCP_IOREG_gc=0xD8 ; IO Register Protection

.equ NVM_CMDEX_bm=0x01 ; NVM.CTRLA Command Execute bit mask
#endasm

#ifdef _ELPM_ // if ELPM instruction is supported
#asm
    .macro LPMZ
    elpm   r@0,z
    .endm

    .macro LPMZ_INC
    elpm   r@0,z+
    .endm
#endasm
#else
#asm
    .macro LPMZ
    lpm   r@0,z
    .endm

    .macro LPMZ_INC
    lpm   r@0,z+
    .endm
#endasm
#endif

#pragma warn-

/*
 This routine is called by several other routines, and contains common code
 for executing an NVM command, including the return statement itself.

 If the operation (NVM command) requires the NVM Address registers to be
 prepared, this must be done before jumping to this routine.

 Input:
     R26 - NVM Command code.

 Returns:
     R30:R31:R22:R23 - 32-bit result from NVM operation.
*/
uint32_t SP_CommonCMD(void)
{
#asm
	sts	NVM_CMD, r26        ; Load command into NVM Command register.
	ldi	r30, CCP_IOREG_gc   ; Prepare Protect IO-register signature in R30.
	ldi	r31, NVM_CMDEX_bm   ; Prepare bitmask for setting NVM Command Execute bit into R31.
	sts	CCP, r30            ; Enable IO-register operation (this disables interrupts for 4 cycles).
	sts	NVM_CTRLA, r31      ; Load bitmask into NVM Control Register A, which executes the command.
#endasm
return *((uint32_t *) &NVM.DATA0) & 0xFFFFFF;
}

/*
This routine is called by several other routines, and contains common code
for executing an SPM command, including the return statement itself.

If the operation (SPM command) requires the R1:R0 registers to be
prepared, this must be done before jumping to this routine.

Input:
     R1:R0    - Optional input to SPM command.
     R30:R31  - Low bytes of Z pointer.
     R26      - NVM Command code.
     R23      - Saved RAMPZ

Returns:
     Nothing.
*/
void SP_CommonSPM(void)
{
#asm
	sts	NVM_CMD, r26     ; Load prepared command into NVM Command register.
	ldi	r26, CCP_SPM_gc  ; Prepare Protect SPM signature in R26.
	sts	CCP, r26         ; Enable SPM operation (this disables interrupts for 4 cycles).
	spm                   ; Self-program.
	out	RAMPZ, r23
#endasm
}

/*
This routine reads a byte from flash given by the address in 'address'
*/
uint8_t SP_ReadByte(uint32_t address)
{
#asm
    ld  r30,y             ; Move the low bytes to the Z pointer
    ldd r31,y+1
    ldd r22,y+2
	in	r23, RAMPZ        ; Save RAMPZ.
	out	RAMPZ, r22        ; Load RAMPZ with the MSB of the address.
	LPMZ 30               ; Extended load byte from address pointed to by Z.
	out	RAMPZ, r23        ; Restore RAMPZ.
#endasm
}

/*
This routine reads a word from flash given by the address in 'address'
*/
uint16_t SP_ReadWord(uint32_t address)
{
#asm
    ld  r30,y             ; Move the low bytes to the Z pointer
    ldd r31,y+1
    ldd r22,y+2
	in	r23, RAMPZ        ; Save RAMPZ.
	out	RAMPZ, r22        ; Load RAMPZ with the MSB of the address.
	LPMZ_INC 0            ; Extended load byte from address pointed to by Z.
	LPMZ 1                ; Extended load byte from address pointed to by Z.
    movw r30, r0
	out	RAMPZ, r23        ; Restore RAMPZ.
#endasm
}

/*
This routine reads the calibration byte given by the 'index'.
*/
uint8_t SP_ReadCalibrationByte(uint8_t index)
{
#asm
    ld  r30, y
    clr r31
	ldi	r26, NVM_CMD_READ_CALIB_ROW_gc    ; Prepare NVM command in R20.
	sts	NVM_CMD, r26    ; Load prepared command into NVM Command register.
	lpm	r30, Z          ; Load requested data into R30.
	sts	NVM_CMD, r31    ; Set NVM command to No Operation (0) so that LPM reads Flash.
#endasm
}

/*
This routine reads the user signature byte given by the 'index'.
*/
uint8_t SP_ReadUserSignatureByte(uint16_t index)
{
#asm
    ld  r30, y
    ldd r31, y+1
	ldi	r26, NVM_CMD_READ_USER_SIG_ROW_gc  ; Prepare NVM command in R20.
	sts	NVM_CMD, r26    ; Load prepared command into NVM Command register.
	lpm	r30, Z          ; Load requested data into R30.
	ldi	r26, NVM_CMD_NO_OPERATION_gc ; Prepare NVM command code in R26.
	sts	NVM_CMD, r26                 ; Set NVM command to No Operation so that LPM reads Flash.
#endasm
}

/*
This routine reads the fuse byte given by 'index'
*/
unsigned char SP_ReadFuseByte(uint8_t index)
{
#asm
    ld  r30, y
	sts	NVM_ADDR0, r30              ; Load fuse byte index into NVM Address Register 0.
	clr	r30                         ; Prepare a zero.
	sts	NVM_ADDR1, r30              ; Load zero into NVM Address Register 1.
	sts	NVM_ADDR2, r30              ; Load zero into NVM Address Register 2.
	ldi	r26, NVM_CMD_READ_FUSES_gc  ; Prepare NVM command in R26.
#endasm
SP_CommonCMD(); // Jump to common NVM Action code.
}

/*
This routine sets the lock bits. Note that unlocking is only
possible by doing a full chip erase, not available from software.
*/
void SP_WriteLockBits(uint8_t lock_bits)
{
#asm
    ld  r30, y
	sts	NVM_DATA0, r30                  ; Load lock bits into NVM Data Register 0.
	ldi	r26, NVM_CMD_WRITE_LOCK_BITS_gc ; Prepare NVM command in R26.
#endasm
SP_CommonCMD(); // Jump to common NVM Action code.
}

/*
This routine reads the lock bits.
*/
uint8_t SP_ReadLockBits(void)
{
return NVM.LOCKBITS; // Read IO-mapped lock bits.
}

/*
This routine erases the user signature row.
*/
void SP_EraseUserSignatureRow(void)
{
#asm
	in	r23, RAMPZ                         ; Save RAMPZ, which is restored in SP_CommonSPM.
	ldi	r26, NVM_CMD_ERASE_USER_SIG_ROW_gc ; Prepare NVM command in R26.
#endasm
SP_CommonSPM(); // Jump to common SPM code.
}

/*
This routine writes the flash buffer to the user signature row.
*/
void SP_WriteUserSignatureRow(void)
{
#asm
	in	r23, RAMPZ                          ; Save RAMPZ, which is restored in SP_CommonSPM.
	ldi	r26, NVM_CMD_WRITE_USER_SIG_ROW_gc  ; Prepare NVM command in R26.
#endasm
SP_CommonSPM(); // Jump to common SPM code.
}

/*
This routine erases the entire application section.
*/
void SP_EraseApplicationSection(void)
{
#asm
	in	r23, RAMPZ                 ; Save RAMPZ, which is restored in SP_CommonSPM.
	ldi	r26, NVM_CMD_ERASE_APP_gc  ; Prepare NVM command in R26.
#endasm
SP_CommonSPM(); // Jump to common SPM code.
}

/*
This routine erases the page at 'address' in the application
section. The address can point anywhere inside the page.
*/
void SP_EraseApplicationPage(uint32_t address)
{
#asm
    ld  r30, y             ; Move the low bytes to the Z pointer
    ldd r31, y+1
    ldd r22, y+2
	in	r23, RAMPZ                      ; Save RAMPZ, which is restored in SP_CommonSPM.
	out	RAMPZ, r22                      ; Load RAMPZ with the MSB of the address.
	ldi	r26, NVM_CMD_ERASE_APP_PAGE_gc  ; Prepare NVM command in R26.
#endasm
SP_CommonSPM(); // Jump to common SPM code.
}

/*
This routine writes the word 'data' into the Flash page buffer at 'address'.
*/
void SP_LoadFlashWord(uint16_t address, uint16_t data)
{
#asm
	in	r23, RAMPZ                         ; Save RAMPZ, which is restored in SP_CommonSPM.
	ld  r0, y                              ; Prepare flash word in R1:R0.
    ldd r1, y+1
    ldd r30, y+2                           ; Flash word address in the page in Z
    ldd r31, y+3
	ldi	r26, NVM_CMD_LOAD_FLASH_BUFFER_gc  ; Prepare NVM command in R26.
#endasm
SP_CommonSPM(); // Jump to common SPM code.
}

/*
This routine writes an entire page from the SRAM buffer at
address pointed by data into the Flash page buffer.
*/
void SP_LoadFlashPage(const uint8_t *data)
{
#asm
	clr	r30              ; Clear low byte of Z, to indicate start of page.
	clr	r31              ; Clear high byte of Z, to indicate start of page.
	ldi	r22, 0x00
	out	RAMPX, r22      ; Clear RAMPX pointer.
	ld  r26, y          ; Load X with data buffer address.
    ldd r27, y+1

	ldi r22, NVM_CMD_LOAD_FLASH_BUFFER_gc  ; Prepare NVM command code in R22.
	sts	NVM_CMD, r22                       ; Load it into NVM command register.
#endasm

#if FLASH_PAGE_SIZE > 512
#asm
	ldi	r25, ((FLASH_PAGE_SIZE/2) >> 8)
#endasm
#endif

#asm
	ldi	r24, ((FLASH_PAGE_SIZE/2)&0xFF)    ; Load R21 with page word count.
	ldi	r22, CCP_SPM_gc                    ; Prepare Protect SPM signature in R22.

SP_LoadFlashPage_1:
	ld	r0, X+         ; Load low byte from buffer into R0.
	ld	r1, X+         ; Load high byte from buffer into R1.
	sts	CCP, r22       ; Enable SPM operation (this disables interrupts for 4 cycles).
	spm                 ; Self-program.
	adiw r30, 2         ; Move Z to next Flash word.
#endasm

#if FLASH_PAGE_SIZE > 512
#asm
	sbiw r24, 1         ; Decrement word count.
#endasm
#else
#asm
	dec	r24            ; Decrement word count.
#endasm
#endif

#asm
	brne	SP_LoadFlashPage_1   ; Repeat until word cont is zero.
#endasm
}

/*
This routine reads an entire Flash page from 'address' into the
SRAM buffer at address pointed by data.
*/
void SP_ReadFlashPage(const uint8_t *data, uint32_t address)
{
#asm
	in	r23, RAMPZ                   ; Save RAMPZ during assembly.

	ld  r30, y                       ; Load Z with Flash address.
    ldd r31, y+1
    ldd r24, y+2
	out	RAMPZ, r24                   ; Load RAMPZ with address

	ldd r26, y+4                     ; Load X with data buffer address.
    ldd r27, y+5
	clr	r24
	out	RAMPX, r24                   ; Load RAMPX with data pointer

	ldi	r24, NVM_CMD_NO_OPERATION_gc ; Prepare NVM command code in R24.
	sts	NVM_CMD, r24                 ; Set NVM command to No Operation so that LPM reads Flash.
#endasm

#if FLASH_PAGE_SIZE > 512
#asm
	ldi	r25, ((FLASH_PAGE_SIZE/2) >> 8) ; Load R25 with byte cont high if flash page is large.
#endasm
#endif

#asm
	ldi	r24, ((FLASH_PAGE_SIZE)&0xFF)   ; Load R24 with byte count.

SP_ReadFlashPage_1:
	LPMZ_INC 0                         ; Load Flash bytes into R0:R1
	LPMZ_INC 1
	st	X+, r0                         ; Write bytes to buffer.
	st	X+, r1
#endasm

#if FLASH_PAGE_SIZE > 512
#asm
	sbiw r24, 1                          ; Decrement word count.
#endasm
#else
#asm
	dec	r24                             ; Decrement word count.
#endasm
#endif

#asm
	brne SP_ReadFlashPage_1              ; Repeat until byte count is zero.

	out	RAMPZ, r23                      ; Restore RAMPZ
#endasm
}

/*
This routine writes the page buffer to the Flash page at 'address'
in the application section. The address can point anywhere inside the page.
*/
void SP_WriteApplicationPage(uint32_t address)
{
#asm
	in	r23, RAMPZ                       ; Save RAMPZ, which is restored in SP_CommonSPM.
    ld  r30, y
    ldd r31, y+1
    ldd r22, y+2
	out	RAMPZ, r22
	ldi	r26, NVM_CMD_WRITE_APP_PAGE_gc   ; Prepare NVM command in R26.
#endasm
SP_CommonSPM(); // Jump to common SPM code.
}

/*
This routine erases first and then writes the page buffer to the
Flash page at 'address' in the application section. The address
can point anywhere inside the page.
*/
void SP_EraseWriteApplicationPage(uint32_t address)
{
#asm
	in	r23, RAMPZ                       ; Save RAMPZ, which is restored in SP_CommonSPM.
    ld  r30, y
    ldd r31, y+1
    ldd r22, y+2
	out	RAMPZ, r22
	ldi	r26, NVM_CMD_ERASE_WRITE_APP_PAGE_gc  ; Prepare NVM command in R26.
#endasm
SP_CommonSPM(); // Jump to common SPM code.
}

/*
This routine flushes the Flash page buffer.
*/
void SP_EraseFlashBuffer(void)
{
#asm
	in	r23, RAMPZ                          ; Save RAMPZ, which is restored in SP_CommonSPM.
	ldi	r26, NVM_CMD_ERASE_FLASH_BUFFER_gc  ; Prepare NVM command in R26.
#endasm
SP_CommonSPM(); // Jump to common SPM code.
}

/*
This routine erases the page at 'address' in the Boot section. The
address can point anywhere inside the page.
*/
void SP_EraseBootPage(uint32_t address)
{
#asm
	in	r23, RAMPZ                         ; Save RAMPZ, which is restored in SP_CommonSPM.
    ld  r30, y
    ldd r31, y+1
    ldd r22, y+2
	out	RAMPZ, r22
	ldi	r26, NVM_CMD_ERASE_BOOT_PAGE_gc    ; Prepare NVM command in R26.
#endasm
SP_CommonSPM(); // Jump to common SPM code.
}

/*
This routine writes the page buffer to the Flash page at 'address'
in the BOOT section. The address can point anywhere inside the page.
*/
void SP_WriteBootPage(uint32_t address)
{
#asm
	in	r23, RAMPZ                         ; Save RAMPZ, which is restored in SP_CommonSPM.
    ld  r30, y
    ldd r31, y+1
    ldd r22, y+2
	out	RAMPZ, r22
	ldi	r26, NVM_CMD_WRITE_BOOT_PAGE_gc  ; Prepare NVM command in R26.
#endasm
SP_CommonSPM(); // Jump to common SPM code.
}

/*
This routine erases first and then writes the page buffer to the
Flash page at 'address' in the Boot section. The address
can point anywhere inside the page.
*/
void SP_EraseWriteBootPage(uint32_t address)
{
#asm
	in	r23, RAMPZ                             ; Save RAMPZ, which is restored in SP_CommonSPM.
    ld  r30, y
    ldd r31, y+1
    ldd r22, y+2
	out	RAMPZ, r22
	ldi	r26, NVM_CMD_ERASE_WRITE_BOOT_PAGE_gc  ; Prepare NVM command in R26.
#endasm
SP_CommonSPM(); // Jump to common SPM code.
}

/*
This routine calculates a CRC for the application section.
*/
uint32_t SP_ApplicationCRC(void)
{
#asm
	ldi	r26, NVM_CMD_APP_CRC_gc    ; Prepare NVM command in R26.
#endasm
SP_CommonCMD(); // Jump to common NVM Action code.
}

/*
This routine calculates a CRC for the Boot section.
*/
uint32_t SP_BootCRC(void)
{
#asm
	ldi	r26, NVM_CMD_BOOT_CRC_gc   ; Prepare NVM command in R26.
#endasm
SP_CommonCMD(); // Jump to common NVM Action code.
}

/*
This routine locks all further access to SPM operations until next reset.
*/
void SP_LockSPM(void)
{
CCP=CCP_IOREG_gc; // Enable IO-register operation (this disables interrupts for 4 cycles).
NVM.CTRLB=NVM_SPMLOCK_bm;   // Load bitmask into NVM Control Register B, which locks SPM.
}

/*
This routine wait for the SPM to finish and clears the command register.

Note that this routine is blocking, and will halt any execution until the SPM
is finished.
*/
void SP_WaitForSPM(void)
{
while (NVM.STATUS & NVM_NVMBUSY_bm); // Repeat check if busy bit is not cleared.
NVM.CMD=NVM_CMD_NO_OPERATION_gc; // Clear up command register to NO_OPERATION.
}

#ifdef _WARNINGS_ON_
#pragma warn+
#endif
