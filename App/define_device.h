#ifndef _DEFINE_DEVICE_
#define _DEFINE_DEVICE_

#include "stdint.h"

uint16_t def_hardware_ver = 0x2001;
uint16_t def_firmware_ver = 0x0501;
char def_serial_number[50] = "Intins Inc, LS-01, 163049, V1.0.5";
uint8_t def_number_of_channel = 1;
uint16_t def_max_curr_can_pro = 2000;
uint16_t def_max_curr_can_take = 1500;
uint16_t def_current = 500;
uint8_t hardware_flag = 0;
uint8_t firmware_flag = 0;
uint8_t serial_flag =0;
uint8_t number_channel_flag = 0;
uint8_t max_curr_pro_flag = 0;
uint8_t max_curr_take_flag = 0;
uint8_t curr_flag = 0;
uint8_t not_serial[50];

#endif
