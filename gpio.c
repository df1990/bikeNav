#include "gpio.h"
#include <avr/io.h>

void gpio_setmode(volatile uint8_t* DDR, uint8_t P, pinMode mode)
{
    if(OUTPUT == mode)
        *DDR |= (1<<P);
    else
        *DDR &= ~(1<<P);
}

void gpio_write(volatile uint8_t* PORT, uint8_t P, uint8_t value)
{
    if(value)
        *PORT |= (1<<P);
    else
        *PORT &= ~(1<<P);
}

uint8_t gpio_read(volatile uint8_t* PIN, uint8_t P)
{
    return (*PIN & (1<<P));
} 

void gpio_shiftout(volatile uint8_t* PORT_data, uint8_t P_data, volatile uint8_t* PORT_clk, uint8_t P_clk,  uint8_t data, dataOrd order)
{
    uint8_t mask = (DORD_MSBFIRST == order) ? 0x80 : 0x01;

    while(mask)
    {
        gpio_write(PORT_clk, P_clk, 0);
        gpio_write(PORT_data, P_data, (data & mask));
        gpio_write(PORT_clk, P_clk, 1);
        mask = (DORD_MSBFIRST == order) ? mask>>1 : mask<<1;
    }
}
