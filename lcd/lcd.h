#ifndef _LCD_H_
#define _LCD_H_

void lcd_init(void);
void lcd_clear(void);
int lcd_puts(const char *s);
int lcd_putchar(char c);

#endif
