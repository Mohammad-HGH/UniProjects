/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief  XMEGA AES driver example source.
 *
 *      This file contains an example application that demonstrates the AES
 *      driver. It shows how to use AES module
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
#define BLOCK_COUNT   9

/* Interrupt driver */
AES_interrupt_driver_t interrupt_driver;

/* Key used when AES encryption is done operations. (single AES only uses the first 8 values). */
uint8_t  key[BLOCK_LENGTH] = { 0x94, 0x74, 0xB8, 0xE8, 0xC7, 0x3B, 0xCA, 0x7D,
                               0x28, 0x34, 0x76, 0xAB, 0x38, 0xCF, 0x37, 0xC2};

/* \brief Key used when the AES shall decrypt. This key is a modified version of the key. */
uint8_t  lastsubkey[BLOCK_LENGTH];

/*! \brief Initialisation vector (IV) used during Cipher Block Chaining. */
uint8_t  init[BLOCK_LENGTH] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
                               0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};

/*! \brief Plain text used during Cipher Block Chaining.
 *
 *  This data block need to be exactly [BLOCK_LENGTH * BLOCK_COUNT] bytes.
 */
uint8_t  data_block[BLOCK_LENGTH * BLOCK_COUNT] =
{
  "AVR1318: Using the XMEGA built-in AES accelerator."
  "This is a little textstring which will be encrypted"
  "and decrypted with CBC in the AES module."
};

/*! \brief Variable used to store ciphertext from Cipher Block Chaining. */
uint8_t cipher_block_ans[BLOCK_LENGTH * BLOCK_COUNT];

/*! \brief Variable used to store decrypted plaintext from Cipher Block Chaining. */
uint8_t block_ans[BLOCK_LENGTH * BLOCK_COUNT];

/* Variable used to check if decrypted answer is equal original data*/
bool success;

void main(void)
{
    uint16_t i;
	/* Assume that everything is ok*/
	success = true;

	/* Before using the AES it is recommended to do an AES software reset to put
	 * the module in known state, in case other parts of your code has accessed
	 * the AES module. */
	AES_software_reset();

	/* Initialize AES interrupt driver for encryption. */
	AES_interrupt_driver_init(&interrupt_driver,
	                          data_block,
	                          cipher_block_ans,
	                          key,
	                          init,
	                          BLOCK_COUNT,
	                          false);

	/* Enable PMIC interrupt to level low. */
	PMIC.CTRL |= PMIC_LOLVLEN_bm;

	/* Enable global interrupts. */
	sei();

	/* Start interrupt driver. */
	success = AES_interrupt_driver_start(&interrupt_driver, AES_INTLVL_LO_gc);

	if(success){
		do{
		/* Wait until all the encryption is finished. */
		}while(!AES_interrupt_driver_finished(&interrupt_driver));
	}else{
		while(true){
			/* If the example ends up here something is wrong. */
                        nop();
		}
	}

	/* Before doing a decryption generate and store the last sub key of the
	 * extended key. */
	success = AES_lastsubkey_generate(key, lastsubkey);

	/* Initialize AES interrupt driver for decryption. */
	AES_interrupt_driver_init(&interrupt_driver,
	                          cipher_block_ans,
	                          block_ans,
	                          lastsubkey,
	                          init,
	                          BLOCK_COUNT,
	                          true);

	/* Start interrupt driver. */
	success = AES_interrupt_driver_start(&interrupt_driver, AES_INTLVL_LO_gc);

	if(success){
		do{
			/* Wait until the decryption is finished. */
		}while(!AES_interrupt_driver_finished(&interrupt_driver));
	}

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


/*! \brief Interrupt vector proceccing data after one encrypt/decrypt complete. */
ISR(AES_INT_vect)
{
	/* Run AES interrupts according to interrupt_driver. */
	AES_interrupt_handler(&interrupt_driver);
}
