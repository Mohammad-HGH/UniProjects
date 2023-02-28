/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief  XMEGA AES driver example source.
 *
 *      This file contains an example application that demonstrates the AES
 *      driver using polled functions. Both single AES and Cipher Block Chaining
 *      AES encyption and decryption is done.
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
 * $Revision: 1569 $
 * $Date: 2008-04-22 13:03:43 +0200 (ti, 22 apr 2008) $  \n
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

/* Global Plaintext variable containing one block. */
uint8_t data[BLOCK_LENGTH] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                              0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16};

/* Variable used to store the result from a single AES encryption/decryption .*/
uint8_t single_ans[BLOCK_LENGTH];

/* Key used when AES encryption is done operations. */
uint8_t key[BLOCK_LENGTH] = {0x94, 0x74, 0xB8, 0xE8, 0xC7, 0x3B, 0xCA, 0x7D,
                             0x28, 0x34, 0x76, 0xAB, 0x38, 0xCF, 0x37, 0xC2};

/* Key used when the AES shall decrypt. This key is a modified version of the key. */
uint8_t  lastsubkey[BLOCK_LENGTH];

/* Initialisation vector used during Cipher Block Chaining. */
uint8_t init[BLOCK_LENGTH] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
                              0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};

/* Plain text used during Cipher Block Chaining(contains 3 blocks). */
uint8_t data_block[BLOCK_LENGTH * BLOCK_COUNT] =
                        {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                         0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00,
                         0xAB, 0xBA, 0x00, 0x00, 0xDE, 0xAD, 0x00, 0x00,
                         0xAB, 0xBA, 0x00, 0x00, 0xDE, 0xAD, 0x00, 0x00,
                         0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                         0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00};

/* Variable used to store ciphertext from Cipher Block Chaining. */
uint8_t  cipher_block_ans[BLOCK_LENGTH * BLOCK_COUNT];

/* Variable used to store decrypted plaintext from Cipher Block Chaining. */
uint8_t  block_ans[BLOCK_LENGTH * BLOCK_COUNT];

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

	/* Generate last subkey. */
	AES_lastsubkey_generate(key, lastsubkey);

	/* Do AES encryption and decryption of a single block. */
	success = AES_encrypt(data, single_ans, key);
	success = AES_decrypt(single_ans, single_ans, lastsubkey);

	/* Check if decrypted answer is equal to plaintext. */
	for(i = 0; i < BLOCK_LENGTH ; i++ ){
		if (data[i] != single_ans[i]){
			success = false;
		}
	}

	/* Do AES Cipher Block Chaining encryption and decryption on three blocks. */
	success = AES_CBC_encrypt(data_block, cipher_block_ans, key, init,
                                BLOCK_COUNT);
	success = AES_CBC_decrypt(cipher_block_ans, block_ans, lastsubkey,
                                init, BLOCK_COUNT);

	/* Check if decrypted answer is equal to plaintext. */
	for(i = 0; i < BLOCK_LENGTH * BLOCK_COUNT ; i++ ){
		if (data_block[i] != block_ans[i]){
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
