#include "uart.h"
#include <avr/interrupt.h>
#include <string.h>

#ifdef UART_BUFFER_MODE
static volatile uint8_t rx_buff[UART_RX_BUFF_SIZE];
static volatile uint8_t rx_buff_w_idx;
static volatile uint8_t rx_buff_r_idx;
#else//UART_HANDLER_MODE
void (*rx_handler)(uint8_t);
#endif

void uart_init(uint32_t baud)
{
    uint32_t ubrr;
    ubrr = F_CPU/16UL/baud - 1;
    
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)(ubrr);

    UCSR0B |= ((1<<RXEN0) | (1<<TXEN0));
    UCSR0C |= (3<<UCSZ00);

#ifdef UART_BUFFER_MODE
    rx_buff_w_idx = 0;
    rx_buff_r_idx = 0;
    //rx_buff_cnt = 0;
    
    memset((void *)rx_buff, 0, UART_RX_BUFF_SIZE);
#else//UART_HANDLER_MODE
    rx_handler = NULL;
#endif
    
    /* Init ISR */
    UCSR0B |= (1 << RXCIE0);
}

void uart_putc(char c)
{
    while ( !( UCSR0A & (1<<UDRE0)) )
    {          
        /* wait fot transmit buffer */
    }
    UDR0 = c;

}

void uart_puts(const char *str)
{
    while(*str)
    {
        uart_putc(*str++);
        //str++;
    }
}
void uart_putsn(const char* str, uint8_t cnt)
{
    uint8_t str_cnt = 0;
    while((*str) && (str_cnt++ < cnt))
    {
        uart_putc(*str++);
    }
}

#ifdef UART_BUFFER_MODE

uint8_t uart_getc(char *c)
{
    if(rx_buff_w_idx == rx_buff_r_idx)
        return 0;
    
    *c = rx_buff[rx_buff_r_idx++];
    rx_buff_r_idx = rx_buff_r_idx % UART_RX_BUFF_SIZE;
    return 1;
}

#else//UART_HANDLER_MODE

void uart_rx_handler_register(void (*handler)(uint8_t))
{
    rx_handler = handler;
}

#endif

ISR(USART_RX_vect)
{
#ifdef UART_BUFFER_MODE
    rx_buff[rx_buff_w_idx++] = UDR0;
    rx_buff_w_idx = rx_buff_w_idx % UART_RX_BUFF_SIZE;
    if(rx_buff_w_idx == rx_buff_r_idx)
        rx_buff_r_idx++;
#else//UART_HANDLER_MODE
    uint8_t tmp = UDR0;
    if(NULL != rx_handler)
        rx_handler(tmp);
#endif
    
}

