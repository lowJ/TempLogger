/*
 * project4.c
 *
 * Created: 6/5/2020 12:14:45 AM
 * Author : jonathan
 */ 

#include <avr/interrupt.h>
#include <avr/io.h>
#include "lcd.h"
#include "avr.h"
#include "twi.h"
#include "bmp280.h"
#include "store.h"
#include "uart.h"

#define PERIOD 3000 //in ms

int pause = 0; //1 = pause sampling, 0 = resume

//Function handles all incoming UART commands
void read_command();

int main(void)
{
	lcd_init();
	twi_init();
	init_uart();
	sei(); //setup interrupt
	
	//init bmp280
	if(!bmp280_init()){
		lcd_pos(0,0);
		lcd_puts("ERROR!");
		lcd_pos(1,0);
		lcd_puts("BMP280 wrong ID");
		while(1);
	}
	
	avr_wait(50); //allow time for BMP to setup
	
    while (1) 
    {
		if(!pause){
			float temp;
			
			//83.46F will get stored as 8346, 
			//8346/100 = 83.46 divide by 100 to obtain correct value
			temp = convert_C_to_F(bmp280_calc_temp()) * 100;
			store_value((int)temp); //store in eeprom
			avr_wait(25);
			temp = get_value(0); //Get 0th sample from eeprom
			temp /= 100.0; lcd_pos(0,0);
			lcd_puts("0:"); lcd_put_float(temp, 0, 2);
			avr_wait(25);
			temp = get_value(1);
			temp /= 100.0; lcd_pos(0,8);
			lcd_puts("1:"); lcd_put_float(temp, 0, 10);
			avr_wait(25);
			temp = get_value(2);
			temp /= 100.0; lcd_pos(1,0);
			lcd_puts("2:"); lcd_put_float(temp, 1, 2);
			avr_wait(25);
			temp = get_value(3);
			temp /= 100.0; lcd_pos(1,8);
			lcd_puts("3:"); lcd_put_float(temp, 1, 10);
			avr_wait(25);
		}
		avr_wait(PERIOD);
    }
}

//Interrupt triggers when there is incoming serial data
ISR(USART_RXC_vect)
{
	char cur = UDR;
	rx_buffer[rx_get_pos()] = cur;
	if(cur == '\n'){//finished receiving data
		read_command(); //use data in rx_buffer to execute command
		rx_set_pos(0);
		clear_rx_buffer(); 
	}
	else{ //still receiving data
		rx_set_pos(rx_get_pos()+1);
	}
}

void read_command(){
	//if a pause command was recieved
	if(rx_buffer[0] == 'p'){
		if(pause == 1){
			pause = 0;
			send_tx_string("RESUME SAMPLING \n");
		}
		else if(pause == 0){
			pause = 1;
			send_tx_string("PAUSING SAMPLING \n");
		}
	}
	//if a get sample command was recieved
	else if(rx_buffer[0] == 'n'){
		int i; char temp[8];
		temp[0] = rx_buffer[1]; temp[1] = rx_buffer[2]; temp[2] = '\0';
		sscanf(temp, "%d", &i); //convert char[] into int
		if(check_bounds(i)){ //checks if bounds is valid
			float val = get_value(i);
			avr_wait(25);
			val /= 100;
			sprintf(temp,"%.2f", val); //convert float to char[]
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
