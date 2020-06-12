#ifndef TWI_H
#define TWI_H

#include <avr/io.h>

void twi_init();
void twi_start();
void twi_write_addr_w(unsigned char addr); //send addr+[write bit]
void twi_write_byte(unsigned char data);
void twi_write_addr_r(unsigned char addr); //send addr+[read bit]
unsigned char twi_read_byte(); //Reads incoming byte and returns ACK
unsigned char twi_read_last_byte(); //Reads incoming byte and returns NACK
void twi_stop();

unsigned short twi_read16(unsigned char reg, unsigned char addrw, unsigned char  addrr); //read 16 bits
uint32_t twi_read24REV(unsigned char reg, unsigned char addrw, unsigned char addrr); //read 24 bits, return in reverse order




#endif