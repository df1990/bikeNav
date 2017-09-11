#ifndef _GPIO_H_
#define _GPIO_H_
#include <avr/io.h>

typedef enum {DORD_MSBFIRST, DORD_LSBFIRST} dataOrd;
typedef enum {OUTPUT, INPUT} pinMode;

void gpio_setmode(volatile uint8_t* DDR, uint8_t P, pinMode mode);
void gpio_write(volatile uint8_t* PORT, uint8_t P, uint8_t value);
uint8_t gpio_read(volatile uint8_t* PIN, uint8_t P);
void gpio_shiftout(volatile uint8_t* PORT_data, uint8_t P_data, volatile uint8_t* PORT_clk, uint8_t P_clk,  uint8_t data, dataOrd order);
#endif
