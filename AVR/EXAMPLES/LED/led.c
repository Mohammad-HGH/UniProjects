/*******************************************************************************************
   Moving LED example

   CodeVisionAVR C Compiler
   (C) 2000-2013 HP InfoTech S.R.L.
   www.hpinfotech.ro

   Chip: ATmega328P running at 16MHz
   Memory Model: SMALL
   Data Stack Size: 512 bytes
   Development board: Arduino UNO

   - Connect the cathodes of 8 LEDs to the outputs marked DIGITAL 0..7 on the board.
     These outputs correspond to PORTD pins PD0..PD7 of the microcontroller.
   - Connect each LED’s anode, using a 1k resistor, to the pin marked 5V of
     the board’s POWER connector header.
   - Connect the USB connector of the Arduino UNO board to an USB port of your computer.
     This will provide serial communication and power supply to the board.
   - Open the Project|Configure|After Build menu and select the correct COM
     port used for communication with the Arduino UNO board.
   - Do Project|Build All and press the Program the Chip button in
     the Information window that will be displayed after build.
*******************************************************************************************/

// I/O register definitions for ATmega328P
#include <io.h>

// Timer1 output compare A interrupt service routine
interrupt [TIM1_COMPA] void timer1_compa_isr(void)
{
// If all LEDs are off, light the first one
if (PORTD == 0xFF) PORTD = 0xFE;
// One of the LEDs is already lighted, turn it off and light the next one
else PORTD = (PORTD << 1) | 1;
}

void main(void)
{
// Crystal Oscillator division factor: 1
#pragma optsize-
CLKPR=(1<<CLKPCE);
CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif

// Port D initialization
// Function: Bit7=Out Bit6=Out Bit5=Out Bit4=Out Bit3=Out Bit2=Out Bit1=Out Bit0=Out
DDRD=(1<<DDD7) | (1<<DDD6) | (1<<DDD5) | (1<<DDD4) | (1<<DDD3) | (1<<DDD2) | (1<<DDD1) | (1<<DDD0);
// State: Bit7=1 Bit6=1 Bit5=1 Bit4=1 Bit3=1 Bit2=1 Bit1=1 Bit0=1
// All LEDs are initally off
PORTD=(1<<PORTD7) | (1<<PORTD6) | (1<<PORTD5) | (1<<PORTD4) | (1<<PORTD3) | (1<<PORTD2) | (1<<PORTD1) | (1<<PORTD0);

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 250.000 kHz
// Mode: CTC top=OCR1A
// OC1A output: Disconnected
// OC1B output: Disconnected
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer Period: 0.2 s
// Timer1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: On
// Compare B Match Interrupt: Off
TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10);
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0xC3;
OCR1AL=0x4F;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer/Counter 1 Interrupt(s) initialization
TIMSK1=(0<<ICIE1) | (0<<OCIE1B) | (1<<OCIE1A) | (0<<TOIE1);

// Global enable interrupts
#asm
    sei
#endasm

// The rest is done by TIMER1 overflow interrupts
while (1);
}
