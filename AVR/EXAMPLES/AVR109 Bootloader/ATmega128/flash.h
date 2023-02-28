/*****************************************************************************
*
* (C) 2010, HP InfoTech srl, www.hpinfotech.com
*
* File              : flash.h
* Compiler          : CodeVisionAVR V2.xx
* Revision          : $Revision: 1.0 $
* Date              : $Date: December 9, 2010 $
* Updated by        : $Author: HP InfoTech $
*
* Target platform   : All AVRs with bootloader support
*
* AppNote           : AVR109 - Self-programming
*
* Description       : Flash operations for AVR109 Self-programming
****************************************************************************/


#define _GET_LOCK_BITS() __AddrToZByteToSPMCR_LPM( (void flash *) 0x0001, 0x09 )
#define _GET_LOW_FUSES() __AddrToZByteToSPMCR_LPM( (void flash *) 0x0000, 0x09 )
#define _GET_HIGH_FUSES() __AddrToZByteToSPMCR_LPM( (void flash *) 0x0003, 0x09 )
#define _GET_EXTENDED_FUSES() __AddrToZByteToSPMCR_LPM( (void flash *) 0x0002, 0x09 )
#define _SET_LOCK_BITS(data) __DataToR0ByteToSPMCR_SPM( data, 0x09 )
#define _ENABLE_RWW_SECTION() __DataToR0ByteToSPMCR_SPM( 0x00, 0x11 )

#define _WAIT_FOR_SPM() while( SPMCR_REG & (1<<SPMEN) );

#define _LOAD_PROGRAM_MEMORY(addr) (*( (unsigned char flash *) (addr) ))

#ifdef _MODEL_SMALL_
  #define _FILL_TEMP_WORD(addr,data) __AddrToZWordToR1R0ByteToSPMCR_SPM( (void flash *) (addr), data, 0x01 )
  #define _PAGE_ERASE(addr) __AddrToZByteToSPMCR_SPM( (void flash *) (addr), 0x03 )
  #define _PAGE_WRITE(addr) __AddrToZByteToSPMCR_SPM( (void flash *) (addr), 0x05 )
#else
  #define _FILL_TEMP_WORD(addr,data) __AddrToZ24WordToR1R0ByteToSPMCR_SPM( (void flash *) (addr), data, 0x01 )
  #define _PAGE_ERASE(addr) __AddrToZ24ByteToSPMCR_SPM( (void flash *) (addr), 0x03 )
  #define _PAGE_WRITE(addr) __AddrToZ24ByteToSPMCR_SPM( (void flash *) (addr), 0x05 )
#endif

unsigned char __AddrToZByteToSPMCR_LPM(void flash *addr, unsigned char ctrl);
void __DataToR0ByteToSPMCR_SPM(unsigned char data, unsigned char ctrl);
void __AddrToZByteToSPMCR_SPM(void flash *addr, unsigned char ctrl);
void __AddrToZWordToR1R0ByteToSPMCR_SPM(void flash *addr, unsigned int data, unsigned char ctrl);
void __AddrToZ24WordToR1R0ByteToSPMCR_SPM(void flash *addr, unsigned int data, unsigned char ctrl);
void __AddrToZ24ByteToSPMCR_SPM(void flash *addr, unsigned char ctrl);
