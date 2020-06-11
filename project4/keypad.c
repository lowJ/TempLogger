#include "lcd.h"
#include "avr.h"

int mapIntVal(int k){
	static int intmap[17] = {0,
		1, 4, 7, 0,
		2, 5, 8, 0,
		3, 6, 9, 0,
		0, 0, 0, 0};
	return intmap[k];
}

char mapButton(int k){
	//Add bounds checking
	static char charmap[17] = {'0',
		'1', '4', '7', '*',
		'2', '5', '8', '0',
		'3', '6', '9', '#',
	'A', 'B', 'C', 'D'};
	return charmap[k];
}
int waitKeypress(){
	while(1){
		int count;
		count = 1;
		for (int i = 4; i < 8; i++) //go through each col
		{
			SET_BIT(PORTA, i); //enable pullup
			avr_wait(4);
			for(int j = 0; j < 4; j++){ //scan through rows
				SET_BIT(DDRA, j); //set to ouptut
				CLR_BIT(PORTA, j); //set to low
				if(!GET_BIT(PINA, i)){ //Checks if key is pressed
					return count;
				}
				count += 1;
				CLR_BIT(DDRA, j); //set back as input(disconnects)
			}
			CLR_BIT(PORTA,i); //disable pullup
		}
	}
}

int pressed(int k){
	int count;
	count = 1;
	for (int i = 4; i < 8; i++) //go through each col
	{
		SET_BIT(PORTA, i); //enable pullup
		for(int j = 0; j < 4; j++){ //scan through the rows
			SET_BIT(DDRA, j); //set to ouptut
			CLR_BIT(PORTA, j); //set to low
			if(count == k){ //Checks specific key
				if(!GET_BIT(PINA, i)){ //checks if button pressed
					return 1;
				}
				return 0;
			}
			count += 1;
			CLR_BIT(DDRA, j); //set back as input(disconnects)
		}
		CLR_BIT(PORTA,i); //disable pullup
	}
		return 0;
}

void waitKeyRelease(int k){
	while(pressed(k)){;}
}


int waitPAR(){
	int key = waitKeypress();
	waitKeyRelease(key);
	return key;
}