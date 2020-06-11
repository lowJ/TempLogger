/**
 * lcd.h
 * Copyright (C) 2001-2020, Tony Givargis
 */

#ifndef _LCD_H_
#define _LCD_H_

#include <stdio.h>

void lcd_init(void);

void lcd_clr(void);

void lcd_pos(unsigned char r, unsigned char c);

void lcd_put(char c);

void lcd_puts(const char *s);

void lcd_put_ushort(unsigned short s, unsigned int row, unsigned int col);

void lcd_put_sshort(unsigned short s, unsigned int row, unsigned int col);

void lcd_put_float(float f, unsigned int row, unsigned int col);
//void lcd_put_sshort(signed short s);
#endif /* _LCD_H_ */