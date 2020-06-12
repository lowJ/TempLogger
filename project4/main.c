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

#define TX_BUFFER_SIZE 64
#define RX_BUFFER_SIZE 64

char rx_buffer[RX_BUFFER_SIZE];
char tx_buffer[TX_BUFFER_SIZE];
int rx_pos = 0;
int tx_pos = 0;

int test = 0;

int pause = 0;
float period;

void clear_rx_buffer(){
	for(int i = 0; i < RX_BUFFER_SIZE; ++i){
		rx_buffer[i] = 0;
	}
}

void clear_tx_buffer(){
	for(int i = 0; i < TX_BUFFER_SIZE; ++i){
		tx_buffer[i] = 0;
	}
}
void send_tx_char(char ch)
{
	while (! (UCSRA & (1<<UDRE)));  /* Wait for empty transmit buffer */
	UDR = ch ;
}
void send_tx_string(char s[]){
	int i; char cur;
	i=0;
	cur = s[i];
	while(cur != '\0'){
		cur = s[i];
		send_tx_char(s[i]);
		i++;
	}
}

void read_command(){
	if(rx_buffer[0] == 'p'){
		if(pause == 1){
			pause = 0;
		}
		else if(pause == 0){
			pause = 1;
		}
	}
	else if(rx_buffer[0] == 'n'){
		int i;
		char temp[8];
		temp[0] = rx_buffer[1]; temp[1] = rx_buffer[2]; temp[2] = '\0';
		sscanf(temp, "%d", &i);
		if(check_bounds(i)){
			float val;
			val = get_value(i);
			val /= 100;
			sprintf(temp,"%.2f", val);
			send_tx_string(temp);
			send_tx_char('\n');
		}
		else{
			send_tx_string("INVALID INDEX \n");
		}
		
	}
	else{
		send_tx_string("INVALID COMMAND \n");
	}
}

ISR(USART_RXC_vect)
{
	char cur = UDR;
	rx_buffer[rx_pos] = cur;
	if(cur == '\n'){
		rx_pos = 0;
		read_command();
		clear_rx_buffer();
	}
	else{
		rx_pos++;
	}
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
		
		avr_wait(1000);
		
    }
}

