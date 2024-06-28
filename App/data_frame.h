#ifndef _FRAME_H_
#define _FRAME_H_

#include <stdint.h>

// define fields, except data field 
typedef struct
{	
	uint8_t Start_Sync[2];
	uint8_t Protocol_Ver[2];
	uint8_t Err_Number[2];
	uint8_t Message_Type[2];
	uint8_t Data_Len[4];
	uint8_t End_Sync[2];
} data_field;

typedef struct
{
	uint8_t Start_Sync[2];
	uint8_t Protocol_Ver[2];
	uint8_t Err_Number[2];
	uint8_t Message_Type[2];
	uint8_t Data_Len[4];
	uint8_t End_Sync[2];
} res_data_field;

// init default fields
typedef struct {
	uint16_t r_Start_Syn;
	uint16_t r_Protocol_Ver;
	uint16_t r_Message_Type;
	uint32_t r_Data_Len;
	uint16_t r_End_Syn;
} res_data_init;

// init error code
typedef struct {
	uint16_t success;
	uint16_t invalid_protocol;
	uint16_t unknown_mes_type;
	uint16_t mes_too_large;
	uint16_t data_len_not_match;
	uint16_t invalid_data;
	uint16_t infor_not_esit;
	uint16_t device_busy;
}err_code_typedef;

// data of channel
typedef struct
{
	uint8_t channel_index;
	uint16_t max_curr_prov;
	uint16_t max_curr_can_take;
	uint16_t channel_current;
} __attribute__((packed)) channel_typedef;

// data of device
typedef struct
{
	uint16_t hardware_ver;
	uint16_t firmware_ver;
	uint8_t serial_number[50];
	uint8_t number_of_channel;
	channel_typedef channel[8];
} __attribute__((packed)) device_typedef;

// init device function to excute
typedef struct
{
	uint16_t get_hardware_ver;
	uint16_t get_firmware_ver;
	uint16_t get_serial_number;
	uint16_t set_serial_number;
	uint16_t set_prepro_mode;
	uint16_t set_hardware_ver;
	uint16_t wire_firmware;
} device_message_type;

// init channel functions to excute
typedef struct
{
	uint16_t get_number_of_channel;
	uint16_t set_number_of_channel;
	uint16_t turn_on_channel;
	uint16_t turn_off_channel;
	uint16_t set_max_current_pro;
	uint16_t get_max_current_pro;
	uint16_t set_max_current_can_take;
	uint16_t get_max_current_can_take;
	uint16_t save_current_setting_all;
	uint16_t get_current_setting_all;
	uint16_t turn_on_off_shutter;
}led_message_type;



#endif
