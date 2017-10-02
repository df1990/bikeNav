#ifndef _UART_H_
#define _UART_H_
#include <avr/io.h>

#define UART_RX_BUFF_SIZE 128

void uart_init(uint32_t baud);
void uart_putc(char c);
void uart_puts(const char* str);
uint8_t uart_getc(char *c);
#endif
