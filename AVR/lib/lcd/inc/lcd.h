#ifndef _LCD_H_
#define _LCD_H_
#include <stdint.h>

void lcd_init(void);
void lcd_clear(void);
int lcd_puts(const char *s);
int lcd_putsn(const char *s, uint8_t cnt);
int lcd_putchar(char c);
void lcd_setpos(int x, int y);

#endif
