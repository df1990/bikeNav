#ifndef _UART_H_
#define _UART_H_

#include <avr/io.h>

#define UART_HANDLER_MODE

#ifdef UART_BUFFER_MODE
#define UART_RX_BUFF_SIZE 128
#else//UART_HANDLER_MODE

#endif

void uart_init(uint32_t baud);
void uart_putc(char c);
void uart_puts(const char* str);
void uart_putsn(const char* str, uint8_t cnt);

#ifdef UART_BUFFER_MODE
uint8_t uart_getc(char *c);
#else//UART_HANDLER_MODE
void uart_rx_handler_register(void (*handler)(uint8_t));
#endif

#endif
