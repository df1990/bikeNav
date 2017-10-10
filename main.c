/*----------------------------------------------------------------------*/
/* Foolproof FatFs sample project for AVR              (C)ChaN, 2014    */
/*----------------------------------------------------------------------*/

#include <avr/io.h>	/* Device specific declarations */
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
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

static const char string_1[] PROGMEM = "String 1";

static PGM_P const string_table[] PROGMEM = 
{
    string_1
};


static uint8_t gpgga_filter = 0;
static uint8_t gpgsv_filter = 0;
static uint8_t received_sentence;

int main (void)
{
    char fw_version[10];
    char hour_raw[11] = "";
    char hour_conv[9] = "";
    char latitude[10] = "";
    char ns_ind[2] = "";
    char longtitude[11] = "";
    char we_ind[2] = "";
    char gps_fix[2] = "";
    char satelites[3] = "";

    strcpy_P(fw_version,(PGM_P)fw_version_pgm);
    
    uart_init(9600UL);
    lcd_init();
    lcd_clear();
    gps_init();
    
    if(Gps_OK == gps_filter_add("GPGGA",&gpgga_filter))
    {
        uart_puts("GPGGA filter add [OK]\n");
        if(Gps_OK == gps_field_add(gpgga_filter,0,hour_raw))
        {
            uart_puts("GPGGA field 0 add [OK]\n");
        }
        if(Gps_OK == gps_field_add(gpgga_filter,5,gps_fix))
        {
            uart_puts("GPGGA field 5 add [OK]\n");
        }
        
        if(Gps_OK == gps_field_add(gpgga_filter,1,latitude))
        {
            uart_puts("GPGGA field 1 add [OK]\n");
        }
        if(Gps_OK == gps_field_add(gpgga_filter,2,ns_ind))
        {
            uart_puts("GPGGA field 2 add [OK]\n");
        }
        
        if(Gps_OK == gps_field_add(gpgga_filter,3,longtitude))
        {
            uart_puts("GPGGA field 3 add [OK]\n");
        }
        if(Gps_OK == gps_field_add(gpgga_filter,4,we_ind))
        {
            uart_puts("GPGGA field 4 add [OK]\n");
        }
    }
    if(Gps_OK == gps_filter_add("GPGSV",&gpgsv_filter))
    {
        uart_puts("GPGSV filter add [OK]\n");
        if(Gps_OK == gps_field_add(gpgsv_filter,2,satelites))
        {
            uart_puts("GPGSV field 2 add [OK]\n");
        }
    }
    //gps_nmea_trace_add("GPGGA",1,hour_raw);
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
    
    lcd_puts("GPS Logger");
    lcd_setpos(0, 1);
    lcd_puts("build:");
    lcd_setpos(0, 2);
    lcd_puts(fw_version);
	
    uart_puts("=== GPS Logger ===\n");
    uart_puts("build: ");
    uart_puts(fw_version);
    uart_puts("\n");
    
    _delay_ms(5000);
    lcd_clear();
    
    //gps_handler_register();
    
    
	while(1)
    {
        if(gps_nmea_rx_complete())
        {
            gps_nmea_rx_clear();
            if(Gps_OK == gps_nmea_rx_check(&received_sentence))
            {
                if(gpgga_filter == received_sentence)
                {
                    hour_convert(hour_conv,hour_raw);
                    
                    lcd_setpos(0, 0);
                    lcd_puts(hour_conv);
                    
                    lcd_setpos(0, 3);
                    lcd_puts(latitude);
                    lcd_puts(" ");
                    lcd_puts(ns_ind);
                    
                    lcd_setpos(0, 4);
                    lcd_puts(longtitude);
                    lcd_puts(" ");
                    lcd_puts(we_ind);
                }
                else if(gpgsv_filter == received_sentence)
                {
                    lcd_setpos(0, 1);
                    lcd_puts(satelites);
                }

            }
        }
        /*if(hour_raw[0])
        {
            hour_convert(hour_formatted,hour_raw);
            uart_puts(hour_formatted);
            uart_puts("\n");
            lcd_setpos(0,3);
            lcd_puts(hour_formatted);
            hour_raw[0] = 0x00;
        }*/
    }
}


