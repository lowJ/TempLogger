/**
 * lcd.c
 * Copyright (C) 2001-2020, Tony Givargis
 */

#include "avr.h"
#include "lcd.h"

#define DDR    DDRB
#define PORT   PORTB
#define RS_PIN 0
#define RW_PIN 1
#define EN_PIN 2

static void
set_data(unsigned char x)
{
	PORTA = x;
	DDRA = 0xff;
}

static unsigned char
get_data(void)
{
	DDRA = 0x00;
	return PINA;
}

static void
sleep_700ns(void)
{
	NOP();
	NOP();
	NOP();
}

static unsigned char
input(unsigned char rs)
{
	unsigned char d;

	if (rs) {
		SET_BIT(PORT, RS_PIN);
	}
	else {
		CLR_BIT(PORT, RS_PIN);
	}
	SET_BIT(PORT, RW_PIN);
	get_data();
	SET_BIT(PORT, EN_PIN);
	sleep_700ns();
	d = get_data();
	CLR_BIT(PORT, EN_PIN);
	return d;
}

static void
output(unsigned char d, unsigned char rs)
{
	if (rs) {
		SET_BIT(PORT, RS_PIN);
	}
	else {
		CLR_BIT(PORT, RS_PIN);
	}
	CLR_BIT(PORT, RW_PIN);
	set_data(d);
	SET_BIT(PORT, EN_PIN);
	sleep_700ns();
	CLR_BIT(PORT, EN_PIN);
}

static void
write(unsigned char c, unsigned char rs)
{
	while (input(0) & 0x80);
	output(c, rs);
}

void
lcd_init(void)
{
	SET_BIT(DDR, RS_PIN);
	SET_BIT(DDR, RW_PIN);
	SET_BIT(DDR, EN_PIN);
	avr_wait(16);
	output(0x30, 0);
	avr_wait(5);
	output(0x30, 0);
	avr_wait(1);
	write(0x3c, 0);
	write(0x0c, 0);
	write(0x06, 0);
	write(0x01, 0);
}

void
lcd_clr(void)
{
	write(0x01, 0);
}

void
lcd_pos(unsigned char r, unsigned char c)
{
	unsigned char n = r * 40 + c;

	write(0x02, 0);
	while (n--) {
		write(0x14, 0);
	}
}

void
lcd_put(char c)
{
	write(c, 1);
}

void
lcd_puts(const char *s)
{
	char c;
	while ((c = *(s++)) != 0) {
		write(c, 1);
	}
}


void 
lcd_put_ushort(unsigned short s, unsigned int row, unsigned int col){
	char temp[2];
	sprintf(temp, "%hu", s);
	lcd_pos(row, col);
	lcd_puts(temp);
	
	
			//sprintf(result, "%.2f", x); //fix this formatting

}

void
lcd_put_sshort(unsigned short s, unsigned int row, unsigned int col){
	char temp[2];
	sprintf(temp, "%hi", s);
	lcd_pos(row, col);
	lcd_puts(temp);
	
	
	//sprintf(result, "%.2f", x); //fix this formatting

}
