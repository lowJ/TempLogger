#ifndef _KEYPAD_H_
#define _KEYPAD_H_
#include "avr.h"

int waitKeypress(); //wait until keypressed, return num of key pressed.

int pressed(int k); //checks if key is pressed

void waitKeyRelease(int k); //waits still specific key is released

int waitPAR(); //waits until a key is pressed and released. Returns num of key pressed

char mapButton(int k); //maps key's num to char value shown on keypad;

int mapIntVal(int k); //maps key's num to int value shown on keypad;




#endif