/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief  XMEGA AES back to back driver example source.
 *
 *      This file contains an example application that demonstrates the AES
 *      driver using back to back polled functions.
 *
 * \par Application note:
 *      AVR1318 Using the XMEGA built in AES accelerator
 *
 * \par Documentation
 *      For comprehensive code documentation, supported compilers, compiler
 *      settings and supported devices see readme.html
 *
 * \author
 *      Atmel Corporation: http://www.atmel.com \n
 *      Support email: avr@atmel.com
 *
 * $Revision: 891 $
 * $Date: 2007-11-15 13:52:12 +0100 (to, 15 nov 2007) $  \n
 *
 * Copyright (c) 2008, Atmel Corporation All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of ATMEL may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include "avr_compiler.h"
#include "AES_driver.h"
#include <stdint.h>

#define BLOCK_LENGTH  16
#define BLOCK_COUNT   3

/* Global Plaintext variables containing one block. */
uint8_t data1[BLOCK_LENGTH] ={0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                              0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16};

uint8_t data2[BLOCK_LENGTH] ={0xAA, 0xAA, 0xAA, 0xBB, 0xBB, 0xBB, 0xCC, 0xCC,
                              0xCC, 0xDD, 0xDD, 0xDD, 0xEE, 0xEE, 0xEE, 0x0F};

/* Variables used to store the result from a single AES encryption/decryption .*/
uint8_t single_ans1[BLOCK_LENGTH];
uint8_t single_ans2[BLOCK_LENGTH];

/* Key used when AES encryption is done operations. */
uint8_t key[BLOCK_LENGTH] = {0x94, 0x74, 0xB8, 0xE8, 0xC7, 0x3B, 0xCA, 0x7D,
                             0x28, 0x34, 0x76, 0xAB, 0x38, 0xCF, 0x37, 0xC2};


/* Variable used to check if decrypted answer is equal original data. */
bool success;


void main( void )
{
    uint8_t i;

  	/* Assume that everything is ok*/
  	success = true;

	/* Before using the AES it is recommended to do an AES software reset to put
	 * the module in known state, in case other parts of your code has accessed
	 * the AES module. */
	AES_software_reset();

	/* Do AES encryption and decryption every other time with the same key on
	 * two data blocks. */
	success = AES_encrypt(data1, single_ans1, key);
	success = AES_decrypt_backtoback(single_ans1, single_ans1);
	success = AES_encrypt_backtoback(data2, single_ans2);
	success = AES_decrypt_backtoback(single_ans2, single_ans2);

	/* Check if decrypted answers is equal to plaintext. */
	for(i = 0; i < BLOCK_LENGTH ; i++ ){
		if (data1[i] != single_ans1[i]){
			success = false;
		}
		if (data2[i] != single_ans2[i]){
			success = false;
		}
	}

	if(success){
	    	while(true){
			/* If the example ends up here every thing is ok. */
                        nop();
		}
	}else{
		while(true){
			/* If the example ends up here something is wrong. */
                        nop();
		}
	}
}

