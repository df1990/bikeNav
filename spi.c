#include "spi.h"
#include <avr/io.h>

void init_spi(void)
{
    DDRB |= ((1 << PB2) | (1 << PB3) | (1 << PB5));
    SPCR |= ((1 << SPE) | (1 << MSTR) | (1 << SPR1));
}

BYTE spi_transceive(BYTE d)
{
    SPDR = d;
    while(!(SPSR & (1<<SPIF)));
    return SPDR;
}

void xmit_mmc (
	const BYTE* buff,	
	UINT bc				
)
{
    while(bc--)
    {
        spi_transceive(*buff++);
    }
}

void rcvr_mmc (
	BYTE *buff,
	UINT bc	
)
{
    while(bc--)
    {
        *buff++ = spi_transceive(0xFF);
    }
}


