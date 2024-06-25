#ifndef _I2C_H_
#define _I2C_H_

#include "max5215.h"
#include "main.h"

void config_i2c(I2C_HandleTypeDef *i2c_config);
void transmit_data_to_max5215(uint8_t device_addr, uint8_t command, uint8_t *data, uint16_t timeout);
void driver_init(void);
void turn_on(uint8_t *data);
void turn_on_using_aux(uint8_t *data);
void turn_off(void);

#endif
