#include <io.h>
#include <stdint.h>
#include <stdbool.h>

#define USART_Format_Set(_usart, _charSize, _parityMode, _twoStopBits)\
    (_usart)->CTRLC = (uint8_t) _charSize | _parityMode |\
                      (_twoStopBits ? USART_SBMODE_bm : 0)

#define USART_Baudrate_Set(_usart, _bselValue, _bScaleFactor)\
        (_usart)->BAUDCTRLA =(uint8_t)_bselValue;\
        (_usart)->BAUDCTRLB =(_bScaleFactor << USART_BSCALE0_bp)|(_bselValue >> 8)

#define USART_Rx_Enable(_usart) ((_usart)->CTRLB |= USART_RXEN_bm)
#define USART_Tx_Enable(_usart) ((_usart)->CTRLB |= USART_TXEN_bm)

// USARTC0 TX initialization function
void usartc0_tx_init(void)
{
// Init the USART
// This PORT setting is only valid for USARTC0
// if other USART is used a different PORT and/or pins are used,
// please see the datasheet
// PIN3 (TXD0) as output
PORTC.DIRSET = PIN3_bm;
// PC2 (RXD0) as input
PORTC.DIRCLR = PIN2_bm;

// USARTC0, 8 Data bits, No Parity, 1 Stop bit
USART_Format_Set(&_ATXMEGA_USART_, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, false);

// Set Baudrate to 9600 bps
// Use the default I/O clock fequency that is 2 MHz
// Do not use the baudrate scale factor
// Baudrate select = (1/(16*(((I/O clock frequency)/Baudrate)-1) = 12
USART_Baudrate_Set(&_ATXMEGA_USART_, 12, 0);

// Enable TX
USART_Tx_Enable(&_ATXMEGA_USART_);
}
