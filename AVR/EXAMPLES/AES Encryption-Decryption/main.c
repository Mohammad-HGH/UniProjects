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
******************************************************************************/        

/*
 The results of the AES functions are displayed on a
 2 rows x 16 characters LCD connected to PORTC on a STK500
 or similar development board.
 Chip type: ATmega8515 running at 8 MHz.
 
 The LCD connections are specified
 in the Project|Configure|C Compiler|Libaries|Alphanumeric LCD
 menu:
 
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
  
 Chip type and LCD connections can be easily modified in
 the Project|Configure|C Compiler menu.
*/
 
#include "aes.h"
#include <string.h>
#include <alcd.h>
#include <delay.h>

// Buffer that holds the 128bit AES key
unsigned char AESKey[16]={0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};

// text buffer size must be multiple of 16
char text_buf[16];

void main()
{
unsigned char i;
// initialize the LCD with 16 chars/row
lcd_init(16);
lcd_putsf("AES encryption\n& decryption");
// 2sec. delay
delay_ms(2000);
// copy some text to the encryption buffer
// ensure that the unused portion of the text will be filled with 0s
strncpy(text_buf,"Hello world",sizeof(text_buf));
// encrypt the plain text from text_buf
AESEncrypt128(text_buf);
// display encrypted text
lcd_clear();
lcd_putsf("Encrypted text:\n");
for (i=0;i<16;i++) lcd_putchar(text_buf[i]);
// 2sec. delay
delay_ms(2000);
// restore the AESKey as it was altered by the encryption process
AESKeyRewind();
// prepare the AESKey for decryption
AESKeyDecPreprocess();
// decrypt the encrypted text
AESDecrypt128(text_buf);
// now text_buf must contain "Hello world" again
lcd_clear();
lcd_putsf("Decrypted text:\n");
lcd_puts(text_buf);
// stop here
while (1);
}
