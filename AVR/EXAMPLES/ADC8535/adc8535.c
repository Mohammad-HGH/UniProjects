/*
ADC example for the ATmega8535 on
the Atmel STK500 starter kit

CodeVisionAVR C Compiler
© Copyright 2001-2007 HP InfoTech S.R.L.
www.hpinfotech.ro

Set the following jumpers on the STK500 board:
VTARGET, AREF, RESET, XTAL1, OSCSEL: 1-2

Connect the PORTB and LEDS headers with
a 10 wire ribbon cable

Connect the ISP6PIN and SPROG3 headers with
a 6 wire ribbon cable

The ATmega8535 must be located on socket SCKT3100A3

In the AVR Studio Tools|STK500|Board window set:
- VTarget=5.0V
- ARef=5.0V
- Oscillator=3.6864MHz

Apply a positive DC voltage in the 0..5.0V range
between the PORTA header PA0 and GND pins
*/

// I/O register definitions for ATmega8535
#include <mega8535.h>
// delay functions
#include <delay.h>

#define ADC_VREF_TYPE 0x20

// ADC interrupt service routine
interrupt [ADC_INT] void adc_isr(void)
{
// The LEDs will display the 8 most
// semnificative ADC bits
PORTB=ADCH;
// 20ms delay
delay_ms(20);
// Start a new AD conversion
ADCSRA|=0x40;
}

void main(void)
{
// Port B initialization
PORTB=0xFF; // all outputs
DDRB=0xFF;  // all LEDs are initially off

// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
SFIOR=0x00;

// ADC initialization
// ADC Clock frequency: 57.600 kHz
// ADC Voltage Reference: AREF pin
// ADC High Speed Mode: Off
// ADC Auto Trigger Source: None
// Only the 8 most significant bits of
// the AD conversion result are used
// Select ADC input 0
ADMUX=ADC_VREF_TYPE;
ADCSRA=0x8E;
SFIOR&=0xEF;

// Global enable interrupts
#asm("sei")

// Start the first AD conversion
ADCSRA|=0x40;

// All the job is done by ADC interrupts
while (1);
}
