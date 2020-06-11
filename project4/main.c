/*
 * project4.c
 *
 * Created: 6/5/2020 12:14:45 AM
 * Author : jonathan
 */ 

#include <avr/io.h>
#include "lcd.h"
#include "avr.h"
#include "twi.h"
#include "bmp280.h"



int main(void)
{
	lcd_init();
	twi_init();
	
	//init bmp280
	if(!bmp280_init()){
		lcd_pos(0,0);
		lcd_puts("ERROR!");
		lcd_pos(1,0);
		lcd_puts("BMP280 wrong ID");
		while(1);
	}
	//
	
	
	
	
	//
	
    while (1) 
    {
		lcd_put_float(convert_C_to_F(bmp280_calc_temp()), 0, 0);
		avr_wait(1000);
    }
}

