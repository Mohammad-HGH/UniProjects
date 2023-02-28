/*****************************************************************************
*
* Atmel Corporation
*
* File              : main.c
* Compiler          : CodeVisionAVR V2.xx
* Revision          : $Revision: 2.0 $
* Date              : $Date: Tuesday, November 29, $
* Updated by        : $Author: raapeland $
*
* Support mail      : avr@atmel.com
*
* Target platform   : All AVRs with bootloader support
*
* AppNote           : AVR109 - Self-programming
*
* Description   : This Program allows an AVR with bootloader capabilities to
*                 Read/write its own Flash/EEprom. To enter Programming mode
*                 an input pin is checked. If this pin is pulled low, programming mode
*                 is entered. If not, normal execution is done from $0000
*                 "reset" vector in Application area.
*
* Preparations  : Use the preprocessor.xls file for obtaining a customized
*                 defines.h file and linker-file code-segment definition for
*                 the device you are compiling for.
****************************************************************************/
#include <io.h>
#include "defines.h"
#include "serial.h"
#include "flash.h"


#ifndef EEWE
  #define EEWE EEPE
#endif
#ifndef EEMWE
  #define EEMWE EEMPE
#endif

/*** WARNING: The AVRprog application in AVR Studio does not support block mode,
     but it will try to use it anyway. Therefore, block mode must be disabled below
     if AVRprog is going to be used. ***/


/* Uncomment the following to save code space */
#define REMOVE_AVRPROG_SUPPORT
//#define REMOVE_FUSE_AND_LOCK_BIT_SUPPORT
//#define REMOVE_BLOCK_SUPPORT
//#define REMOVE_EEPROM_BYTE_SUPPORT
//#define REMOVE_FLASH_BYTE_SUPPORT



#ifndef REMOVE_BLOCK_SUPPORT
unsigned char BlockLoad(unsigned int size, unsigned char mem, long *address);
void BlockRead(unsigned int size, unsigned char mem, long *address);

/* BLOCKSIZE should be chosen so that the following holds: BLOCKSIZE*n = PAGESIZE,  where n=1,2,3... */
#define BLOCKSIZE PAGESIZE

#endif



void main(void)
{
	long address;
	unsigned int temp_int;
	unsigned char val;


	/* Initialization */
	void (*funcptr)( void ) = 0x0000; // Set up function pointer to RESET vector.
	PROGPORT |= (1<<PROG_NO); // Enable pull-up on PROG_NO line on PROGPORT.
	initbootuart(); // Initialize UART.

	/* Branch to bootloader or application code? */
	if( !(PROGPIN & (1<<PROG_NO)) ) // If PROGPIN is pulled low, enter programmingmode.
	{
		/* Main loop */
		for(;;)
		{
			val=recchar(); // Wait for command character.

			// Check autoincrement status.
			if(val=='a')
			{
				sendchar('Y'); // Yes, we do autoincrement.
			}


			// Set address.
			else if(val=='A') // Set address...
			{ // NOTE: Flash addresses are given in words, not bytes.
				unsigned char high = recchar();
				unsigned char low = recchar();
				address = ((long)high<<8) | (long)low;
				sendchar('\r'); // Send OK back.
			}

			// Set 24-bit address.
			else if(val=='H')
			{ // NOTE: Flash addresses are given in words, not bytes.
				unsigned char highest = recchar();
				unsigned char high = recchar();
				unsigned char low = recchar();
				address = ((long)highest << 16) | ((long)high << 8) | (long)low;
				sendchar('\r'); // Send OK back.
			}

			// Chip erase.
			else if(val=='e')
			{
				for(address = 0; address < APP_END;address += PAGESIZE)
				{ // NOTE: Here we use address as a byte-address, not word-address, for convenience.
					_WAIT_FOR_SPM();
#pragma diag_suppress=Pe1053 // Suppress warning for conversion from long-type address to flash ptr.
					_PAGE_ERASE( address );
#pragma diag_default=Pe1053 // Back to default.
				}

				sendchar('\r'); // Send OK back.
			}

#ifndef REMOVE_BLOCK_SUPPORT
			// Check block load support.
			else if(val=='b')
			{
				sendchar('Y'); // Report block load supported.
				sendchar((BLOCKSIZE>>8) & 0xFF); // MSB first.
				sendchar(BLOCKSIZE&0xFF); // Report BLOCKSIZE (bytes).
			}


			// Start block load.
			else if(val=='B')
			{
				temp_int = (recchar()<<8) | recchar(); // Get block size.
				val = recchar(); // Get memtype.
				sendchar( BlockLoad(temp_int,val,&address) ); // Block load.
			}


			// Start block read.
			else if(val=='g')
			{
				temp_int = (recchar()<<8) | recchar(); // Get block size.
				val = recchar(); // Get memtype
				BlockRead(temp_int,val,&address); // Block read
			}
#endif

#ifndef REMOVE_FLASH_BYTE_SUPPORT
			// Read program memory.
			else if(val=='R')
			{
				// Send high byte, then low byte of flash word.
				_WAIT_FOR_SPM();
				_ENABLE_RWW_SECTION();
#pragma diag_suppress=Pe1053 // Suppress warning for conversion from long-type address to flash ptr.
				sendchar( _LOAD_PROGRAM_MEMORY( (address << 1)+1 ) );
				sendchar( _LOAD_PROGRAM_MEMORY( (address << 1)+0 ) );
#pragma diag_default=Pe1053 // Back to default.

				address++; // Auto-advance to next Flash word.
			}


			// Write program memory, low byte.
			else if(val=='c')
			{ // NOTE: Always use this command before sending high byte.
				temp_int=recchar(); // Get low byte for later _FILL_TEMP_WORD.
				sendchar('\r'); // Send OK back.
			}


			// Write program memory, high byte.
			else if(val=='C')
			{
				temp_int |= (recchar()<<8); // Get and insert high byte.
				_WAIT_FOR_SPM();
#pragma diag_suppress=Pe1053 // Suppress warning for conversion from long-type address to flash ptr.
				_FILL_TEMP_WORD( (address << 1), temp_int ); // Convert word-address to byte-address and fill.
#pragma diag_default=Pe1053 // Back to default.
				address++; // Auto-advance to next Flash word.
				sendchar('\r'); // Send OK back.
			}


			// Write page.
			else if(val== 'm')
			{
				if( address >= (APP_END>>1) ) // Protect bootloader area.
				{
					sendchar('?');
				} else
				{
					_WAIT_FOR_SPM();
#pragma diag_suppress=Pe1053 // Suppress warning for conversion from long-type address to flash ptr.
					_PAGE_WRITE( address << 1 ); // Convert word-address to byte-address and write.
#pragma diag_default=Pe1053 // Back to default.
				}

				sendchar('\r'); // Send OK back.
			}
#endif

#ifndef REMOVE_EEPROM_BYTE_SUPPORT
			// Write EEPROM memory.
			else if (val == 'D')
			{
				_WAIT_FOR_SPM();
				EEARL = address; // Setup EEPROM address.
				EEARH = (address >> 8);
				EEDR = recchar(); // Get byte.
				EECR |= (1<<EEMWE); // Write byte.
				EECR |= (1<<EEWE);
				while (EECR & (1<<EEWE)) // Wait for write operation to finish.
					;

				address++; // Auto-advance to next EEPROM byte.
				sendchar('\r');// Send OK back.
			}


			// Read EEPROM memory.
			else if (val == 'd')
			{
				EEARL = address; // Setup EEPROM address.
				EEARH = (address >> 8);
				EECR |= (1<<EERE); // Read byte...
				sendchar(EEDR); // ...and send it back.
				address++; // Auto-advance to next EEPROM byte.
			}
#endif

#ifndef REMOVE_FUSE_AND_LOCK_BIT_SUPPORT
			// Write lockbits.
			else if(val=='l')
			{
				_WAIT_FOR_SPM();
				_SET_LOCK_BITS( recchar() ); // Read and set lock bits.
				sendchar('\r'); // Send OK back.
			}


			// Read lock bits.
			else if(val=='r')
			{
				_WAIT_FOR_SPM();
				sendchar( _GET_LOCK_BITS() );
			}


			// Read fuse bits.
			else if(val=='F')
			{
				_WAIT_FOR_SPM();
				sendchar( _GET_LOW_FUSES() );
			}


			// Read high fuse bits.
			else if(val=='N')
			{
				_WAIT_FOR_SPM();
				sendchar( _GET_HIGH_FUSES() );
			}


			// Read extended fuse bits.
			else if(val=='Q')
			{
				_WAIT_FOR_SPM();
				sendchar( _GET_EXTENDED_FUSES() );
			}
#endif

#ifndef REMOVE_AVRPROG_SUPPORT
			// Enter and leave programming mode.
			else if((val=='P')||(val=='L'))
			{
				sendchar('\r'); // Nothing special to do, just answer OK.
			}


			// Exit bootloader.
			else if(val=='E')
			{
				_WAIT_FOR_SPM();
				_ENABLE_RWW_SECTION();
				sendchar('\r');
				funcptr(); // Jump to Reset vector 0x0000 in Application Section.
			}


			// Get programmer type.
			else if (val=='p')
			{
				sendchar('S'); // Answer 'SERIAL'.
			}


			// Return supported device codes.
			else if(val=='t')
			{
				sendchar( PARTCODE ); // Supports only this device, of course.
				sendchar( 0 ); // Send list terminator.
			}


			// Set LED, clear LED and set device type.
			else if((val=='x')||(val=='y')||(val=='T'))
			{
				recchar(); // Ignore the command and it's parameter.
				sendchar('\r'); // Send OK back.
			}
#endif

			// Return programmer identifier.
			else if(val=='S')
			{
				sendchar('A'); // Return 'AVRBOOT'.
				sendchar('V'); // Software identifier (aka programmer signature) is always 7 characters.
				sendchar('R');
				sendchar('B');
				sendchar('O');
				sendchar('O');
				sendchar('T');
			}


			// Return software version.
			else if(val=='V')
			{
				sendchar('1');
				sendchar('5');
			}


			// Return signature bytes.
			else if(val=='s')
			{
				sendchar( SIGNATURE_BYTE_3 );
				sendchar( SIGNATURE_BYTE_2 );
				sendchar( SIGNATURE_BYTE_1 );
			}


			// The last command to accept is ESC (synchronization).
			else if(val!=0x1b)                  // If not ESC, then it is unrecognized...
			{
				sendchar('?');
			}
		} // end: for(;;)
	}
	else
	{
		_WAIT_FOR_SPM();
		_ENABLE_RWW_SECTION();
		funcptr(); // Jump to Reset vector 0x0000 in Application Section.
	}
} // end: main


#ifndef REMOVE_BLOCK_SUPPORT
/*! \brief  Write block from serial port and program mem.
 *
 *  Read 'size' bytes from serial port and write to EPROM/Flash
 *  memory 'address'.
 *  If 'mem' == 'E', then 'address' gives
 *  the byte address in EEPROM. If 'mem' == 'F', then 'address'
 *  gives the word address in Flash.
 */
unsigned char BlockLoad(unsigned int size, unsigned char mem, long *address)
{
	unsigned char buffer[BLOCKSIZE];
	unsigned int data;
	long tempaddress;

	// EEPROM memory type.
	if(mem=='E')
	{
		/* Fill buffer first, as EEPROM is too slow to copy with UART speed */
		for(tempaddress=0;tempaddress<size;tempaddress++)
			buffer[tempaddress] = recchar();

		/* Then program the EEPROM */
		_WAIT_FOR_SPM();
		for( tempaddress=0; tempaddress < size; tempaddress++)
		{
			EEARL = *address; // Setup EEPROM address
			EEARH = ((*address) >> 8);
			EEDR = buffer[tempaddress]; // Get byte.
			EECR |= (1<<EEMWE); // Write byte.
			EECR |= (1<<EEWE);
			while (EECR & (1<<EEWE)) // Wait for write operation to finish.
				;

			(*address)++; // Select next EEPROM byte
		}

		return '\r'; // Report programming OK
	}

	// Flash memory type.
	else if(mem=='F')
	{ // NOTE: For flash programming, 'address' is given in words.
		(*address) <<= 1; // Convert address to bytes temporarily.
		tempaddress = (*address);  // Store address in page.

		do
		{
			data = recchar();
			data |= (recchar() << 8);
#pragma diag_suppress=Pe1053 // Suppress warning for conversion from long-type address to flash ptr.
			_FILL_TEMP_WORD(*address,data);
#pragma diag_default=Pe1053 // Back to default.
			(*address)+=2; // Select next word in memory.
			size -= 2; // Reduce number of bytes to write by two.
		} while(size); // Loop until all bytes written.

		if( tempaddress < (APP_END) ) { // Protect bootloader area.
#pragma diag_suppress=Pe1053 // Suppress warning for conversion from long-type address to flash ptr.
			_PAGE_WRITE(tempaddress);
#pragma diag_default=Pe1053 // Back to default.
			_WAIT_FOR_SPM();
		}
		_ENABLE_RWW_SECTION();

		(*address) >>= 1; // Convert address back to Flash words again.
		return '\r'; // Report programming OK
	}

	// Invalid memory type?
	else
	{
		return '?';
	}
}



/*! \brief  Read block of mem and send to serial port.
 *
 *  Read 'size' bytes from EEPROM/Flash memory 'address' and
 *  send to serial port. If 'mem' == 'E', then 'address' gives
 *  the byte address in EEPROM. If 'mem' == 'F', then 'address'
 *  gives the word address in Flash.
 */
void BlockRead(unsigned int size, unsigned char mem, long *address)
{
	// EEPROM memory type.
	if (mem=='E') // Read EEPROM
	{
		do
		{
			EEARL = *address; // Setup EEPROM address
			EEARH = ((*address) >> 8);
			(*address)++; // Select next EEPROM byte
			EECR |= (1<<EERE); // Read EEPROM
			sendchar(EEDR); // Transmit EEPROM dat ato PC

			size--; // Decrease number of bytes to read
		} while (size); // Repeat until all block has been read
	}

	// Flash memory type.
	else if(mem=='F')
	{
		(*address) <<= 1; // Convert address to bytes temporarily.

		do
		{
#pragma diag_suppress=Pe1053 // Suppress warning for conversion from long-type address to flash ptr.
			sendchar( _LOAD_PROGRAM_MEMORY(*address) );
			sendchar( _LOAD_PROGRAM_MEMORY((*address)+1) );
#pragma diag_default=Pe1053 // Back to default.
			(*address) += 2; // Select next word in memory.
			size -= 2; // Subtract two bytes from number of bytes to read
		} while (size); // Repeat until all block has been read

		(*address) >>= 1; // Convert address back to Flash words again.
	}
}
#endif


/* end of file */

