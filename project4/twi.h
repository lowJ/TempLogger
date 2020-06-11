#ifndef TWI_H
#define TWI_H
#include <avr/io.h>

void twi_init();
void twi_start();
void twi_write_addr_w(unsigned char addr);
void twi_write_byte(unsigned char data);
void twi_write_addr_r(unsigned char addr);
unsigned char twi_read_byte();
unsigned char twi_read_last_byte();
void twi_stop();
unsigned short twi_read16(unsigned char reg, unsigned char addrw, unsigned char  addrr);
uint32_t twi_read24REV(unsigned char reg, unsigned char addrw, unsigned char addrr);




#endif