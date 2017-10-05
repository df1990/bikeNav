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
#include "gps.h"

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

static void hour_convert(char *dst, char *src)
{
    uint8_t d_idx = 0;
    uint8_t s_idx = 0;
    uint8_t loop_cnt = 0;
    for(loop_cnt = 0; loop_cnt < 3; loop_cnt++)
    {
        dst[d_idx++] = src[s_idx++];
        dst[d_idx++] = src[s_idx++];
        dst[d_idx++] = ':';
    }
    dst[--d_idx] = 0x00;
}

int main (void)
{
    TNmeaSentence sentence;
    char hour_raw[11];
    char hour_formatted[9];
    
    uart_init(9600UL);
    lcd_init();
    lcd_clear();
    gps_init();
    gps_sentence_filter_add(nmea_gga,0,hour_raw);
    sei();
    
    FRESULT rc;
    
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
    
    lcd_puts("GSP Logger");
    lcd_setpos(0, 1);
    lcd_puts("build:");
    lcd_setpos(0, 2);
    lcd_puts(fw_version);
	
    uart_puts("=== GSP Logger ===\n");
    uart_puts("build: ");
    uart_puts(fw_version);
    uart_puts("\n");
    
    _delay_ms(5000);
    
    gps_handler_register();
    
    
	while(1)
    {
        if(nmea_no_sentence != (sentence = gps_nmea_sentence_received()))
        {
            gps_nmea_sentence_clear();
            if(nmea_gga == sentence)
            {
                hour_convert(hour_formatted,hour_raw);
                uart_puts(hour_formatted);
                uart_puts("\n");
                lcd_setpos(0,3);
                lcd_puts(hour_formatted);
                
            }
        }
    }
}


