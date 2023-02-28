/*************************************************************
Sample program to test the ATxmega128A1 ADC in 12 Bit unsigned
free running polled mode.

The chip is clocked from the internal 2MHz RC oscillator.

Use a STK600 development board with STK600-TQFP100 and
STK600-RC100X-13 addapters

The STK600 programmer must be set in JTAG programming mode
in the Tools|Programmer menu.

Make sure that the VTARGET and VREF voltages are set to 3.6V
using AVR Studio.
The VTARGET LED on the STK600 board must be lighted.

Make the following connections on the STK600
PC3 - RS232 SPARE TXD.

Connect the STK600 RS232 output to the COM port of the PC
Make sure to configure the CodeVisionAVR Terminal
to 9600bps 8N1.

DC voltage in the 0-1.0V range must be applied between the
PORTA pins ADC0, ADC1 and ADC3 (used by ADC channels 0, 1
and 2) and GND.
ADC channel 3 is used to read the voltage of the
internal temperature reference.

The PORTA ADC7 pin must be connected to GND, because it is
used to read the ADC offset voltage, that must be compensated.
*************************************************************/

#define VREF 1000.0 // ADC reference voltage [mV]

// specify if the adca_sweep_read function will be used (1) or not (0)
#define USE_SWEEP 1

// I/O Registers definitions
#include <io.h>

// Standard Input/Output functions
#include <stdio.h>

// Delay functions
#include <delay.h>

#include <stdint.h>

// Ports initialization
void ports_init(void)
{
// PORTA initialization
// OUT register
PORTA.OUT=0x00;
// Bit0: Input
// Bit1: Input
// Bit2: Input
// Bit3: Input
// Bit4: Input
// Bit5: Input
// Bit6: Input
// Bit7: Input
PORTA.DIR=0x00;
// Bit0 Output/Pull configuration: Totempole/No
// Bit0 Input/Sense configuration: Sense both edges
// Bit0 inverted: Off
// Bit0 slew rate limitation: Off
PORTA.PIN0CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit1 Output/Pull configuration: Totempole/No
// Bit1 Input/Sense configuration: Sense both edges
// Bit1 inverted: Off
// Bit1 slew rate limitation: Off
PORTA.PIN1CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit2 Output/Pull configuration: Totempole/No
// Bit2 Input/Sense configuration: Sense both edges
// Bit2 inverted: Off
// Bit2 slew rate limitation: Off
PORTA.PIN2CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit3 Output/Pull configuration: Totempole/No
// Bit3 Input/Sense configuration: Sense both edges
// Bit3 inverted: Off
// Bit3 slew rate limitation: Off
PORTA.PIN3CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit4 Output/Pull configuration: Totempole/No
// Bit4 Input/Sense configuration: Sense both edges
// Bit4 inverted: Off
// Bit4 slew rate limitation: Off
PORTA.PIN4CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit5 Output/Pull configuration: Totempole/No
// Bit5 Input/Sense configuration: Sense both edges
// Bit5 inverted: Off
// Bit5 slew rate limitation: Off
PORTA.PIN5CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit6 Output/Pull configuration: Totempole/No
// Bit6 Input/Sense configuration: Sense both edges
// Bit6 inverted: Off
// Bit6 slew rate limitation: Off
PORTA.PIN6CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit7 Output/Pull configuration: Totempole/No
// Bit7 Input/Sense configuration: Sense both edges
// Bit7 inverted: Off
// Bit7 slew rate limitation: Off
PORTA.PIN7CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
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
// OUT register
PORTC.OUT=0x08;
// Bit0: Input
// Bit1: Input
// Bit2: Input
// Bit3: Output
// Bit4: Input
// Bit5: Input
// Bit6: Input
// Bit7: Input
PORTC.DIR=0x08;
// Bit0 Output/Pull configuration: Totempole/No
// Bit0 Input/Sense configuration: Sense both edges
// Bit0 inverted: Off
// Bit0 slew rate limitation: Off
PORTC.PIN0CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit1 Output/Pull configuration: Totempole/No
// Bit1 Input/Sense configuration: Sense both edges
// Bit1 inverted: Off
// Bit1 slew rate limitation: Off
PORTC.PIN1CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit2 Output/Pull configuration: Totempole/No
// Bit2 Input/Sense configuration: Sense both edges
// Bit2 inverted: Off
// Bit2 slew rate limitation: Off
PORTC.PIN2CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit3 Output/Pull configuration: Totempole/No
// Bit3 Input/Sense configuration: Sense both edges
// Bit3 inverted: Off
// Bit3 slew rate limitation: Off
PORTC.PIN3CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit4 Output/Pull configuration: Totempole/No
// Bit4 Input/Sense configuration: Sense both edges
// Bit4 inverted: Off
// Bit4 slew rate limitation: Off
PORTC.PIN4CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit5 Output/Pull configuration: Totempole/No
// Bit5 Input/Sense configuration: Sense both edges
// Bit5 inverted: Off
// Bit5 slew rate limitation: Off
PORTC.PIN5CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit6 Output/Pull configuration: Totempole/No
// Bit6 Input/Sense configuration: Sense both edges
// Bit6 inverted: Off
// Bit6 slew rate limitation: Off
PORTC.PIN6CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
// Bit7 Output/Pull configuration: Totempole/No
// Bit7 Input/Sense configuration: Sense both edges
// Bit7 inverted: Off
// Bit7 slew rate limitation: Off
PORTC.PIN7CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
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
}

// USARTC0 initialization
// USARTC0 initialization
void usartc0_init(void)
{
// Note: the correct PORTC direction for the RxD, TxD and XCK signals
// is configured in the ports_init function

// Transmitter is enabled
// Set TxD=1
PORTC.OUTSET=0x08;

// Communication mode: Asynchronous USART
// Data bits: 8
// Stop bits: 1
// Parity: Disabled
USARTC0.CTRLC=USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc;

// Receive complete interrupt: Disabled
// Transmit complete interrupt: Disabled
// Data register empty interrupt: Disabled
USARTC0.CTRLA=(USARTC0.CTRLA & (~(USART_RXCINTLVL_gm | USART_TXCINTLVL_gm | USART_DREINTLVL_gm))) |
	USART_RXCINTLVL_OFF_gc | USART_TXCINTLVL_OFF_gc | USART_DREINTLVL_OFF_gc;

// Required Baud rate: 9600
// Real Baud Rate: 9601.0, Error: 0.0 %
USARTC0.BAUDCTRLA=0x85;
USARTC0.BAUDCTRLB=((0x09 << USART_BSCALE_bp) & USART_BSCALE_gm) | 0x0C;

// Receiver: Off
// Transmitter: On
// Double transmission speed mode: On
// Multi-processor communication mode: Off
USARTC0.CTRLB=(USARTC0.CTRLB & (~(USART_RXEN_bm | USART_TXEN_bm | USART_CLK2X_bm | USART_MPCM_bm | USART_TXB8_bm))) |
	USART_TXEN_bm | USART_CLK2X_bm;
}

// Write a character to the USARTC0 Transmitter
// USARTC0 is used as the default output device by the 'putchar' function
#define _ALTERNATE_PUTCHAR_

#pragma used+
void putchar(char c)
{
while ((USARTC0.STATUS & USART_DREIF_bm) == 0);
USARTC0.DATA=c;
}
#pragma used-

// Function used to read the calibration byte from the
// signature row, specified by 'index'
#pragma optsize-
unsigned char read_calibration_byte(unsigned char index)
{
unsigned char r;
NVM.CMD=NVM_CMD_READ_CALIB_ROW_gc;
r=*((flash unsigned char*) index);
// Clean up NVM command register
NVM.CMD=NVM_CMD_NO_OPERATION_gc;
return r;
}
#pragma optsize_default

// ADCA initialization

// Variable used to store the ADC offset
// for 12 Bit Unsigned conversion mode
unsigned char adca_offset;

void adca_init(void)
{
unsigned char i;
unsigned int offs;

// ADCA is enabled
// Resolution: 12 Bits
// Load the calibration value for 12 Bit resolution
// from the signature row
ADCA.CALL=read_calibration_byte(PROD_SIGNATURES_START+ADCACAL0_offset);
ADCA.CALH=read_calibration_byte(PROD_SIGNATURES_START+ADCACAL1_offset);

// Conversion mode: Unsigned
ADCA.CTRLB=(ADCA.CTRLB & (~(ADC_CONMODE_bm | ADC_FREERUN_bm | ADC_RESOLUTION_gm))) |
	ADC_RESOLUTION_12BIT_gc;

// Clock frequency: 125.000 kHz
ADCA.PRESCALER=(ADCA.PRESCALER & (~ADC_PRESCALER_gm)) | ADC_PRESCALER_DIV16_gc;

// Reference: Internal 1.00 V
// Temperature reference: On
ADCA.REFCTRL=(ADCA.REFCTRL & ((~(ADC_REFSEL_gm | ADC_TEMPREF_bm)) | ADC_BANDGAP_bm)) |
	ADC_REFSEL_INT1V_gc | ADC_TEMPREF_bm | ADC_BANDGAP_bm;

// Read and save the ADC offset using channel 0
// ADC7 pin connected to GND
ADCA.CH0.CTRL=(ADCA.CH0.CTRL & (~(ADC_CH_START_bm | ADC_CH_GAINFAC_gm | ADC_CH_INPUTMODE_gm))) |
	ADC_CH_INPUTMODE_SINGLEENDED_gc;
ADCA.CH0.MUXCTRL=(ADCA.CH0.MUXCTRL & (~(ADC_CH_MUXPOS_gm | ADC_CH_MUXNEG_gm))) |
	ADC_CH_MUXPOS_PIN7_gc;
// Enable the ADC in order to read the offset
ADCA.CTRLA|=ADC_ENABLE_bm;
// Insert a delay to allow the ADC common mode voltage to stabilize
delay_us(2);
// Perform several offset measurements and store the mean value
offs=0;
for (i=0; i<16; i++)
    {
    // Start the AD conversion on channel 0
    ADCA.CH0.CTRL|=ADC_CH_START_bm;
    // Wait for the AD conversion to complete
    while ((ADCA.CH0.INTFLAGS & ADC_CH_CHIF_bm)==0);
    // Clear the interrupt flag
    ADCA.CH0.INTFLAGS=ADC_CH_CHIF_bm;
    // Read the offset
    offs+=(unsigned char) ADCA.CH0.RESL;
    }
// Disable the ADC
ADCA.CTRLA&= ~ADC_ENABLE_bm;
// Store the mean value of the offset
adca_offset=(unsigned char) (offs/16);

// Initialize the ADC Compare register
ADCA.CMPL=0x00;
ADCA.CMPH=0x00;

// ADC channel 0 gain: 1
// ADC channel 0 input mode: Single-ended positive input signal
ADCA.CH0.CTRL=(ADCA.CH0.CTRL & (~(ADC_CH_START_bm | ADC_CH_GAINFAC_gm | ADC_CH_INPUTMODE_gm))) |
	ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_SINGLEENDED_gc;

// ADC channel 0 positive input: ADC0 pin
// ADC channel 0 negative input: GND
ADCA.CH0.MUXCTRL=(ADCA.CH0.MUXCTRL & (~(ADC_CH_MUXPOS_gm | ADC_CH_MUXNEG_gm))) |
	ADC_CH_MUXPOS_PIN0_gc;

// ADC channel 1 gain: 1
// ADC channel 1 input mode: Single-ended positive input signal
ADCA.CH1.CTRL=(ADCA.CH1.CTRL & (~(ADC_CH_START_bm | ADC_CH_GAINFAC_gm | ADC_CH_INPUTMODE_gm))) |
	ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_SINGLEENDED_gc;

// ADC channel 1 positive input: ADC1 pin
// ADC channel 1 negative input: GND
ADCA.CH1.MUXCTRL=(ADCA.CH1.MUXCTRL & (~(ADC_CH_MUXPOS_gm | ADC_CH_MUXNEG_gm))) |
	ADC_CH_MUXPOS_PIN1_gc;

// ADC channel 2 gain: 1
// ADC channel 2 input mode: Single-ended positive input signal
ADCA.CH2.CTRL=(ADCA.CH2.CTRL & (~(ADC_CH_START_bm | ADC_CH_GAINFAC_gm | ADC_CH_INPUTMODE_gm))) |
	ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_SINGLEENDED_gc;

// ADC channel 2 positive input: ADC2 pin
// ADC channel 2 negative input: GND
ADCA.CH2.MUXCTRL=(ADCA.CH2.MUXCTRL & (~(ADC_CH_MUXPOS_gm | ADC_CH_MUXNEG_gm))) |
	ADC_CH_MUXPOS_PIN2_gc;

// ADC channel 3 gain: 1
// ADC channel 3 input mode: Internal positive input signal
ADCA.CH3.CTRL=(ADCA.CH3.CTRL & (~(ADC_CH_START_bm | ADC_CH_GAINFAC_gm | ADC_CH_INPUTMODE_gm))) |
	ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_INTERNAL_gc;

// ADC channel 3 positive input: Temp. Reference
// ADC channel 3 negative input: GND
ADCA.CH3.MUXCTRL=(ADCA.CH3.MUXCTRL & (~(ADC_CH_MUXPOS_gm | ADC_CH_MUXNEG_gm))) |
	ADC_CH_MUXINT_TEMP_gc;

// ADC is free-running, sweeped channel(s): 0, 1, 2, 3
ADCA.EVCTRL=ADC_SWEEP_0123_gc | ADC_EVACT_NONE_gc;

// Channel 0 interrupt: Disabled
ADCA.CH0.INTCTRL=(ADCA.CH0.INTCTRL & (~(ADC_CH_INTMODE_gm | ADC_CH_INTLVL_gm))) |
	ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_OFF_gc;
// Channel 1 interrupt: Disabled
ADCA.CH1.INTCTRL=(ADCA.CH1.INTCTRL & (~(ADC_CH_INTMODE_gm | ADC_CH_INTLVL_gm))) |
	ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_OFF_gc;
// Channel 2 interrupt: Disabled
ADCA.CH2.INTCTRL=(ADCA.CH2.INTCTRL & (~(ADC_CH_INTMODE_gm | ADC_CH_INTLVL_gm))) |
	ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_OFF_gc;
// Channel 3 interrupt: Disabled
ADCA.CH3.INTCTRL=(ADCA.CH3.INTCTRL & (~(ADC_CH_INTMODE_gm | ADC_CH_INTLVL_gm))) |
	ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_OFF_gc;

// Free Running mode: On
ADCA.CTRLB|=ADC_FREERUN_bm;

// Enable the ADC
ADCA.CTRLA|=ADC_ENABLE_bm;
// Insert a delay to allow the ADC common mode voltage to stabilize
delay_us(2);
}

// ADCA channel data read function using polled mode
unsigned int adca_read(unsigned char channel)
{
ADC_CH_t *pch=&ADCA.CH0+channel;
unsigned int data;

// Wait for the AD conversion to complete
while ((pch->INTFLAGS & ADC_CH_CHIF_bm)==0);
// Clear the interrupt flag
pch->INTFLAGS=ADC_CH_CHIF_bm;
// Read the AD conversion result
((unsigned char *) &data)[0]=pch->RESL;
((unsigned char *) &data)[1]=pch->RESH;
// Compensate the ADC offset
data-=adca_offset;
return data;
}

// Sweeped channel(s): 0, 1, 2, 3 for ADCA
// Define the number of channels to be sweeped for ADCA
#define ADCA_SWEEPED_CHANNELS 4

// ADCA sweeped channel(s) data read function
void adca_sweep_read(unsigned int *pdata)
{
ADC_CH_t *pch=&ADCA.CH0;
unsigned char i;

// Read and store the AD conversion results
// for all the sweeped channels
for (i=0; i<ADCA_SWEEPED_CHANNELS; i++)
    {
    // Wait for the AD conversion to complete
    while ((pch->INTFLAGS & ADC_CH_CHIF_bm)==0);
    // Clear the interrupt flag
    pch->INTFLAGS=ADC_CH_CHIF_bm;
    // Read the AD conversion result
    ((unsigned char *) pdata)[0]=pch->RESL;
    ((unsigned char *) pdata)[1]=pch->RESH;
    // Compensate the ADC offset
    *pdata-=adca_offset;
    pdata++;
    pch++;
    }
}

void main(void)
{
// Declare your local variables here
unsigned char n;
// Array used to store the data read from ADC channels 0..3
unsigned int adc_data[ADCA_SWEEPED_CHANNELS];

// Ports initialization
ports_init();

// USARTC0 initialization
usartc0_init();

printf("ADCA polled mode test\n\r\n\r");

// ADCA initialization
adca_init();

printf("ADCA offset: %.1f mV\n\r\n\r",adca_offset*VREF/4095.0);

while (1)
      {
#     if USE_SWEEP
      // Sweep all channels and store the read data into the adc_data array
      adca_sweep_read(adc_data);
#     else
      // perform AD conversions on all channels and store the results in adc_data
      for (n=0; n<ADCA_SWEEPED_CHANNELS; n++)
          adc_data[n]=adca_read(n);
#     endif
      // Display the data read for channels 0..3
      for (n=0; n<ADCA_SWEEPED_CHANNELS; n++)
          printf("Channel %u -> %.1f mV\n\r",n,adc_data[n]*VREF/4095.0);
      printf("\n\r");
      // 1000ms delay
      delay_ms(1000);
      }
}
