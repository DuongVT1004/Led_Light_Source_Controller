#include "i2c.h"
#include "stdlib.h"
#include "supplement.h"
#include "response_ci.h"
#include "math.h"
#include "max5215.h"
#include "bh1750.h"

#define VREF 2.5

uint16_t i2c_data;
uint16_t f_data;

I2C_HandleTypeDef *i2c;
//BH1750_device_t a;
//extern BH1750_device_t* test_dev;

void config_i2c(I2C_HandleTypeDef *i2c_config)
{
	i2c = i2c_config;
}

uint16_t i2c_data_handle(uint8_t *data)
{
	uint16_t i2c_data_dec;
	i2c_data_dec = tranfer_hex_to_dec(data, 2);
	return i2c_data_dec;
}

void transmit_data_to_max5215(uint8_t device_addr, uint8_t command, uint8_t *data, uint16_t timeout)
{
	i2c_data = i2c_data_handle(data);
	i2c_data = (i2c_data << 8) | (i2c_data >> 8);
	f_data = ((((float)i2c_data)/1000) * pow(2, 14)) / VREF;
	f_data = f_data << 2;
	uint8_t i2c_buf[3];
	i2c_buf[0] = command;
	i2c_buf[1] = (uint8_t)(f_data >> 8);
	i2c_buf[2] = (uint8_t)(f_data & 0x00FF);
	//response_hex(i2c_buf, 3);
	HAL_I2C_Master_Transmit(i2c, device_addr, i2c_buf, sizeof(i2c_buf), timeout);
}

void driver_init()
{
	uint8_t power_down_mode[2] = {0, 0x03};
	HAL_GPIO_WritePin(AUX_GPIO_Port, AUX_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_SET);
	transmit_data_to_max5215(MAX5215_ADDRESS, USER_CONFIG, power_down_mode, 300);
}

void turn_on(uint8_t *data)
{
	uint8_t power_down_mode[2] = {0x00, 0x00};
	transmit_data_to_max5215(MAX5215_ADDRESS, USER_CONFIG, power_down_mode, 300);
	transmit_data_to_max5215(MAX5215_ADDRESS, CODE_LOAD, data, 300);
//	test_dev->poll(test_dev);
//	a = *test_dev;
}

void turn_on_using_aux(uint8_t *data)
{
	HAL_GPIO_WritePin(AUX_GPIO_Port, AUX_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(AUX_GPIO_Port, AUX_Pin, GPIO_PIN_SET);
	transmit_data_to_max5215(MAX5215_ADDRESS, CODE_LOAD, data, 300);
}

void turn_off()
{
	uint8_t power_down_mode[2] = {0, 0x03};
	transmit_data_to_max5215(MAX5215_ADDRESS, USER_CONFIG, power_down_mode, 300);
}
