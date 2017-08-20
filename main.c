/*----------------------------------------------------------------------*/
/* Foolproof FatFs sample project for AVR              (C)ChaN, 2014    */
/*----------------------------------------------------------------------*/

#include <avr/io.h>	/* Device specific declarations */
#include <util/delay.h>
#include "ff.h"		/* Declarations of FatFs API */
#include "uart.h"
//#include "delay.h"

FATFS FatFs;		/* FatFs work area needed for each volume */
FIL Fil;			/* File object needed for each open file */
DIR Dir;

void ERROR(const char *str, uint32_t ecode)
{
    uart_sends(str);
    uart_sends("ECODE:");
    uart_sendc('0' + ecode);
    uart_sendc('\n');
    while(1)
    {
        _delay_ms(1000);
        uart_sendc('.');
    }
}

int main (void)
{
	UINT bw;
	FRESULT rc;
	
	
	uart_init(19200UL);
	uart_sends("GSP Logger test\n");

	if((rc = f_mount(&FatFs, "", 0)) != FR_OK)		/* Give a work area to the default drive */
	{
		ERROR("f_mount failed ", rc);	
	}
	uart_sends("f_mount [OK]\n");
	
	if((rc = f_mkdir("sub2")) != FR_OK)
	{
		ERROR("f_mkdir failed ", rc);	
	}
	uart_sends("f_mkdir [OK]\n");
	
		
	if((rc = f_open(&Fil, "sub2/newfile.txt", FA_WRITE | FA_CREATE_ALWAYS)) != FR_OK)
	{
		ERROR("f_open failed ", rc);	
	}
	
	f_write(&Fil, "It works!\r\n", 11, &bw);	/* Write data to the file */
	f_printf(&Fil, "fprintf test %d", 1234); 
	f_close(&Fil); /* Close the file */
	
	if(11 == bw)
	{
		uart_sends("File write success!\n");	
	}
	
	while(1)
    {
        uart_sends("Program end...\n");
        _delay_ms(1000);
    }
}


