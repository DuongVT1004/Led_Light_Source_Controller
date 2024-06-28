#ifndef _UART_H_
#define _UART_H_

#include <stdint.h>
/*
    - receive_data function : receive each byte of data frame and save to buffer
    - uart_handle: happpen when the data frame ends and handle data from buffer
*/
void receive_data(uint8_t);
void uart_handle(void);

#endif

