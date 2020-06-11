#include <avr/eeprom.h>
#include "store.h"
//Configure the size of store here
#define TOP 20
#define BOTTOM 10

static int size = TOP - BOTTOM + 1;
static int cur = BOTTOM; //stores the last written value

unsigned int get_value(int n){
	int i = cur - n;
	if(i >= BOTTOM){
		return eeprom_read_word((unsigned int *)i);
	}
	else{
		i = TOP - (BOTTOM - i - 1);
		return eeprom_read_word((unsigned int *)i);
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
	if(cur + 1 > TOP){
		cur = BOTTOM;
	}
	else{
		cur += 1;
	}
	eeprom_update_word((unsigned int *)cur, val);
}