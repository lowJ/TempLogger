#include "uart.h"
#include "avr.h"
#include <stdio.h>

#define F_CPU 8000000 //8Mhz
#define BAUDRATE 9600
#define BRC 51 //Value from data sheet, with 8Mhz crystal

#define TX_BUFFER_SIZE 64
#define RX_BUFFER_SIZE 64

char rx_buffer[RX_BUFFER_SIZE];
char tx_buffer[TX_BUFFER_SIZE];
int rx_pos = 0;
int tx_pos = 0;

//Configure registers for UART
void init_uart(){
	UCSRB |= (1 <<RXEN) | (1 << TXEN) | (1 << RXCIE);
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
	UBRRL = BRC;
	UBRRH = (BRC >> 8);
	SET_BIT(PORTB, 0);
}

int rx_get_pos(){
	return rx_pos;
}

void rx_set_pos(int p){
	rx_pos = p;
}

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

//Transmits a single char over serial
void send_tx_char(char c)
{
	while (! (UCSRA & (1<<UDRE)));
	UDR = c ;
}

//Transmits a string over serial
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

