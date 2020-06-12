#include "twi.h"
#include <avr/io.h>

void twi_init(){
	TWBR = 0x00;
	TWBR= 0x0C;//set rate of SCL
}

void twi_start(){
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	//while((TWSR & (0xF8)) != 0x08);
}
void twi_write_addr_w(unsigned char addr){
	TWDR= addr;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	//while((TWSR & (0xF8)) != 0x18);
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
	//add check status register
}

//Format [byte2] [byte1]
unsigned short twi_read16(unsigned char reg, unsigned char addrw, unsigned char addrr){
	unsigned short data;
	unsigned short temp;
	twi_start();
	twi_write_addr_w(addrw);
	twi_write_byte(reg);
	twi_start();
	twi_write_addr_r(addrr);
	data = twi_read_byte();
	temp = twi_read_last_byte();
	twi_stop();
	temp <<= 8;
	data |= temp;
	return data;
}

//format [byte1][byte2][byte3]
uint32_t twi_read24REV(unsigned char reg, unsigned char addrw, unsigned char addrr){
	uint32_t data;
	twi_start();
	twi_write_addr_w(addrw);
	twi_write_byte(reg);
	twi_start();
	twi_write_addr_r(addrr);
	data = twi_read_byte();
	data <<= 8;
	data |=twi_read_byte();
	data <<= 8;
	data |= twi_read_last_byte();
	twi_stop();
	return data;
}