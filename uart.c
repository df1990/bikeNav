#include "uart.h"

void uart_init(uint32_t baud)
{
    uint32_t ubrr;
    ubrr = F_CPU/16UL/baud - 1;
    
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)(ubrr);

    UCSR0B = ((1<<RXEN0) | (1<<TXEN0));
    UCSR0C = (3<<UCSZ00);

}

void uart_sendc(char c)
{
    while ( !( UCSR0A & (1<<UDRE0)) )
    {          
        /* wait fot transmit buffer */
    }
    UDR0 = c;

}

void uart_sends(const char *str)
{
    while(*str)
    {
        uart_sendc(*str);
        str++;
    }
}

void xmit(char c)
{
    while ( !( UCSR0A & (1<<UDRE0)) )
    {          
        /* wait fot transmit buffer */
    }
    UDR0 = c;
}
