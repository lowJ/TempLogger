#include "volt.h"

static int read_adc(){
	ADMUX = 0x40; //01000000 //configure adc for pin ADC0
	SET_BIT(ADCSRA, 7); //turn on adc
	SET_BIT(ADCSRA, 6);//stat adc
	while(GET_BIT(ADCSRA, 6) == 1){
		//wait till adc is readty to be read.
	}
	return ADC;
}

float getVoltage(){
	float x = read_adc();
	x = (float)x * 5 / 1024;
	return x;
}

void displayM1(struct vdata *s){
	char temp[5];
	float val;
	lcd_pos(0,0);
	lcd_puts("I:");
	lcd_pos(0,2);
	sprintf(temp, "%.2f", s->instant);
	lcd_puts(temp);
	lcd_pos(0,7);
	lcd_puts("AVG:");
	val = s->cur_sum/s->num_samples;
	sprintf(temp, "%.2f", val);
	lcd_pos(0,11);
	lcd_puts(temp);
	lcd_pos(1,0);
	lcd_puts("MI:");
	sprintf(temp, "%.2f", s->min);
	lcd_pos(1,4);
	lcd_puts(temp);
	lcd_pos(1,9);
	lcd_puts("MX:");
	sprintf(temp, "%.2f", s->max);
	lcd_pos(1,12);
	lcd_puts(temp);
	
	
	
	
}

void displayM0(struct vdata *s){
	char temp[5];
	lcd_pos(0,0);
	lcd_puts("I:");
	lcd_pos(0,2);
	sprintf(temp, "%.2f", s->instant);
	lcd_puts(temp);
	lcd_pos(0,7);
	lcd_puts("AVG:");
	lcd_pos(0,11);
	lcd_puts("----");
	lcd_pos(1,0);
	lcd_puts("MI:");
	lcd_pos(1,4);
	lcd_puts("----");
	lcd_pos(1,9);
	lcd_puts("MX:");
	lcd_pos(1,12);
	lcd_puts("----");
	
	
	
	
}

void update(struct vdata *s, float v){
	s->cur_sum += v;
	s->num_samples += 1;
	s->instant = v;
	if(v < s->min){
		s->min = v;
	}
	if(v > s->max){
		s->max = v;
	}
}

