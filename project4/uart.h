#ifndef _UART_H_
#define _UART_H_

//allow the rx_buffer to be accessible by main
extern char rx_buffer[];

void init_uart(); //configure registers for UART
int rx_get_pos();
void rx_set_pos(int p);
void clear_rx_buffer();
void clear_tx_buffer();
void send_tx_char(char ch);
void send_tx_string(char s[]);

#endif