/******************************************************************************
 AES routines are Copyright (C) 2006 B. Poettering
 
 Implementation and demo for the CodeVisionAVR V2.05.1+ Compiler are
 Copyright (C) 2011 P. Haiduc, HP InfoTech s.r.l., www.hpinfotech.ro
 
 This program is free software you can redistribute and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation either version 2 of the License, or
 (at your option) any later version. Whenever you redistribute a copy
 of this document, make sure to include the copyright and license
 agreement without modification.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 The license text can be found here: http://www.gnu.org/licenses/gpl.txt

                             RijndaelFantastic

 This is a microcontroller implementation of the Rijndael block cipher, better
 known as AES. The target device class is Atmel's AVR, a family of very fast 
 and very powerful flash MCUs, operating at clock rates up to 16 MHz,  
 executing one instruction per clock cycle (16 MIPS). The implementation 
 given here is optimized for RAM requirement, and achieves an encryption 
 rate of about 63 kByte/sec (on a 16MHz MCU). Decryption is done with
 55 kByte/sec.
 
 The implemented algorithm is restricted to block and key sizes of 128 bit. 
 Larger key sizes can be obtained by altering the key scheduling code, which
 should be easy. 

 This source code consists of some routines and an example application, 
 which encrypts a certain plaintext and decrypts it afterwards with the
 same key. Comments in the code clarify the interaction between the key 
 expansion and the encryption/decryption routines.

 I encourage to read the following Rijndael-related papers/books/sites:
 [1] "The Design of Rijndael", Daemen & Rijmen, Springer, ISBN 3-540-42580-2
 [2] http://www.esat.kuleuven.ac.be/~rijmen/rijndael/
 [3] http://www.esat.kuleuven.ac.be/~rijmen/rijndael/rijndaeldocV2.zip
 [4] http://www.esat.kuleuven.ac.be/~rijmen/rijndael/atmal.zip
 [5] http://csrc.nist.gov/CryptoToolkit/aes/rijndael/

 [1] is *the* book about Rijndael, [2] is the official Rijndael homepage,
 [3] contains the complete Rijndael AES specification, [4] is another
 Rijndael-implementation for AVR MCUs (but much slower than this one, 
 taking 3815 clock cycles per encryption), [5] is the official NIST AES 
 site with further links.

 AVR and ATmega are registered trademarks by the ATMEL corporation.
 See http://www.atmel.com and http://www.atmel.com/products/avr/ for
 further details.
******************************************************************************/        

#include "aes.h"

#ifndef _ENHANCED_CORE_
#error The AES functions require AVR chips with enhanced core instructions
#endif

#asm
    .def ST11=r0
    .def ST21=r1
    .def ST31=r2
    .def ST41=r3
    .def ST12=r4
    .def ST22=r5
    .def ST32=r6        
    .def ST42=r7
    .def ST13=r8
    .def ST23=r9
    .def ST33=r10
    .def ST43=r11
    .def ST14=r12
    .def ST24=r13
    .def ST34=r14
    .def ST44=r15
    .def H1=r16
    .def H2=r17
    .def Rcon=r18
    .def XL=r26
    .def XH=r27
    .def YL=r28
    .def YH=r29
    .def ZL=r30
    .def ZH=r31
    
    .macro SAVER
    push ST11
    in   ST11,SREG
    push ST11
    cli
    push ST21
    push ST31
    push ST41
    push ST12
    push ST22
    push ST32
    push ST42
    push ST13
    push ST23
    push ST33
    push ST43
    push ST14
    push ST24
    push ST34
    push ST44
    push H1
    push H2
    push Rcon
    ldd zl,y+0
    ldd zh,y+1
    .endm
    
    .macro SAVEY
    push yl
    push yh
    .endm
    
    .macro RESTORER
    rcall CopyResult
    pop  Rcon
    pop  H2
    pop  H1
    pop  ST44
    pop  ST34
    pop  ST24
    pop  ST14
    pop  ST43
    pop  ST33
    pop  ST23
    pop  ST13
    pop  ST42
    pop  ST32
    pop  ST22
    pop  ST12
    pop  ST41
    pop  ST31
    pop  ST21
    pop  ST11
    out  SREG,ST11
    pop  ST11
    .endm
    
    .macro RESTOREY
    pop  yh
    pop  yl
    .endm
#endasm

#if (defined _MODEL_MEDIUM_) || (defined _MODEL_LARGE_)
#asm
    #define LPM_INST elpm
#endasm
#else
#asm
    #define LPM_INST lpm
#endasm
#endif
        
/******************************************************************************
 Encrypt the 16 byte block pointed by pblock under the 128 bit key from AESKey
 Note that calling this function modifies AESKey. Therefore, before encrypting
 a second block with the same key AESKey has to be restored.
 This can be done by calling the function AESKeyRewind, but it is faster to
 simply backup the  original key somewhere to RAM and to restore it after
 calling AESEncrypt128.
******************************************************************************/
#pragma warn-
void AESEncrypt128(void *pblock)
{
#asm
        SAVER
        SAVEY
        rcall CopyData
#endasm
#if (defined _MODEL_MEDIUM_) || (defined _MODEL_LARGE_)
#asm
        ldi r30,byte3(sbox<<1)
        out rampz,r30
#endasm
#endif
#asm
        ldi Rcon, 1
AESEnc1:
        rcall AddRoundKey
        rcall RAMIncKey128
        rcall ShiftRowsSubBytes
        cpi Rcon, 0x6c
        breq AESEnc2
        rcall MixColumns
        rjmp AESEnc1
AESEnc2:
        rcall AddRoundKey
        RESTOREY
        RESTORER
#endasm
}
#ifdef _WARNINGS_ON_
#pragma warn+
#endif

/******************************************************************************
 Rewind the key given in AESKey. See AESEncrypt128 for more details.
******************************************************************************/

void AESKeyRewind(void)
{
#asm
        push ST11
        in   ST11,SREG
        push ST11
        cli
        push H1
        push H2
        push Rcon
        SAVEY
        ldi  yl,low(_AESKey)
        ldi  yh,high(_AESKey)
        ldi Rcon, 0x36
AESKeyR:rcall RAMDecKey128
        cpi Rcon, 0
        brne AESKeyR
        RESTOREY
        pop Rcon
        pop H2
        pop H1
        pop ST11
        out SREG,ST11
        pop ST11
#endasm
}
        
/******************************************************************************
 Preprocess the key given in AESKey for use for decryption. See AESDecrypt128
 for more details.
******************************************************************************/

void AESKeyDecPreprocess(void)
{
#asm
        push ST11
        in   ST11,SREG
        push ST11
        cli
        push H1
        push H2
        push Rcon
        SAVEY
        ldi  yl,low(_AESKey)
        ldi  yh,high(_AESKey)
        ldi Rcon, 1
AESKeyF:rcall RAMIncKey128
        cpi Rcon, 0x6c
        brne AESKeyF
        RESTOREY
        pop Rcon
        pop H2
        pop H1
        pop ST11
        out SREG,ST11
        pop ST11
#endasm
}
                
/******************************************************************************
 Decrypt the 16 byte block pointed by pblock under the 128 bit key from AESKey
 The decryption key has to be preprocessed by AESKeyDecPreprocess before
 calling this funtion. Like in AESEncrypt128 AESKey is modified by this 
 function, but the key can be restored by calling AESKeyDecPreprocess.
 Again,
 backing up the key to RAM will be faster.

 Note that AESKeyRewind and AESKeyDecPreprocess are the inverses of each
 other. In other words: if encryption and decryption are performed in 
 strictly alternating order, the calls to AESKeyRewind and AESKeyPreprocess
 can be ommitted.
******************************************************************************/

#pragma warn-
void AESDecrypt128(void *pblock)
{
#asm
        SAVER
        SAVEY
        rcall CopyData
#endasm
#if (defined _MODEL_MEDIUM_) || (defined _MODEL_LARGE_)
#asm
        ldi r30,byte3(sbox<<1)
        out rampz,r30
#endasm
#endif
#asm    
        ldi Rcon, 0x36
        rcall AddRoundKey
        rcall RAMDecKey128
        rcall ShiftRowsSubBytesInverse
AESDec1:rcall AddRoundKey
        rcall RAMDecKey128
        rcall MixColumnsInverse
        rcall ShiftRowsSubBytesInverse
        cpi Rcon, 0
        brne AESDec1
        rcall AddRoundKey
        RESTOREY
        RESTORER
#endasm
}
#ifdef _WARNINGS_ON_
#pragma warn+
#endif

/******************************************************************************
 The following subroutines are for internal use only. They shouldn't be 
 called by any client application directly.
******************************************************************************/

#asm        
CopyData:
        ldi yl,low(_AESKey)
        ldi yh,high(_AESKey)
        ldi xl,0
        ldi xh,0
        ldi H1,16
CopyData0:
        ld  H2,z+
        st  x+,H2
        dec H1
        brne CopyData0
        ret
        
CopyResult:
        ldd zl,y+0
        ldd zh,y+1
        ldi xl,0
        ldi xh,0
        ldi H1,16
CopyResult0:
        ld  H2,x+
        st  z+,H2
        dec H1
        brne CopyResult0
        ret
        
AddRoundKey:        ; Touched registers: ST11-ST44, H1
        ld H1, Y
        eor ST11, H1
        ldd H1, Y+1
        eor ST21, H1
        ldd H1, Y+2
        eor ST31, H1
        ldd H1, Y+3
        eor ST41, H1
        ldd H1, Y+4
        eor ST12, H1
        ldd H1, Y+5
        eor ST22, H1
        ldd H1, Y+6
        eor ST32, H1
        ldd H1, Y+7
        eor ST42, H1
        ldd H1, Y+8
        eor ST13, H1
        ldd H1, Y+9
        eor ST23, H1
        ldd H1, Y+10
        eor ST33, H1
        ldd H1, Y+11
        eor ST43, H1
        ldd H1, Y+12
        eor ST14, H1
        ldd H1, Y+13
        eor ST24, H1
        ldd H1, Y+14
        eor ST34, H1
        ldd H1, Y+15
        eor ST44, H1
        ret

        
MixColumnsInverse:        ; Touched registers: ST11-ST44, H1, H2, Z
        ldi ZH, high(xtime<<1)
        mov ZL, ST11                ; u = xtime(xtime(a[0] ^ a[2]))
        eor ZL, ST31
        LPM_INST ZL, Z
        LPM_INST ZL, Z
        eor ST11, ZL                ; a[0] ^= u
        eor ST31, ZL                ; a[2] ^= u
        mov ZL, ST21                ; v = xtime(xtime(a[1] ^ a[3]))
        eor ZL, ST41
        LPM_INST ZL, Z
        LPM_INST ZL, Z
        eor ST21, ZL                ; a[1] ^= v
        eor ST41, ZL                ; a[3] ^= v

        mov ZL, ST12
        eor ZL, ST32
        LPM_INST ZL, Z
        LPM_INST ZL, Z
        eor ST12, ZL
        eor ST32, ZL        
        mov ZL, ST22
        eor ZL, ST42
        LPM_INST ZL, Z
        LPM_INST ZL, Z
        eor ST22, ZL
        eor ST42, ZL        

        mov ZL, ST13
        eor ZL, ST33
        LPM_INST ZL, Z
        LPM_INST ZL, Z
        eor ST13, ZL
        eor ST33, ZL        
        mov ZL, ST23
        eor ZL, ST43
        LPM_INST ZL, Z
        LPM_INST ZL, Z
        eor ST23, ZL
        eor ST43, ZL        

        mov ZL, ST14
        eor ZL, ST34
        LPM_INST ZL, Z
        LPM_INST ZL, Z
        eor ST14, ZL
        eor ST34, ZL        
        mov ZL, ST24
        eor ZL, ST44
        LPM_INST ZL, Z
        LPM_INST ZL, Z
        eor ST24, ZL
        eor ST44, ZL        


MixColumns:        ; Touched registers: ST11-ST44, H1, H2, Z
        ldi ZH, high(xtime<<1)
        mov H1, ST11                ; Tmp = a[0] ^ a[1] ^ a[2] ^ a[3]
        eor H1, ST21
        mov ZL, H1
        eor H1, ST31
        eor H1, ST41
        mov H2, ST11                ; save a[0] for later use
        LPM_INST ZL, Z                ; Tm = xtime(a[0] ^ a[1])
        eor ST11, ZL                ; a[0] ^= Tm ^ Tmp
        eor ST11, H1
        mov ZL, ST21                ; Tm = xtime(a[1] ^ a[2])
        eor ZL, ST31
        LPM_INST ZL, Z
        eor ST21, ZL                ; a[1] ^= Tm ^ Tmp
        eor ST21, H1
        mov ZL, ST31                ; Tm = xtime(a[2] ^ a[3])
        eor ZL, ST41
        LPM_INST ZL, Z
        eor ST31, ZL                ; a[2] ^= Tm ^ Tmp
        eor ST31, H1
        mov ZL, ST41                ; Tm = xtime(a[3] ^ a[0])
        eor ZL, H2
        LPM_INST ZL, Z
        eor ST41, ZL                ; a[3] ^= Tm ^ Tmp
        eor ST41, H1
        
        mov H1, ST12
        eor H1, ST22
        mov ZL, H1
        eor H1, ST32
        eor H1, ST42
        mov H2, ST12
        LPM_INST ZL, Z
        eor ST12, ZL
        eor ST12, H1
        mov ZL, ST22
        eor ZL, ST32
        LPM_INST ZL, Z
        eor ST22, ZL
        eor ST22, H1
        mov ZL, ST32
        eor ZL, ST42
        LPM_INST ZL, Z
        eor ST32, ZL
        eor ST32, H1
        mov ZL, ST42
        eor ZL, H2
        LPM_INST ZL, Z
        eor ST42, ZL
        eor ST42, H1
        
        mov H1, ST13
        eor H1, ST23
        mov ZL, H1
        eor H1, ST33
        eor H1, ST43
        mov H2, ST13
        LPM_INST ZL, Z
        eor ST13, ZL
        eor ST13, H1
        mov ZL, ST23
        eor ZL, ST33
        LPM_INST ZL, Z
        eor ST23, ZL
        eor ST23, H1
        mov ZL, ST33
        eor ZL, ST43
        LPM_INST ZL, Z
        eor ST33, ZL
        eor ST33, H1
        mov ZL, ST43
        eor ZL, H2
        LPM_INST ZL, Z
        eor ST43, ZL
        eor ST43, H1

        mov H1, ST14
        eor H1, ST24
        mov ZL, H1
        eor H1, ST34
        eor H1, ST44
        mov H2, ST14
        LPM_INST ZL, Z
        eor ST14, ZL
        eor ST14, H1
        mov ZL, ST24
        eor ZL, ST34
        LPM_INST ZL, Z
        eor ST24, ZL
        eor ST24, H1
        mov ZL, ST34
        eor ZL, ST44
        LPM_INST ZL, Z
        eor ST34, ZL
        eor ST34, H1
        mov ZL, ST44
        eor ZL, H2
        LPM_INST ZL, Z
        eor ST44, ZL
        eor ST44, H1
        ret
        
        
ShiftRowsSubBytes:        ; Touched registers: ST11-ST44, H1, Z
        ldi ZH, high(sbox<<1)
        mov ZL, ST11
        LPM_INST ST11, Z
        mov ZL, ST12
        LPM_INST ST12, Z
        mov ZL, ST13
        LPM_INST ST13, Z
        mov ZL, ST14
        LPM_INST ST14, Z
        mov H1, ST21
        mov ZL, ST22
        LPM_INST ST21, Z
        mov ZL, ST23
        LPM_INST ST22, Z
        mov ZL, ST24
        LPM_INST ST23, Z
        mov ZL, H1
        LPM_INST ST24, Z
        mov H1, ST31
        mov ZL, ST33
        LPM_INST ST31, Z
        mov ZL, H1
        LPM_INST ST33, Z
        mov H1, ST32
        mov ZL, ST34
        LPM_INST ST32, Z
        mov ZL, H1
        LPM_INST ST34, Z
        mov H1, ST44
        mov ZL, ST43
        LPM_INST ST44, Z
        mov ZL, ST42
        LPM_INST ST43, Z
        mov ZL, ST41
        LPM_INST ST42, Z
        mov ZL, H1
        LPM_INST ST41, Z
        ret        

        
ShiftRowsSubBytesInverse:        ; Touched registers: ST11-ST44, H1, Z
        ldi ZH, high(isbox<<1)
        mov ZL, ST11
        LPM_INST ST11, Z
        mov ZL, ST12
        LPM_INST ST12, Z
        mov ZL, ST13
        LPM_INST ST13, Z
        mov ZL, ST14
        LPM_INST ST14, Z
        mov H1, ST21
        mov ZL, ST24
        LPM_INST ST21, Z
        mov ZL, ST23
        LPM_INST ST24, Z
        mov ZL, ST22
        LPM_INST ST23, Z
        mov ZL, H1
        LPM_INST ST22, Z
        mov H1, ST31
        mov ZL, ST33
        LPM_INST ST31, Z
        mov ZL, H1
        LPM_INST ST33, Z
        mov H1, ST32
        mov ZL, ST34
        LPM_INST ST32, Z
        mov ZL, H1
        LPM_INST ST34, Z
        mov H1, ST44
        mov ZL, ST41
        LPM_INST ST44, Z
        mov ZL, ST42
        LPM_INST ST41, Z
        mov ZL, ST43
        LPM_INST ST42, Z
        mov ZL, H1
        LPM_INST ST43, Z
        ret        


RAMIncKey128:        ; Touched registers: Rcon, H1, H2, Z
        ldi ZH, high(sbox<<1)
        ldd H2, Y+12
        ldd ZL, Y+13
        LPM_INST ZL, Z
        eor ZL, Rcon
        lsl Rcon
        brcc PC+2
        ldi Rcon, 0x1b
        rcall RAMInc1
        ldd ZL, Y+13
        LPM_INST ZL, Z
        rcall RAMInc1
        ldd ZL, Y+13
        LPM_INST ZL, Z
        rcall RAMInc1
        mov ZL, H2
        LPM_INST ZL, Z
        rcall RAMInc1
        sbiw YL, 4
        ret
RAMInc1:ld H1, Y
         eor ZL, H1
         st Y+, ZL
         ldd H1, Y+3
         eor ZL, H1
         std Y+3, ZL
         ldd H1, Y+7
         eor ZL, H1
         std Y+7, ZL
         ldd H1, Y+11
         eor ZL, H1
         std Y+11, ZL
         ret

        
RAMDecKey128:        ; Touched registers: Rcon, H1, H2, Z
        ldi ZH, high(sbox<<1)
        ldi H1, 4
RAMDec1:ldd ZL, Y+12
        ldd H2, Y+8
        eor ZL, H2
        std Y+12, ZL
        ldd ZL, Y+4
        eor H2, ZL
        std Y+8, H2
        ld H2, Y+
        eor ZL, H2
        std Y+3, ZL
        dec H1
        brne RAMDec1
        ldd ZL, Y+8
        LPM_INST ZL, Z
        ld H1, -Y
        eor H1, ZL
        st Y, H1
        ldd ZL, Y+12
        LPM_INST ZL, Z
        ld H1, -Y
        eor H1, ZL
        st Y, H1
        ldd ZL, Y+12
        LPM_INST ZL, Z
        ld H1, -Y
        eor H1, ZL
        st Y, H1
        ldd ZL, Y+12
        LPM_INST ZL, Z
        ld H1, -Y
        eor H1, ZL
        eor H1, Rcon
        st Y, H1
        lsr Rcon
        cpi Rcon, 0x0d
        brne PC+2
        ldi Rcon, 0x80
        ret        
#endasm

/******************************************************************************
 SBOX and "xtime" tables
******************************************************************************/

#asm
    .CSEG
    .ORG (High(PC)+1)*0x100 ; ensure proper alignement
        
sbox:
    .db 0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76 
    .db 0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0 
    .db 0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15 
    .db 0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75 
    .db 0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84 
    .db 0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf 
    .db 0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8 
    .db 0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2 
    .db 0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73 
    .db 0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb 
    .db 0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79 
    .db 0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08 
    .db 0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a 
    .db 0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e 
    .db 0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf 
    .db 0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16 

isbox:
    .db 0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb 
    .db 0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb 
    .db 0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e 
    .db 0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25 
    .db 0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92 
    .db 0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84 
    .db 0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06 
    .db 0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b 
    .db 0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73 
    .db 0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e 
    .db 0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b 
    .db 0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4 
    .db 0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f 
    .db 0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef 
    .db 0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61 
    .db 0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d 

xtime:
    .db 0x00,0x02,0x04,0x06,0x08,0x0a,0x0c,0x0e,0x10,0x12,0x14,0x16,0x18,0x1a,0x1c,0x1e
    .db 0x20,0x22,0x24,0x26,0x28,0x2a,0x2c,0x2e,0x30,0x32,0x34,0x36,0x38,0x3a,0x3c,0x3e
    .db 0x40,0x42,0x44,0x46,0x48,0x4a,0x4c,0x4e,0x50,0x52,0x54,0x56,0x58,0x5a,0x5c,0x5e
    .db 0x60,0x62,0x64,0x66,0x68,0x6a,0x6c,0x6e,0x70,0x72,0x74,0x76,0x78,0x7a,0x7c,0x7e
    .db 0x80,0x82,0x84,0x86,0x88,0x8a,0x8c,0x8e,0x90,0x92,0x94,0x96,0x98,0x9a,0x9c,0x9e
    .db 0xa0,0xa2,0xa4,0xa6,0xa8,0xaa,0xac,0xae,0xb0,0xb2,0xb4,0xb6,0xb8,0xba,0xbc,0xbe
    .db 0xc0,0xc2,0xc4,0xc6,0xc8,0xca,0xcc,0xce,0xd0,0xd2,0xd4,0xd6,0xd8,0xda,0xdc,0xde
    .db 0xe0,0xe2,0xe4,0xe6,0xe8,0xea,0xec,0xee,0xf0,0xf2,0xf4,0xf6,0xf8,0xfa,0xfc,0xfe
    .db 0x1b,0x19,0x1f,0x1d,0x13,0x11,0x17,0x15,0x0b,0x09,0x0f,0x0d,0x03,0x01,0x07,0x05
    .db 0x3b,0x39,0x3f,0x3d,0x33,0x31,0x37,0x35,0x2b,0x29,0x2f,0x2d,0x23,0x21,0x27,0x25
    .db 0x5b,0x59,0x5f,0x5d,0x53,0x51,0x57,0x55,0x4b,0x49,0x4f,0x4d,0x43,0x41,0x47,0x45
    .db 0x7b,0x79,0x7f,0x7d,0x73,0x71,0x77,0x75,0x6b,0x69,0x6f,0x6d,0x63,0x61,0x67,0x65
    .db 0x9b,0x99,0x9f,0x9d,0x93,0x91,0x97,0x95,0x8b,0x89,0x8f,0x8d,0x83,0x81,0x87,0x85
    .db 0xbb,0xb9,0xbf,0xbd,0xb3,0xb1,0xb7,0xb5,0xab,0xa9,0xaf,0xad,0xa3,0xa1,0xa7,0xa5
    .db 0xdb,0xd9,0xdf,0xdd,0xd3,0xd1,0xd7,0xd5,0xcb,0xc9,0xcf,0xcd,0xc3,0xc1,0xc7,0xc5
    .db 0xfb,0xf9,0xff,0xfd,0xf3,0xf1,0xf7,0xf5,0xeb,0xe9,0xef,0xed,0xe3,0xe1,0xe7,0xe5
#endasm

