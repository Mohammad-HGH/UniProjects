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

#ifndef _AES_INCLUDED_
#define _AES_INCLUDED_

#pragma keep+
// Buffer that must hold the 128bit AES key
extern unsigned char AESKey[16]; 
#pragma keep-

/******************************************************************************
 Encrypt the 16 byte block pointed by pblock under the 128 bit key from AESKey
 Note that calling this function modifies AESKey. Therefore, before encrypting
 a second block with the same key AESKey has to be restored.
 This can be done by calling the function AESKeyRewind, but it is faster to
 simply backup the  original key somewhere to RAM and to restore it after
 calling AESEncrypt128.
******************************************************************************/
void AESEncrypt128(void *pblock);

/******************************************************************************
 Decrypt the 16 byte block pointed by pblock under the 128 bit key from AESKey
 The decryption key has to be preprocessed by AESKeyDecPreprocess before
 calling this funtion. Like in AESEncrypt128 AESKey is modified by this 
 function, but the key can be restored by calling AESKeyDecPreprocess. Again,
 backing up the key to RAM will be faster.

 Note that AESKeyRewind and AESKeyDecPreprocess are the inverses of each
 other. In other words: if encryption and decryption are performed in 
 strictly alternating order, the calls to AESKeyRewind and AESKeyPreprocess
 can be ommitted.
******************************************************************************/
void AESDecrypt128(void *pblock);

/******************************************************************************
 Rewind the key given in AESKey. See AESEncrypt128 for more details.
******************************************************************************/
void AESKeyRewind(void);

/******************************************************************************
 Preprocess the key given in AESKey for use for decryption. See AESDecrypt128
 for more details.
******************************************************************************/
void AESKeyDecPreprocess(void);

#endif

