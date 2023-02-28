/* This file has been prepared for Doxygen automatic documentation generation. */
/*! \file *********************************************************************
 *
 * \brief The XMEGA Quadrature Decoder example source code.
 *
 *      This file contains example code that demonstrate the quadrature
 *      decoder. It shows how to set up the event system and the Timer/Counter
 *      to decode the angle of rotation from a quadrature encoder.
 *
 * \par Application note:
 *      AVR1600: Using the XMEGA Quadrature Decoder
 *
 * \par Documentation
 *      For comprehensive code documentation, supported compilers, compiler
 *      settings and supported devices see readme.html
 *
 * \author
 *      Atmel Corporation: http://www.atmel.com \n
 *      Support email: avr@atmel.com
 *
 * $Revision: 1699 $
 * $Date: 2008-07-30 09:20:10 +0200 (on, 30 jul 2008) $
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
#include "qdec_driver.h"
#include "qdec_signal_generator.h"

/*! \brief  If GENERATE_TEST_SIGNAL is defined the system generates a test signal
 *          with frequency equal to freq (RPM = freq*60).
 */
//#define GENERATE_TEST_SIGNAL

/*! \brief Output frequency when system generates a signal*/
uint8_t freq = 60;

/*! \brief Direction of the output signal*/
bool    dir  = 0;

/*! \brief Defines the clock division for the timer used.
 *         if changed both defines NEED to be changed correspondingly.
 */
#define CLOCK_DIV_bm  TC_CLKSEL_DIV64_gc
#define CLOCK_DIV     64

/*! \brief Number of lines in the quadrature encoder. */
uint8_t lineCount = 30;

/*! \brief Global frequency variable. */
uint16_t captureFreq  = 0;
uint16_t calcFreq     = 0;
uint16_t calcRPM      = 0;

/*! \brief Quadrature decoding example.
 *
 *  This is the main example code demonstrating the setup and configuration
 *  to make the quadrature decoder work. The example use the event system and
 *  a Timer/Counter to decode the signals and the counter value in the
 *  Timer/Counter will reflect the angle of rotation. The direction bit in the
 *  TC reflect the current direction of rotation.
 *
 *  Hardware setup:
 *   - PD0 - QDPH0
 *   - PD1 - QDPH90
 *   - PD2 - QDINDX
 *
 *  Peripherals used:
 *   - PORTD[2:0] signal input.
 *   - Event channel 0: quadrature signal.
 *   - Event channel 1: index signal.
 *   - TCC0: Quadrature counter.
 */
void main( void )
{
	/* Set up port C as output. */
	PORTC.DIRSET = 0xFF;

	/* Setup PORTD with pin 0 as input for QDPH0, dont invert IO pins.
	 *
	 * Setup event channel 0, pin 0 on PORTD as input, don't enable index.
	 * if index used then state 00 is set as the index reset state.
	 *
	 * Setup TCC0 with Event channel 0 and lineCount.
	 */
	QDEC_Total_Setup(&PORTD,                    /*PORT_t * qPort*/
	                 0,                         /*uint8_t qPin*/
	                 false,                     /*bool invIO*/
	                 0,                         /*uint8_t qEvMux*/
	                 EVSYS_CHMUX_PORTD_PIN0_gc, /*EVSYS_CHMUX_t qPinInput*/
	                 false,                     /*bool useIndex*/
	                 EVSYS_QDIRM_00_gc,         /*EVSYS_QDIRM_t qIndexState*/
	                 &TCC0,                     /*TC0_t * qTimer*/
	                 TC_EVSEL_CH0_gc,           /*TC_EVSEL_t qEventChannel*/
	                 lineCount);                /*uint8_t lineCount*/

	/* Setup TCD0 with Event channel 2 for same pin as QDPH0 and clk div 64. */
	QDEC_TC_Freq_Setup(&TCD0, TC_EVSEL_CH2_gc, EVSYS_CHMUX_PORTD_PIN0_gc, CLOCK_DIV_bm);

#ifdef GENERATE_TEST_SIGNAL
	/* Initialize and start outputting quadrature signal.*/
	generate_qdec_signal(&PORTE, lineCount, freq, dir);

	/* Enable low level interrupt.*/
	PMIC.CTRL |= PMIC_LOLVLEN_bm;

	/* Enable global interrupts.*/
	sei();
#endif
	
	/* Display the frequency of rotation on LEDs */
	while (1) {

		if ((TCD0.INTFLAGS & TC0_CCAIF_bm) !=  0) {
			/* Period of counter ticks are 1/(F_CPU/clk_div)
			 * Real tick count is 4 times captured value
			 * (when used with quadratur signal).
			 * Real frequency is then (F_CPU/clk_div)/(capture_value * linecount)
		 	 * For output in RPM multiply frequency by 60 (60 sec per min).*/
			calcFreq = (F_CPU / CLOCK_DIV) /
			           ((GetCaptureValue(TCD0) & 0xFFFC) * lineCount );
			calcRPM = calcFreq*60;
			PORTC.OUT = ~(calcFreq);
		}
	}
}
