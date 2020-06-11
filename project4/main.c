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
#include "store.h"



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
	float temp;
    while (1) 
    {
		
		//Correct vlaue , wait 4 cyccles, another correct , etc....
		
		
		temp = convert_C_to_F(bmp280_calc_temp());
		temp = temp * 100;
		store_value((int)temp);
		avr_wait(25);
		
		//This entry is always correct.
		temp = get_value(0);
		temp /= 100.0;
		lcd_put_float(temp, 0, 0);
		avr_wait(25);
		
		//I think get value is off
		temp = get_value(1);
		temp /= 100.0;
		lcd_put_float(temp, 0, 8);
		avr_wait(25);
		
		temp = get_value(2);
		temp /= 100.0;
		lcd_put_float(temp, 1, 0);
		avr_wait(25);
		
		//fucks with the value here.
		temp = get_value(3);
		temp /= 100.0;
		lcd_put_float(temp, 1, 8);
		avr_wait(4000);
    }
}

