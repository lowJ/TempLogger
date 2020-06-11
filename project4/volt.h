#ifndef VOLT_H
#define VOLT_H

#include "avr.h"
#include "lcd.h"
#include <stdio.h>

struct vdata{
	float cur_sum;
	float num_samples;
	float min;
	float max;
	float instant;
};

float getVoltage();
void displayM1(struct vdata *s);
void displayM0(struct vdata *s);
void update(struct vdata *s, float v);
#endif