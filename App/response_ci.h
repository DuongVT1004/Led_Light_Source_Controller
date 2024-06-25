#ifndef RESPONSE_CI_H
#define RESPONSE_CI_H

#include "stm32f1xx_hal.h"

void response_print(const char *str,...);
void response_init(UART_HandleTypeDef *_huart_print);
void response_hex(uint8_t *buf, uint32_t length);

#endif
