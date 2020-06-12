/*
 * project4.c
 *
 * Created: 6/5/2020 12:14:45 AM
 * Author : jonathan
 */ 

#include <avr/io.h>
#include "lcd.h"
#include "avr.h"
#include "twi.h"
#include "bmp280.h"
#include "store.h"

#include <avr/interrupt.h>

#define F_CPU 8000000
#define BAUDRATE 9600
#define BRC 51



int test = 0;
ISR(USART_RXC_vect)
{
	lcd_pos(1,test);
	lcd_put(UDR);
	test++;
}
void UART_TxChar(char ch)
{
	while (! (UCSRA & (1<<UDRE)));  /* Wait for empty transmit buffer */
	UDR = ch ;
}

int main(void)
{
	lcd_init();
	twi_init();
	
	//store_init();
	
	//init bmp280
	if(!bmp280_init()){
		lcd_pos(0,0);
		lcd_puts("ERROR!");
		lcd_pos(1,0);
		lcd_puts("BMP280 wrong ID");
		while(1);
	}
	//
	
	
	
	UCSRB |= (1 <<RXEN) | (1 << TXEN) | (1 << RXCIE);
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
	UBRRL = BRC;
	UBRRH = (BRC >> 8);
	
	SET_BIT(PORTB, 0);
	
	sei(); //from code for RX int
	
	//
	avr_wait(500);
    while (1) 
    {
		
		//UART_TxChar('A');
		lcd_pos(0,0);
		lcd_put('\n');
		avr_wait(1000);
		
    }
}

