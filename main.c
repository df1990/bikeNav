/*----------------------------------------------------------------------*/
/* Foolproof FatFs sample project for AVR              (C)ChaN, 2014    */
/*----------------------------------------------------------------------*/

#include <avr/io.h>	/* Device specific declarations */
#include <util/delay.h>
#include <avr/interrupt.h>
#include "ff.h"		/* Declarations of FatFs API */
#include "uart.h"
#include "version.h"
#include "lcd.h"

FATFS FatFs;		/* FatFs work area needed for each volume */
FIL Fil;			/* File object needed for each open file */
DIR Dir;
FILINFO fno;

void ERROR(const char *str, uint32_t ecode)
{
    uart_puts(str);
    uart_puts("ECODE:");
    uart_putc('0' + ecode);
    uart_putc('\n');
    while(1)
    {
        _delay_ms(1000);
        uart_putc('.');
    }
}

int main (void)
{
    lcd_init();
    lcd_clear();

    
    lcd_puts("Hello world!");
    lcd_setpos(0, 1);
    lcd_puts(fw_version);
    
    FRESULT rc;
	
    uart_init(9600UL);
	
    uart_puts("=== GSP Logger ===\n");
    uart_puts("build: ");
    uart_puts(fw_version);
    uart_puts("\n");
    
    _delay_ms(5000);

    sei();
    
	if((rc = f_mount(&FatFs, "", 0)) != FR_OK)		/* Give a work area to the default drive */
	{
		ERROR("f_mount failed ", rc);	
	}
	uart_puts("f_mount [OK]\n");
    /*
    if((rc = f_stat("sub1",&fno)) == FR_OK)
    {
        uart_puts("sub1 already exists\n");
    }
	else if((rc = f_mkdir("sub1")) != FR_OK)
	{
		ERROR("f_mkdir failed ", rc);	
    }
	
		
	if((rc = f_open(&Fil, "sub1/newfile.txt", FA_WRITE | FA_CREATE_ALWAYS)) != FR_OK)
	{
		ERROR("f_open failed ", rc);	
	}
	
	f_printf(&Fil, "GPS Logger, fw_version=%s\n", fw_version); 
	f_close(&Fil);
	uart_puts("File write complete\n");
	*/
	while(1)
    {
        char c;
        if(uart_getc(&c))
        {
            uart_putc(c);
        }
    }
}


