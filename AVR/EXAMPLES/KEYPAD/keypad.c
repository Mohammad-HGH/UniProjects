/* 
  4x4 Keypad Demo

  CodeVisionAVR C Compiler
  (C) 2000-2010 HP InfoTech S.R.L.
  www.hpinfotech.ro

  Chip: ATmega8515
  
  PLEASE MAKE SURE THAT THE CKSEL0..3 FUSE
  BITS ARE PROGRAMMED TO USE THE EXTERNAL
  CLOCK SOURCE OF THE STK500 AND NOT
  THE INTERNAL 1MHz OSCILLATOR.
  The ATmega8515 chip comes from the factory
  with CKSEL0..3 fuse bits set to use the
  internal 1 MHz oscillator.

  Connect the keypad matrix as follows:
   
  [STK500 PORTD HEADER]   [KEYS]  R1
  1 PD0 -----0----1----2----3----~~~~~---o+5V
             |    |    |    |     R2   |
  2 PD1 -----4----5----6----7----~~~~~-
             |    |    |    |     R3   |
  3 PD2 -----8----9----10---11---~~~~~-
             |    |    |    |     R4   |
  4 PD3 -----12---13---14---15---~~~~~-
         D1  |    |    |    |
  5 PD4 -|<|-     |    |    |
         D2       |    |    |
  6 PD5 -|<|------     |    |
         D3            |    |
  7 PD6 -|<|-----------     |  R1..R4=10k..47k
         D4                 |
  8 PD7 -|<|----------------   D1..D4=1N4148
   
  Use an 2x16 alphanumeric LCD connected
  to PORTC as follows:

  [LCD]   [STK500 PORTC HEADER]
   1 GND- 9  GND
   2 +5V- 10 VCC  
   3 VLC- LCD contrast control voltage 0..1V
   4 RS - 1  PC0
   5 RD - 2  PC1
   6 EN - 3  PC2
  11 D4 - 5  PC4
  12 D5 - 6  PC5
  13 D6 - 7  PC6
  14 D7 - 8  PC7
*/

// Alphanumeric LCD Module functions
// The LCD connections are specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric menu
#include <alcd.h>
#include <stdio.h>
#include <delay.h>
#include <mega8515.h>

// quartz crystal frequency [Hz]
#define F_XTAL 3686400L
// PIND0..3 will be row inputs
#define KEYIN PIND
// PORTD4..7 will be column outputs
#define KEYOUT PORTD
// used for TIMER0 count initialization
#define INIT_TIMER0 TCNT0=0x100L-F_XTAL/64L/500L
#define FIRST_COLUMN 0x80
#define LAST_COLUMN 0x10

typedef unsigned char byte;
// store here every key state as a bit,
// bit 0 will be KEY0, bit 1 KEY1,...
unsigned keys;
// LCD display buffer
char buf[33];

// TIMER 0 interrupt at every 2 ms
interrupt [TIM0_OVF] void timer0_int(void)
{
static byte key_pressed_counter=20;
static byte key_released_counter,column=FIRST_COLUMN;
static unsigned row_data,crt_key;
// reinitialize TIMER0
INIT_TIMER0;
row_data<<=4;
// get a group of 4 keys in in row_data
row_data|=~KEYIN&0xf;
column>>=1;
if (column==(LAST_COLUMN>>1))
   {
   column=FIRST_COLUMN;
   if (row_data==0) goto new_key;
   if (key_released_counter) --key_released_counter;
   else
      {
      if (--key_pressed_counter==9) crt_key=row_data;
      else
         {
         if (row_data!=crt_key)
            {
            new_key:
            key_pressed_counter=10;
            key_released_counter=0;
            goto end_key;
            };
         if (!key_pressed_counter)
            {
            keys=row_data;
            key_released_counter=20;
            };
         };
      };
   end_key:;
   row_data=0;
   };
// select next column, inputs will be with pull-up
KEYOUT=~column;
}

// test if a key was pressed
unsigned inkey(void)
{
unsigned k;
if (k=keys) keys=0;
return k;
}

void init_keypad(void)
{
// PORT D initialization
// Bits 0..3 inputs
// Bits 4..7 outputs
DDRD=0xf0;
// Use pull-ups on bits 0..3 inputs
// Output 1 on 4..7 outputs
PORTD=0xff;
// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: 57.600 kHz
// Mode: Normal top=FFh
// OC0 output: Disconnected
TCCR0=0x03;
INIT_TIMER0;
OCR0=0x00;
// External Interrupts are off
MCUCR=0x00;
EMCUCR=0x00;
// Timer 0 overflow interrupt is on
TIMSK=0x02;
#asm("sei")
}

main() {
unsigned k;
init_keypad();
lcd_init(16);
lcd_putsf("CVAVR Keypad");
// read keys and display key code
while (1)
      {
      lcd_gotoxy(0,1);
      if (k=inkey())
         {
         sprintf(buf,"Key code=%Xh",k);
         lcd_puts(buf);
         }
      else lcd_putsf("NO KEY        ");
      delay_ms(500);
      }
}
