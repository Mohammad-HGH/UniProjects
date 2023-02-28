//******************************************************************************
// Bootloader based on Atmel application note AVR109 communication protocol
// Target chips: ATmega168/328
// Chip clock frequency: 16MHz specified in the project configuration.
//
// (C) 2010-2012 Pavel Haiduc, HP InfoTech s.r.l.,
// All rights reserved
//
// Compiler: CodeVisionAVR V2.60+
// Version: 1.00
//******************************************************************************

/*
The bootloader expects to receive the '@' character in the first
5 seconds after reset and enters the bootloader mode and
responds with a '\r' character.
If the above condition is not met, execution starts from address 0.

It can be tested with the included avr109test.exe program.
*/

#include "defines.h"
#include "flash.h"
#include <stdio.h>

// Baud rate used for communication with the bootloader
#define	BAUD_RATE 115200
// baud rate register value calculation	
#define	BRREG_VALUE	(_MCU_CLOCK_FREQUENCY_/(8*BAUD_RATE)-1)

#define STARTUP_CHAR '@' // character used to start bootloader mode
#define STARTUP_DELAY 5  // period during which the bootloader expects
                         // to receive the STARTUP_CHAR character
                         // to enter bootloader mode
#define TIMER1_OVF_PERIOD 100 // time period between two timer 1 overflows [ms]
#define TIMER1_CLK_DIV 64 // value for timer 1 clock division coeficient
#define TIMER1_CNT_INIT (65536-(_MCU_CLOCK_FREQUENCY_*TIMER1_OVF_PERIOD)/(TIMER1_CLK_DIV*1000))
                        
// BLOCKSIZE should be chosen so that the following holds: BLOCKSIZE*n = PAGESIZE,  where n=1,2,3...
#define BLOCKSIZE PAGESIZE

unsigned char startup_delay_cnt;

// Timer1 overflow interrupt service routine
// Occurs every 100 ms
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
// Reinitialize Timer1 value
TCNT1H=TIMER1_CNT_INIT >> 8;
TCNT1L=TIMER1_CNT_INIT & 0xff;
// decrement the startup delay counter
if (startup_delay_cnt) --startup_delay_cnt;
}

unsigned char BlockLoad(unsigned int size, unsigned char mem, ADDR_t *address);
void BlockRead(unsigned int size, unsigned char mem, ADDR_t *address);

void main(void)
{
ADDR_t address;
unsigned int temp_int;
unsigned char val;

// Initialize USART
UCSR0A=(0<<RXC0) | (0<<TXC0) | (0<<UDRE0) | (0<<FE0) | (0<<DOR0) | (0<<UPE0) | (1<<U2X0) | (0<<MPCM0);
UCSR0B=(0<<RXCIE0) | (0<<TXCIE0) | (0<<UDRIE0) | (1<<RXEN0) | (1<<TXEN0) | (0<<UCSZ02) | (0<<RXB80) | (0<<TXB80);
UCSR0C=(0<<UMSEL01) | (0<<UMSEL00) | (0<<UPM01) | (0<<UPM00) | (0<<USBS0) | (1<<UCSZ01) | (1<<UCSZ00) | (0<<UCPOL0);
UBRR0H=BRREG_VALUE >> 8;
UBRR0L=BRREG_VALUE & 0xFF;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock divisor: 64
// Mode: Normal top=0xFFFF
// Timer Period: 100 ms
// Timer1 Overflow Interrupt: On
TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10);
TCNT1H=TIMER1_CNT_INIT >> 8;
TCNT1L=TIMER1_CNT_INIT & 0xFF;

// Timer/Counter 1 Interrupt(s) initialization
TIMSK1=(0<<ICIE1) | (0<<OCIE1B) | (0<<OCIE1A) | (1<<TOIE1);

startup_delay_cnt=STARTUP_DELAY*10;

// Global enable interrupts
#asm("sei")

// wait to receive the '@' character
while (startup_delay_cnt)
      {
      if (UCSR0A & (1<<RXC0))
         if (UDR0==STARTUP_CHAR) goto enter_bootloader_mode;
      }

// the startup delay elapsed without having received the
// STARTUP_CHAR character, so start execution from the application section
// stop Timer 1 as it's not needed anymore
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10);
TIMSK1=(0<<ICIE1) | (0<<OCIE1B) | (0<<OCIE1A) | (0<<TOIE1);

start_application:
// disable interrupts
#asm("cli")

#pragma optsize-
// will use the interrupt vectors from the application section
MCUCR=(1<<IVCE);
MCUCR=(0<<IVSEL) | (0<<IVCE);
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif

// start execution from address 0
#asm("jmp 0")
      
enter_bootloader_mode:
// stop Timer 1 as it's not needed anymore
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10);
TIMSK1=(0<<ICIE1) | (0<<OCIE1B) | (0<<OCIE1A) | (0<<TOIE1);

// send confirmation response
putchar('\r');

// main loop
while (1)
    {
        val=getchar(); // Wait for command character.

        // Check autoincrement status.
        if(val=='a')
        {
            putchar('Y'); // Yes, we do autoincrement.
        }


        // Set address.
        else if(val=='A') // Set address...
        { // NOTE: Flash addresses are given in words, not bytes.
            // Read address high and low byte.
            ((unsigned char *) &address)[1] = getchar(); // MSB
            ((unsigned char *) &address)[0] = getchar(); // LSB
            putchar('\r'); // Send OK back.
        }


        // Chip erase.
        else if(val=='e')
        {
            for(address = 0; address < APP_END;address += PAGESIZE)
            { // NOTE: Here we use address as a byte-address, not word-address, for convenience.
                _WAIT_FOR_SPM();
                _PAGE_ERASE( address );
            }

            putchar('\r'); // Send OK back.
        }

        // Check block load support.
        else if(val=='b')
        {
            putchar('Y'); // Report block load supported.
            putchar((BLOCKSIZE>>8) & 0xFF); // MSB first.
            putchar(BLOCKSIZE&0xFF); // Report BLOCKSIZE (bytes).
        }


        // Start block load.
        else if(val=='B')
        {
            // Get block size.
            ((unsigned char *) &temp_int)[1] = getchar(); // MSB
            ((unsigned char *) &temp_int)[0] = getchar(); // LSB
            putchar( BlockLoad(temp_int,getchar() /* Get mem. type */,&address) ); // Block load.
        }
		
        // Start block read.
        else if(val=='g')
        {
            // Get block size.
            ((unsigned char *) &temp_int)[1] = getchar(); // MSB
            ((unsigned char *) &temp_int)[0] = getchar(); // LSB
            BlockRead(temp_int,getchar() /* Get mem. type */,&address); // Block read
        }		

        // Read program memory.
        else if(val=='R')
        {
            // Send high byte, then low byte of flash word.
            _WAIT_FOR_SPM();
            _ENABLE_RWW_SECTION();
            putchar( _LOAD_PROGRAM_MEMORY( (address << 1)+1 ) );
            putchar( _LOAD_PROGRAM_MEMORY( (address << 1)+0 ) );

            address++; // Auto-advance to next Flash word.
        }


        // Write program memory, low byte.
        else if(val=='c')
        { // NOTE: Always use this command before sending high byte.
            temp_int=getchar(); // Get low byte for later _FILL_TEMP_WORD.
            putchar('\r'); // Send OK back.
        }


        // Write program memory, high byte.
        else if(val=='C')
        {
            // Get and insert high byte.
            ((unsigned char *) &temp_int)[1] = getchar(); // MSB
            _WAIT_FOR_SPM();
            _FILL_TEMP_WORD( (address << 1), temp_int ); // Convert word-address to byte-address and fill.
            address++; // Auto-advance to next Flash word.
            putchar('\r'); // Send OK back.
        }


        // Write page.
        else if(val== 'm')
        {
            if( address >= (APP_END>>1) ) // Protect bootloader area.
            {
                putchar('?');
            }
            else
            {
                _WAIT_FOR_SPM();
                _PAGE_WRITE( address << 1 ); // Convert word-address to byte-address and write.
            }

            putchar('\r'); // Send OK back.
        }

        // Write EEPROM memory.
        else if (val == 'D')
        {
            _WAIT_FOR_SPM();
            *((eeprom unsigned char *) address) = getchar(); // Write received byte.
            address++; // Auto-advance to next EEPROM byte.
            putchar('\r');// Send OK back.
        }


        // Read EEPROM memory.
        else if (val == 'd')
        {
            putchar(*((eeprom unsigned char *) address)); // Read byte send it back.
            address++; // Auto-advance to next EEPROM byte.
        }

        // Write lockbits.
        else if(val=='l')
        {
            _WAIT_FOR_SPM();
            _SET_LOCK_BITS( getchar() ); // Read and set lock bits.
            putchar('\r'); // Send OK back.
        }


        // Read lock bits.
        else if(val=='r')
        {
            _WAIT_FOR_SPM();
            putchar( _GET_LOCK_BITS() );
        }


        // Read fuse bits.
        else if(val=='F')
        {
            _WAIT_FOR_SPM();
            putchar( _GET_LOW_FUSES() );
        }


        // Read high fuse bits.
        else if(val=='N')
        {
            _WAIT_FOR_SPM();
            putchar( _GET_HIGH_FUSES() );
        }


        // Read extended fuse bits.
        else if(val=='Q')
        {
            _WAIT_FOR_SPM();
            putchar( _GET_EXTENDED_FUSES() );
        }

        // Enter and leave programming mode.
        else if((val=='P')||(val=='L'))
        {
            putchar('\r'); // Nothing special to do, just answer OK.
        }


        // Exit bootloader.
        else if(val=='E')
        {
            _WAIT_FOR_SPM();
            _ENABLE_RWW_SECTION();
            putchar('\r');
            // Jump to Reset vector 0x0000 in Application Section.
            goto start_application;
        }


        // Get programmer type.
        else if (val=='p')
        {
            putchar('S'); // Answer 'SERIAL'.
        }


        // Return supported device codes.
        else if(val=='t')
        {
            putchar( PARTCODE ); // Supports only this device, of course.
            putchar( 0 ); // Send list terminator.
        }


        // Set LED, clear LED and set device type.
        else if((val=='x')||(val=='y')||(val=='T'))
        {
            getchar(); // Ignore the command and it's parameter.
            putchar('\r'); // Send OK back.
        }

        // Return programmer identifier.
        else if(val=='S')
        {
            putchar('A'); // Return 'AVRBOOT'.
            putchar('V'); // Software identifier (aka programmer signature) is always 7 characters.
            putchar('R');
            putchar('B');
            putchar('O');
            putchar('O');
            putchar('T');
        }

        // Return software version.
        else if(val=='V')
        {
            putchar('1');
            putchar('0');
        }

        // Return signature bytes.
        else if(val=='s')
        {							
            putchar( SIGNATURE_BYTE_3 );
            putchar( SIGNATURE_BYTE_2 );
            putchar( SIGNATURE_BYTE_1 );
        }

        // The last command to accept is ESC (synchronization).
        else if(val!=0x1b)                  // If not ESC, then it is unrecognized...
        {
            putchar('?');
        }
    }

}

unsigned char BlockLoad(unsigned int size, unsigned char mem, ADDR_t *address)
{
unsigned int data;
ADDR_t tempaddress,addr;
unsigned char buffer[BLOCKSIZE];
    
addr=*address;
    
// EEPROM memory type.
if(mem=='E')
{
    /* Fill buffer first, as EEPROM is too slow to copy with UART speed */
    for(tempaddress=0;tempaddress<size;tempaddress++)
        buffer[tempaddress] = getchar();

    _WAIT_FOR_SPM();
    /* Then program the EEPROM */
    for( tempaddress=0; tempaddress < size; tempaddress++)
    {
        *((eeprom unsigned char *) addr++) = buffer[tempaddress]; // Write byte.
    }
        
    *address=addr;
    return '\r'; // Report programming OK
}

// Flash memory type.
if(mem=='F')
{ // NOTE: For flash programming, 'address' is given in words.
    addr <<= 1; // Convert address to bytes temporarily.
    tempaddress = addr;  // Store address in page.
	
    do
    {
        ((unsigned char *) &data)[0] = getchar(); // LSB
        ((unsigned char *) &data)[1] = getchar(); // MSB
        _FILL_TEMP_WORD(addr,data);
        addr += 2; // Select next word in memory.
        size -= 2; // Reduce number of bytes to write by two.
    } while(size); // Loop until all bytes written.

    _PAGE_WRITE(tempaddress);
    _WAIT_FOR_SPM();
    _ENABLE_RWW_SECTION();

    addr >>= 1; // Convert address back to Flash words again.
    *address=addr;
    return '\r'; // Report programming OK
}

// Invalid memory type?
return '?';
}


void BlockRead(unsigned int size, unsigned char mem, ADDR_t *address)
{
ADDR_t addr=*address;

// EEPROM memory type.
if (mem=='E') // Read EEPROM
{
    do
    {
        putchar(*((eeprom unsigned char *) addr++)); // Transmit EEPROM data to PC
    } while (--size); // Repeat until all block has been read
    *address=addr;
}

// Flash memory type.
else if(mem=='F')
{
    addr <<= 1; // Convert address to bytes temporarily.
	
    do
    {
        putchar( _LOAD_PROGRAM_MEMORY(addr++) );
        putchar( _LOAD_PROGRAM_MEMORY(addr++) );
        size -= 2; // Subtract two bytes from number of bytes to read
    } while (size); // Repeat until all block has been read

    addr >>= 1; // Convert address back to Flash words again.
    *address=addr;
}
}

