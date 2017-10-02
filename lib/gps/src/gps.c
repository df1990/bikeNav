#include "gps.h"
#include "uart.h"

void gsp_init(void)
{
    uart_puts("GPS init complete\n");    
}

void gps_getvalue(const char *sentence, uint8_t field)
{
       
}

