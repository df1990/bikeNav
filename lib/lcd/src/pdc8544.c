#include "lcd.h"
#include "font_default.h"
#include "gpio.h"
#include "uart.h"

#include <avr/io.h>

#define SCE_DDR DDRD
#define SCE_PORT PORTD
#define SCE_PIN 7

#define RESET_DDR DDRD
#define RESET_PORT PORTD
#define RESET_PIN 6

#define DC_DDR DDRD
#define DC_PORT PORTD
#define DC_PIN 5

#define SDIN_DDR DDRD
#define SDIN_PORT PORTD
#define SDIN_PIN 4

#define SCLK_DDR DDRD
#define SCLK_PORT PORTD
#define SCLK_PIN 3

#define LCD_C     0
#define LCD_D     1

#define LCD_X     84
#define LCD_Y     48


static void lcd_write(uint8_t dc, uint8_t data)
{
  gpio_write(&DC_PORT, DC_PIN, dc);
  gpio_write(&SCE_PORT, SCE_PIN, 0);
  gpio_shiftout(&SDIN_PORT, SDIN_PIN, &SCLK_PORT, SCLK_PIN, data, DORD_MSBFIRST);
  gpio_write(&SCE_PORT, SCE_PIN, 1);
}

void lcd_init(void)
{
    
    gpio_setmode(&SCE_DDR, SCE_PIN, OUTPUT);
    gpio_setmode(&RESET_DDR, RESET_PIN, OUTPUT);
    gpio_setmode(&DC_DDR, DC_PIN, OUTPUT);
    gpio_setmode(&SDIN_DDR, SDIN_PIN, OUTPUT);
    gpio_setmode(&SCLK_DDR, SCLK_PIN, OUTPUT);

    gpio_write(&RESET_PORT, RESET_PIN, 0);
    gpio_write(&RESET_PORT, RESET_PIN, 1);

    
    lcd_write(LCD_C, 0x21 );  // LCD Extended Commands.
    lcd_write(LCD_C, 0xB1 );  // Set LCD Vop (Contrast). 
    lcd_write(LCD_C, 0x04 );  // Set Temp coefficent. //0x04
    lcd_write(LCD_C, 0x14 );  // LCD bias mode 1:48. //0x13
    lcd_write(LCD_C, 0x20 );  // LCD Basic Commands
    lcd_write(LCD_C, 0x0C );  // LCD in normal mode.
    
    uart_puts("LCD init [OK]\n");
}

void lcd_clear(void)
{
    uint16_t index;
    for (index = 0; index < LCD_X * LCD_Y / 8; index++)
    {
        lcd_write(LCD_D, 0x00);
    }
}

int lcd_putchar(char c)
{
    lcd_write(LCD_D, 0x00);
    uint16_t index;
    for (index = 0; index < 5; index++)
    {
        lcd_write(LCD_D, pgm_read_byte(&font_default[c - 0x20][index]));
    } 
    lcd_write(LCD_D, 0x00);
    return 0;
}

int lcd_puts(const char *s)
{
    //char *c = s;
    while(*s)
    {
        lcd_putchar(*s++);   
    }
    return 0;
}

int lcd_putsn(const char *s, uint8_t cnt)
{
    uint8_t str_cnt = 0;
    while((*s) && (str_cnt++ < cnt))
    {
        lcd_putchar(*s++);
    }
    return str_cnt;
}

void lcd_setpos(int x, int y)
{
  lcd_write( 0, 0x80 | x);  // Column.
  lcd_write( 0, 0x40 | y);  // Row.  

}



