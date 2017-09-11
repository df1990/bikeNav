#ifndef _SPI_H_
#define _SPI_H_


#include "integer.h"

//#define SELECT()	PORTB &= ~_BV(2)	/* CS = L */
//#define	DESELECT()	PORTB |=  _BV(2)	/* CS = H */
//#define	SELECTING	!(PORTB &  _BV(2))	/* CS status (true:CS low) */

#define CS_H() PORTB |=  _BV(PB2)
#define CS_L() PORTB &= ~_BV(PB2)

void init_spi(void);

BYTE spi_transceive(BYTE d);

void xmit_mmc (
	const BYTE* buff,	
	UINT bc				
);

void rcvr_mmc (
	BYTE *buff,
	UINT bc	
);


#endif
