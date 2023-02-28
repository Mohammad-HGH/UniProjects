/*************************************************************
Example program for interrupt driven SPI slave reception
using an ATxmega128A1 chip running at 2MHz from the internal
2 MHz RC oscillator.

The SPIC peripheral is used as a master to send data to the
SPID peripheral configured as slave.
SPIC performs polled transmission.
SPID performs interrupt driven reception.

Use a STK600 development board with STK600-TQFP100 and
STK600-RC100X-13 addapters.

The STK600 programmer must be set in JTAG programming
mode in the Tools|Programmer menu.

Make sure that the VTARGET and VREF voltages are set
to 3.6V using AVR Studio.
The VTARGET LED on the STK600 board must be lighted.

Make the following connections on the STK600:
PORTA header PA0..7 - LED0..7
PORTC header PC4 (SPIC, /SS)  - PORTD header PD4 (SPID, /SS)
PORTC header PC5 (SPIC, MOSI) - PORTD header PD5 (SPID, MOSI)
PORTC header PC6 (SPIC, MISO) - PORTD header PD6 (SPID, MISO)
PORTC header PC7 (SPIC, SCK)  - PORTD header PD7 (SPID, SCK)

On succesfull reception the LED7 will light.
In case of failure LED0 will light.
*************************************************************/

#include <io.h>

// Buffer that holds the bytes to be sent by the SPIC master
flash unsigned char tx_buffer[]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99};
// Buffer used to hold the bytes received by SPID slave
unsigned char rx_buffer[sizeof(tx_buffer)];
// Counts the number of bytes received by SPID slave
unsigned char rx_cnt=0;

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

// Internal 2 MHz RC oscillator initialization
// Enable the internal 2 MHz RC oscillator
OSC.CTRL|=OSC_RC2MEN_bm;

// System clock prescaler A division factor: 1
// System clock prescalers B & C division factors: B:1, C:1
// ClkPer4: 2000.000 kHz
// ClkPer2: 2000.000 kHz
// ClkPer:  2000.000 kHz
// ClkCPU:  2000.000 kHz
n=(CLK.PSCTRL & (~(CLK_PSADIV_gm | CLK_PSBCDIV1_bm | CLK_PSBCDIV0_bm))) |
	CLK_PSADIV_1_gc | CLK_PSBCDIV_1_1_gc;
CCP=CCP_IOREG_gc;
CLK.PSCTRL=n;

// Disable the autocalibration of the internal 2 MHz RC oscillator
DFLLRC2M.CTRL&= ~DFLL_ENABLE_bm;

// Wait for the internal 2 MHz RC oscillator to stabilize
while ((OSC.STATUS & OSC_RC2MRDY_bm)==0);

// Select the system clock source: 2 MHz Internal RC Osc.
n=(CLK.CTRL & (~CLK_SCLKSEL_gm)) | CLK_SCLKSEL_RC2M_gc;
CCP=CCP_IOREG_gc;
CLK.CTRL=n;

// Disable the unused oscillators: 32 MHz, 32 kHz, external clock/crystal oscillator, PLL
OSC.CTRL&= ~(OSC_RC32MEN_bm | OSC_RC32KEN_bm | OSC_XOSCEN_bm | OSC_PLLEN_bm);

// Peripheral Clock output: Disabled
// Note: the correct Disabled direction for the Peripheral Clock output
// is configured in the ports_init function
PORTCFG.CLKEVOUT=(PORTCFG.CLKEVOUT & (~PORTCFG_CLKOUT_gm)) | PORTCFG_CLKOUT_OFF_gc;

// Restore interrupts enabled/disabled state
SREG=s;
// Restore optimization for size if needed
#pragma optsize_default
}

// Ports initialization
void ports_init(void)
{
// PORTA initialization
// Bit0: Output
// Bit1: Output
// Bit2: Output
// Bit3: Output
// Bit4: Output
// Bit5: Output
// Bit6: Output
// Bit7: Output
PORTA.DIR=0xFF;
// Bit0 Input/Sense configuration: Sense both edges
// Bit0 Output/Pull configuration: Totempole/No
// Bit0 inverted: On
// Bit0 slew rate limitation: Off
PORTA.PIN0CTRL=PORT_INVEN_bm | PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit1 Input/Sense configuration: Sense both edges
// Bit1 Output/Pull configuration: Totempole/No
// Bit1 inverted: On
// Bit1 slew rate limitation: Off
PORTA.PIN1CTRL=PORT_INVEN_bm | PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit2 Input/Sense configuration: Sense both edges
// Bit2 Output/Pull configuration: Totempole/No
// Bit2 inverted: On
// Bit2 slew rate limitation: Off
PORTA.PIN2CTRL=PORT_INVEN_bm | PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit3 Input/Sense configuration: Sense both edges
// Bit3 Output/Pull configuration: Totempole/No
// Bit3 inverted: On
// Bit3 slew rate limitation: Off
PORTA.PIN3CTRL=PORT_INVEN_bm | PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit4 Input/Sense configuration: Sense both edges
// Bit4 Output/Pull configuration: Totempole/No
// Bit4 inverted: On
// Bit4 slew rate limitation: Off
PORTA.PIN4CTRL=PORT_INVEN_bm | PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit5 Input/Sense configuration: Sense both edges
// Bit5 Output/Pull configuration: Totempole/No
// Bit5 inverted: On
// Bit5 slew rate limitation: Off
PORTA.PIN5CTRL=PORT_INVEN_bm | PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit6 Input/Sense configuration: Sense both edges
// Bit6 Output/Pull configuration: Totempole/No
// Bit6 inverted: On
// Bit6 slew rate limitation: Off
PORTA.PIN6CTRL=PORT_INVEN_bm | PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit7 Input/Sense configuration: Sense both edges
// Bit7 Output/Pull configuration: Totempole/No
// Bit7 inverted: On
// Bit7 slew rate limitation: Off
PORTA.PIN7CTRL=PORT_INVEN_bm | PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// OUT register
// Set LED0..LED7 off
PORTA.OUT=0x00;
// Interrupt 0 level: Disabled
// Interrupt 1 level: Disabled
PORTA.INTCTRL=(PORTA.INTCTRL & (~(PORT_INT1LVL_gm | PORT_INT0LVL_gm))) |
	PORT_INT1LVL_OFF_gc | PORT_INT0LVL_OFF_gc;
// Bit0 pin change interrupt 0: Off
// Bit1 pin change interrupt 0: Off
// Bit2 pin change interrupt 0: Off
// Bit3 pin change interrupt 0: Off
// Bit4 pin change interrupt 0: Off
// Bit5 pin change interrupt 0: Off
// Bit6 pin change interrupt 0: Off
// Bit7 pin change interrupt 0: Off
PORTA.INT0MASK=0x00;
// Bit0 pin change interrupt 1: Off
// Bit1 pin change interrupt 1: Off
// Bit2 pin change interrupt 1: Off
// Bit3 pin change interrupt 1: Off
// Bit4 pin change interrupt 1: Off
// Bit5 pin change interrupt 1: Off
// Bit6 pin change interrupt 1: Off
// Bit7 pin change interrupt 1: Off
PORTA.INT1MASK=0x00;

// PORTC initialization
// Bit0: Input
// Bit1: Input
// Bit2: Input
// Bit3: Input
// Bit4: Output
// Bit5: Output
// Bit6: Input
// Bit7: Output
PORTC.DIR=0xB0;
// Bit0 Input/Sense configuration: Sense both edges
// Bit0 Output/Pull configuration: Totempole/No
// Bit0 inverted: Off
// Bit0 slew rate limitation: Off
PORTC.PIN0CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit1 Input/Sense configuration: Sense both edges
// Bit1 Output/Pull configuration: Totempole/No
// Bit1 inverted: Off
// Bit1 slew rate limitation: Off
PORTC.PIN1CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit2 Input/Sense configuration: Sense both edges
// Bit2 Output/Pull configuration: Totempole/No
// Bit2 inverted: Off
// Bit2 slew rate limitation: Off
PORTC.PIN2CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit3 Input/Sense configuration: Sense both edges
// Bit3 Output/Pull configuration: Totempole/No
// Bit3 inverted: Off
// Bit3 slew rate limitation: Off
PORTC.PIN3CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit4 Input/Sense configuration: Sense both edges
// Bit4 Output/Pull configuration: Totempole/No
// Bit4 inverted: Off
// Bit4 slew rate limitation: Off
PORTC.PIN4CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit5 Input/Sense configuration: Sense both edges
// Bit5 Output/Pull configuration: Totempole/No
// Bit5 inverted: Off
// Bit5 slew rate limitation: Off
PORTC.PIN5CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit6 Input/Sense configuration: Sense both edges
// Bit6 Output/Pull configuration: Totempole/No
// Bit6 inverted: Off
// Bit6 slew rate limitation: Off
PORTC.PIN6CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit7 Input/Sense configuration: Sense both edges
// Bit7 Output/Pull configuration: Totempole/No
// Bit7 inverted: Off
// Bit7 slew rate limitation: Off
PORTC.PIN7CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// OUT register
PORTC.OUT=0x10;
// Interrupt 0 level: Disabled
// Interrupt 1 level: Disabled
PORTC.INTCTRL=(PORTC.INTCTRL & (~(PORT_INT1LVL_gm | PORT_INT0LVL_gm))) |
	PORT_INT1LVL_OFF_gc | PORT_INT0LVL_OFF_gc;
// Bit0 pin change interrupt 0: Off
// Bit1 pin change interrupt 0: Off
// Bit2 pin change interrupt 0: Off
// Bit3 pin change interrupt 0: Off
// Bit4 pin change interrupt 0: Off
// Bit5 pin change interrupt 0: Off
// Bit6 pin change interrupt 0: Off
// Bit7 pin change interrupt 0: Off
PORTC.INT0MASK=0x00;
// Bit0 pin change interrupt 1: Off
// Bit1 pin change interrupt 1: Off
// Bit2 pin change interrupt 1: Off
// Bit3 pin change interrupt 1: Off
// Bit4 pin change interrupt 1: Off
// Bit5 pin change interrupt 1: Off
// Bit6 pin change interrupt 1: Off
// Bit7 pin change interrupt 1: Off
PORTC.INT1MASK=0x00;

// PORTD initialization
// Bit0: Output
// Bit1: Output
// Bit2: Output
// Bit3: Output
// Bit4: Input
// Bit5: Input
// Bit6: Output
// Bit7: Input
PORTD.DIR=0x4F;
// Bit0 Input/Sense configuration: Sense both edges
// Bit0 Output/Pull configuration: Totempole/No
// Bit0 inverted: Off
// Bit0 slew rate limitation: Off
PORTD.PIN0CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit1 Input/Sense configuration: Sense both edges
// Bit1 Output/Pull configuration: Totempole/No
// Bit1 inverted: Off
// Bit1 slew rate limitation: Off
PORTD.PIN1CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit2 Input/Sense configuration: Sense both edges
// Bit2 Output/Pull configuration: Totempole/No
// Bit2 inverted: Off
// Bit2 slew rate limitation: Off
PORTD.PIN2CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit3 Input/Sense configuration: Sense both edges
// Bit3 Output/Pull configuration: Totempole/No
// Bit3 inverted: Off
// Bit3 slew rate limitation: Off
PORTD.PIN3CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit4 Input/Sense configuration: Sense both edges
// Bit4 Output/Pull configuration: Totempole/No
// Bit4 inverted: Off
// Bit4 slew rate limitation: Off
PORTD.PIN4CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit5 Input/Sense configuration: Sense both edges
// Bit5 Output/Pull configuration: Totempole/No
// Bit5 inverted: Off
// Bit5 slew rate limitation: Off
PORTD.PIN5CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit6 Input/Sense configuration: Sense both edges
// Bit6 Output/Pull configuration: Totempole/No
// Bit6 inverted: Off
// Bit6 slew rate limitation: Off
PORTD.PIN6CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit7 Input/Sense configuration: Sense both edges
// Bit7 Output/Pull configuration: Totempole/No
// Bit7 inverted: Off
// Bit7 slew rate limitation: Off
PORTD.PIN7CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// OUT register
PORTD.OUT=0x00;
// Interrupt 0 level: Disabled
// Interrupt 1 level: Disabled
PORTD.INTCTRL=(PORTD.INTCTRL & (~(PORT_INT1LVL_gm | PORT_INT0LVL_gm))) |
	PORT_INT1LVL_OFF_gc | PORT_INT0LVL_OFF_gc;
// Bit0 pin change interrupt 0: Off
// Bit1 pin change interrupt 0: Off
// Bit2 pin change interrupt 0: Off
// Bit3 pin change interrupt 0: Off
// Bit4 pin change interrupt 0: Off
// Bit5 pin change interrupt 0: Off
// Bit6 pin change interrupt 0: Off
// Bit7 pin change interrupt 0: Off
PORTD.INT0MASK=0x00;
// Bit0 pin change interrupt 1: Off
// Bit1 pin change interrupt 1: Off
// Bit2 pin change interrupt 1: Off
// Bit3 pin change interrupt 1: Off
// Bit4 pin change interrupt 1: Off
// Bit5 pin change interrupt 1: Off
// Bit6 pin change interrupt 1: Off
// Bit7 pin change interrupt 1: Off
PORTD.INT1MASK=0x00;
}

// SPIC initialization
void spic_init(void)
{
// SPIC is enabled
// SPI mode: 0
// Operating as: Master
// Data order: MSB First
// SCK clock prescaler: 16
// SCK clock doubled: Off
// SCK clock frequency: 125.000 kHz
SPIC.CTRL=SPI_ENABLE_bm | SPI_MODE_0_gc | SPI_MASTER_bm |
	SPI_PRESCALER_DIV16_gc;

// SPIC interrupt: Disabled
SPIC.INTCTRL=(SPIC.INTCTRL & (~SPI_INTLVL_gm)) | SPI_INTLVL_OFF_gc;

// Note: the MOSI (PORTC Bit 5), SCK (PORTC Bit 7) and
// /SS (PORTC Bit 4) signals are configured as outputs in the ports_init function
}

// Macro used to drive the SPIC /SS signal low in order to select the slave
#define SET_SPIC_SS_LOW {PORTC.OUTCLR=SPI_SS_bm;}
// Macro used to drive the SPIC /SS signal high in order to deselect the slave
#define SET_SPIC_SS_HIGH {PORTC.OUTSET=SPI_SS_bm;}

// SPIC transmit/receive function in Master mode
// c - data to be transmitted
// Returns the received data
#pragma used+
unsigned char spic_master_tx_rx(unsigned char c)
{
// Transmit data in Master mode
SPIC.DATA=c;
// Wait for the data to be transmitted/received
while ((SPIC.STATUS & SPI_IF_bm)==0);
// Return the received data
return SPIC.DATA;
}
#pragma used-

// SPID initialization
void spid_init(void)
{
// SPID is enabled
// SPI mode: 0
// Operating as: Slave
// Data order: MSB First
SPID.CTRL=SPI_ENABLE_bm | SPI_MODE_0_gc;

// SPID interrupt: Low Level
SPID.INTCTRL=(SPID.INTCTRL & (~SPI_INTLVL_gm)) | SPI_INTLVL_LO_gc;

// Note: the MISO (PORTD Bit 6) signal is
// configured as output in the ports_init function
}

// SPID interrupt service routine
interrupt [SPID_INT_vect] void spid_isr(void)
{
// Store the received data in the receive buffer
// and increment the receive counter
rx_buffer[rx_cnt++]=SPID.DATA;
// Data to be transmitted back to the master
SPID.DATA=0;
}

void main(void)
{
// Declare your local variables here
unsigned char n;

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

// System clocks initialization
system_clocks_init();

// I/O Ports initialization
ports_init();

// SPIC initialization
spic_init();

// SPID initialization
spid_init();

// Globaly enable interrupts
#asm("sei")

// Use SPIC master to transmit the contents
// of the tx_buffer to the SPID slave
for (n=0; n<sizeof(tx_buffer); n++)
    {
    // Select the slave
    SET_SPIC_SS_LOW
    // Transmit a byte from the SPIC master to the SPID slave
    spic_master_tx_rx(tx_buffer[n]);
    // Deselect the slave
    SET_SPIC_SS_HIGH
    }

// Compare the contents of the SPID slave receive buffer
// with the SPIC master transmit buffer
for (n=0; n<sizeof(rx_buffer); n++)
    if (rx_buffer[n]!=tx_buffer[n])
       {
       // Mismatch, signal the error by lighting LED0 on the STK600
       PORTA.OUT=1<<0;
       // Stop here
       while (1);
       }

// The contents of the transmit and receive buffers match
// Signal that all is OK, by lighting LED7 on the STK600
PORTA.OUT=1<<7;

// stop here
while (1);
}
