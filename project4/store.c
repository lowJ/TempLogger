#include <avr/eeprom.h>
#include "store.h"
#include "avr.h"

//Configure the size of store here
//example: address 22 - address 10
#define TOP 22 
#define BOTTOM 10
#define WIDTH 4 //4 bytes wide

static int size = ((TOP - BOTTOM)/WIDTH) + 1;

void store_init(){
	eeprom_write_byte((uint8_t*)1, BOTTOM);
	avr_wait(25); 
}

unsigned int get_value(int n){
	uint16_t cur = eeprom_read_word((uint16_t*)1);
	avr_wait(25);
	int i = cur - (n*WIDTH);
	if(i >= BOTTOM){
		return eeprom_read_word((uint16_t*)i);
	}
	else{
		i = TOP - (BOTTOM - i - WIDTH);
		return eeprom_read_word((uint16_t*)i);
	}
}

int check_bounds(int n){
	int max = size - 1;
	if(n >= 0 && n <= max){
		return 1;
	}
	return 0;
}

void store_value(unsigned int val){
	uint16_t cur = eeprom_read_word((uint16_t*)1);
	avr_wait(25);
	if(cur + WIDTH > TOP){
		cur = BOTTOM;
	}
	else{
		cur += WIDTH;
	}
	eeprom_write_word((uint16_t*)1, cur);
	avr_wait(25);
	eeprom_write_word((uint16_t*)((uint16_t)cur), val);
	avr_wait(25);
}