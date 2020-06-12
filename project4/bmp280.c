#include "bmp280.h"
#include "twi.h"
//#include <avr/io.h>

#define ACTUAL_CHIP_ID 0x58
#define CHIP_ID_REG 0xD0 //Register that stores chip ID
#define TEMP_REG 0xFA //Register that stores the MSB for temperature.
#define CONFIG_REG 0xF4 //Register that stores config settings
#define CONFIG_SETTINGS 0x93 //0x93 represents the config:
//0x93 = 10010011
//100 = 8x pres sample, 100 = 8x temp sample, 11 = normal power mode

#define dig_T1_REG 0x88
#define dig_T2_REG 0x8A
#define dig_T3_REG 0x8C
#define BMP_ADDR_W 0xEE
#define BMP_ADDR_R 0xEF

static unsigned short dig_T1;
static signed short dig_T2;
static signed short dig_T3;

static unsigned char get_chip_id(){
	unsigned char temp;
	twi_start();
	twi_write_addr_w(BMP_ADDR_W);
	twi_write_byte(CHIP_ID_REG);
	twi_start();
	twi_write_addr_r(BMP_ADDR_R);
	temp = twi_read_last_byte();
	twi_stop();
	return temp;
}

int bmp280_init(){
	if(!bmp280_verify()){
		return 0;
	}
	bmp280_get_coefficents();
	bmp280_set_config();
	return 1;
}

int bmp280_verify(){
	if(get_chip_id() == ACTUAL_CHIP_ID){
		return 1;
	}
	return 0;
}

void bmp280_get_coefficents(){
	dig_T1 = twi_read16(dig_T1_REG, BMP_ADDR_W, BMP_ADDR_R);
	dig_T2 = (signed short)twi_read16(dig_T2_REG, BMP_ADDR_W, BMP_ADDR_R);
	dig_T3 = (signed short)twi_read16(dig_T3_REG, BMP_ADDR_W, BMP_ADDR_R);
}

void bmp280_set_config(){
	twi_start();
	twi_write_addr_w(BMP_ADDR_W);
	twi_write_byte(CONFIG_REG);
	twi_write_byte(CONFIG_SETTINGS);
	twi_stop();
}

float bmp280_calc_temp(){
	int32_t adc_T, var1, var2, temp;
	adc_T = twi_read24REV(TEMP_REG, BMP_ADDR_W, BMP_ADDR_R); //[BYTE][BYTE]XXXX[][][][]
	adc_T >>= 4; 
	//Calculation arithrmetic found in datasheet.
	var1 = ((((adc_T>>3) - ((int32_t)dig_T1<<1))) * ((int32_t)dig_T2)) >> 11;
	var2 = (((((adc_T>>4) - ((int32_t)dig_T1)) * ((adc_T>>4) - ((int32_t)dig_T1))) >> 12) *
	((int32_t)dig_T3)) >> 14;
	temp = ((var1 + var2) * 5 + 128) >> 8;
	return temp/100.0;
}

float convert_C_to_F(float c){
	return (c * 1.8) + 32.0;
}
