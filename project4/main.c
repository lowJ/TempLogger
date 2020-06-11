/*
 * project4.c
 *
 * Created: 6/5/2020 12:14:45 AM
 * Author : jonathan
 */ 

#include <avr/io.h>
#include "lcd.h"
#include "avr.h"
#include "volt.h"

#define CHIP_ID_REG 0xD0 //Register that stores chip ID
#define TEMP_REG 0xFA //Register that stores the MSB for temperature.
#define MEAS_CTRL_REG 0xF4 //Register that stores config settings
#define dig_T1_REG 0x88 //unsigned short
#define dig_T2_REG 0x8A//signed short
#define dig_T3_REG 0x8C
#define BMP_ADDR_W 0xEE
#define BMP_ADDR_R 0xEF

void twi_init(){
	TWBR = 0x00;
	TWBR= 0x0C;//double check this
}
//Send start condition
void twi_start(){
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	//while((TWSR & (0xF8)) != 0x08);
}
void twi_write_addr_w(unsigned char addr){
	TWDR= addr;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	//while((TWSR & (0xF8)) != 0x18); //check status
}

void twi_write_byte(unsigned char data){
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	//while((TWSR & (0xF8)) != 0x28); //check status
}

void twi_write_addr_r(unsigned char addr){
	TWDR= addr;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	//while((TWSR & (0xF8)) != 0x40); //check status
}

unsigned char twi_read_byte(){
	unsigned char data;
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while(!(TWCR & (1<<TWINT)));
	data = TWDR;
	//while((TWSR & (0xF8)) != 0x50); //check status
	return data;
	
}

unsigned char twi_read_last_byte(){
	unsigned char data;
	
	TWCR = (1<<TWINT)|(1<<TWEN);//TWEA = 0  to send NACK
	while(!(TWCR & (1<<TWINT)));
	data = TWDR;
	//while((TWSR & (0xF8)) != 0x58); //check status
	return data;
}

void twi_stop(){
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

unsigned short twi_read16(unsigned char reg){
	unsigned short data;
	unsigned short temp;
	//Format [b2] [b1]
	twi_start();
	twi_write_addr_w(BMP_ADDR_W);
	twi_write_byte(reg);
	twi_stop();
	twi_start();
	twi_write_addr_r(BMP_ADDR_R);
	data = twi_read_byte();
	temp = twi_read_last_byte();
	twi_stop();
	temp <<= 8;
	data |= temp;
	return data;
}





int main(void)
{
	unsigned char temp;
    //1110111 0x77base addr
	// 0xEE for write addr
	//0xEF for read addr
	
		//sprintf(result, "%.2f", x); //fix this formatting
	//This gets the chip ID
	lcd_init();
	twi_init();
	twi_start();
	twi_write_addr_w(BMP_ADDR_W);
	twi_write_byte(CHIP_ID_REG);//Addr for CHIP ID;
	twi_stop();
	twi_start();
	twi_write_addr_r(BMP_ADDR_R);
	temp = twi_read_last_byte(); //fix this
	twi_stop();
	lcd_pos(0,0);
	lcd_put(temp);
	//Obtain coefficents
	unsigned short dig_T1;
	signed short dig_T2;
	signed short dig_T3;
	dig_T1 = twi_read16(dig_T1_REG);
	dig_T2 = (signed short)twi_read16(dig_T2_REG);
	dig_T3 = (signed short)twi_read16(dig_T3_REG);
	
	
	
	
	
	
	
	
    while (1) 
    {
		lcd_put_sshort(dig_T2, 0, 0);
		lcd_put_sshort(dig_T3, 1, 0);
		avr_wait(1000);
    }
}

