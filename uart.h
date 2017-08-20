#ifndef _UART_H_
#define _UART_H_
#include <avr/io.h>

void uart_init(uint32_t baud);
void uart_sendc(char c);
void uart_sends(const char* str);
#endif
