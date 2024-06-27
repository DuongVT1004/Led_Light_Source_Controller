#include "device_control.h"
#include "flash.h"
#include <string.h>
#include "response_ci.h"
#include "supplement.h"
#include "i2c.h"
#include "define_device.h"

#define ADDR_DEVICE_INFO 0x800A000

res_data_field r_frame; // frame phan hoi chua co data
uint8_t res_frame_no_err[100]; // frame phan hoi neu khong co loi bao gom ca data
uint8_t res_frame_err[14];   // frame phan hoi neu co loi 
error_type state1;
error_type state2;

res_data_init res_init = {
	.r_Start_Syn = 0xFBFA,
	.r_Protocol_Ver = 0x0011,
	.r_Message_Type = 0,
	.r_Data_Len = 0,
	.r_End_Syn = 0xFDFC
};

err_code_typedef err_code_init = {
	.success = 0,
	.invalid_protocol = 0x0100,
	.unknown_mes_type = 0x0200,
	.mes_too_large = 0x0300,
	.data_len_not_match = 0x0400,
	.invalid_data = 0x0500,
	.infor_not_esit = 0x0600
};

static void respone_message_init()
{
	memcpy(r_frame.Start_Sync, &res_init.r_Start_Syn, 2);
	memcpy(r_frame.Protocol_Ver, &res_init.r_Protocol_Ver, 2);
	memcpy(r_frame.Message_Type, &res_init.r_Message_Type, 2);
	memcpy(r_frame.Data_Len, &res_init.r_Data_Len, 4);
	memcpy(r_frame.End_Sync, &res_init.r_End_Syn, 2);
}

void number_index_init(device_typedef *device_info)
{
	for(int i = 0; i < 8; i++)
	{
		device_info->channel[i].channel_index = i + 1;
	}
}

error_type detect_invalid_data_unknow_mes_error(device_typedef *device_info, data_field s_frame, uint8_t *data)
{
	//take_device_info_from_flash(device_info);
	uint16_t mess_type_int;
	uint8_t chan;
	mess_type_int = ((uint16_t)s_frame.Message_Type[0] << 8) | ((uint16_t)s_frame.Message_Type[1]);
	uint16_t data_2;
	switch (mess_type_int)
	{
		case 0x0803:	
		case 0x0808:
		case 0x080B:
			if(data[0] > 7) return NO_DESIRED_INFO;
			return SUCCES;
			break;
		case 0x0801:
			if (data[0] > 8) return INVALID_DATA;
			return SUCCES;
			break;
		case 0x0802:
			chan = data[0];
			if(chan > 7) return INVALID_DATA;
			data_2 = transfer_array2_to_hex(data+1);
			uint16_t a = device_info->channel[chan].max_curr_can_take;
			a = (a >> 8) | (a << 8);
			if(a < data_2) return INVALID_DATA;
			return SUCCES;
			break;
		case 0x080A:
			chan = data[0];
		  	if(chan > 7) return INVALID_DATA;
		  	data_2 = transfer_array2_to_hex(data+1);
			uint16_t b = device_info->channel[chan].max_curr_prov;
			b = (b >> 8) | (b << 8);
			if(b < data_2) return INVALID_DATA;
			return SUCCES;
			break;
		case 0x0807:
			chan = data[0];
			if(chan > 7) return INVALID_DATA;
			data_2 = transfer_array2_to_hex(data+1);
			if(data_2 > 2500) return INVALID_DATA;
			return SUCCES;
			break;
		case 0x0010:
			break;
		case 0x0011:
			break;
		case 0x0020:
			break;
		case 0x0021:
			break;
		case 0x0012:
			break;
		case 0x0800:
			break;
		case 0x080D:
			break;
		case 0x080E:
			break;
		default:
			return UNKNOW_MES;
	}
}

error_type err_cli_handle(data_field s_frame, uint16_t data_len)
{
//	res_init.r_Data_Len = 0x00000000;
//	respone_message_init();
	if(memcmp(s_frame.Start_Sync, r_frame.Start_Sync, 2) != 0 || memcmp(s_frame.Protocol_Ver, r_frame.Protocol_Ver, 2) != 0 || memcmp(s_frame.End_Sync, r_frame.End_Sync, 2) != 0)
	{
		return INVALID_STRUCT;
	}
	
	if(sizeof(s_frame.Message_Type) > 4)
	{
		return LARGE_MES;
	}
	
	if(data_len != tranfer_hex_to_dec(s_frame.Data_Len, 4))
	{
		return NOT_MATCH_DATA_LEN;
	}
	
	return SUCCES;	
}

error_type error_excute(device_typedef *device_info, data_field s_frame, uint8_t *data, uint16_t data_len)
{
	res_init.r_Data_Len = 0x00000000;
	respone_message_init();
	state1 = detect_invalid_data_unknow_mes_error(device_info, s_frame, data);
	state2 = err_cli_handle(s_frame, data_len);
	if(state1 == SUCCES && state2 == SUCCES)
	{
		return SUCCES;
	}
	if(state1 == NO_DESIRED_INFO)
	{
		memcpy(r_frame.Err_Number, &err_code_init.infor_not_esit, 2);
		memcpy(res_frame_err, &r_frame, 14);
		response_hex(res_frame_err, 14);
		response_print("Command is valid but desired information does not esit");
		return NO_DESIRED_INFO;
	}
	else if(state1 == INVALID_DATA)
	{
		memcpy(r_frame.Err_Number, &err_code_init.invalid_data, 2);
		memcpy(res_frame_err, &r_frame, 14);
		response_hex(res_frame_err, 14);
		response_print("invalid data");
		return INVALID_DATA;
	}
	else if(state1 == UNKNOW_MES)
	{
		memcpy(r_frame.Err_Number, &err_code_init.unknown_mes_type, 2);
		memcpy(res_frame_err, &r_frame, 14);
		response_hex(res_frame_err, 14);
		response_print("Unknow message type");
		return UNKNOW_MES;
	}
	if(state2 == INVALID_STRUCT)
	{
		memcpy(r_frame.Err_Number, &err_code_init.invalid_protocol, 2);
		memcpy(res_frame_err, &r_frame, 14);
		response_hex(res_frame_err, 14);
		response_print("invalid protocol");
		return INVALID_DATA;
	}
	else if(state2 == LARGE_MES)
	{
		memcpy(r_frame.Err_Number, &err_code_init.mes_too_large, 2);
		memcpy(res_frame_err, &r_frame, 14);
		response_hex(res_frame_err, 14);
		response_print("Message too large");
		return LARGE_MES;
	}
	else if(state2 == NOT_MATCH_DATA_LEN)
	{
		memcpy(r_frame.Err_Number, &err_code_init.data_len_not_match, 2);
		memcpy(res_frame_err, &r_frame, 14);
		response_hex(res_frame_err, 14);
		response_print("Data Length does not match message type");
		return NOT_MATCH_DATA_LEN;
	}
}

void take_device_info_from_flash(device_typedef *device_info)
{
	flash_read_arr(ADDR_DEVICE_INFO, (uint8_t*)device_info, sizeof(device_typedef));
}

void erase_device_info()
{
	flash_erease(ADDR_DEVICE_INFO);
}

void get_hardware_version(device_typedef *device_info, uint8_t *data)
{
	(void)data;
	take_device_info_from_flash(device_info);
	res_init.r_Message_Type = 0x1000;
	res_init.r_Data_Len = 0x02000000;
	respone_message_init();
	//flash_read_arr(ADDR_DEVICE_INFO, (uint8_t*)device_info, sizeof(device_typedef));                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
	memcpy(res_frame_no_err, &r_frame, 12);
	memcpy(res_frame_no_err + 12, (uint8_t*)&(device_info->hardware_ver), 2);
	memcpy(res_frame_no_err + 14, &r_frame.End_Sync, 2);
	response_hex(res_frame_no_err, 16);
}

void get_firmware_version(device_typedef *device_info, uint8_t *data)
{
	(void)data;
	take_device_info_from_flash(device_info);
	res_init.r_Message_Type = 0x1100;
	res_init.r_Data_Len = 0x02000000;
	respone_message_init();
	reverse_uint16((uint16_t*)(&device_info->firmware_ver));                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
	memcpy(res_frame_no_err, &r_frame, 12);
	memcpy(res_frame_no_err + 12, (uint8_t*)&(device_info->firmware_ver), 2);
	memcpy(res_frame_no_err + 14, &r_frame.End_Sync, 2);
	response_hex(res_frame_no_err, 16);
}

void get_serial_number(device_typedef *device_info, uint8_t *data)
{
	take_device_info_from_flash(device_info);
	res_init.r_Message_Type = 0x1100;
	res_init.r_Data_Len = 0x32000000;
	respone_message_init();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
	memcpy(res_frame_no_err, &r_frame, 12);
	memcpy(res_frame_no_err + 12, (uint8_t*)&(device_info->serial_number), 50);
	//reverse_array(res_frame_no_err, 12, 61);
	memcpy(res_frame_no_err + 62, &r_frame.End_Sync, 2);
	response_hex(res_frame_no_err, 64);
}

void set_hardware_version(device_typedef *device_info, uint8_t *data)
{
	take_device_info_from_flash(device_info);
	memcpy((uint8_t *)&(device_info->hardware_ver), data, 2);
	flash_erase_and_write(ADDR_DEVICE_INFO, (uint8_t *)device_info, sizeof(device_typedef));	
	//HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, 0x800A000, device_info.hardware_ver);
}

void set_firmware_version(device_typedef *device_info, uint8_t *data)
{
	take_device_info_from_flash(device_info);
	memcpy((uint8_t *)&(device_info->firmware_ver), data, 2);
	flash_erase_and_write(ADDR_DEVICE_INFO, (uint8_t *)device_info, sizeof(device_typedef));	
	//HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, 0x800A000, device_info.hardware_ver);
}

void set_serial_number(device_typedef *device_info, uint8_t *data)
{
	take_device_info_from_flash(device_info);
	memcpy((uint8_t *)&(device_info->serial_number), data, 50);
	flash_erase_and_write(ADDR_DEVICE_INFO, (uint8_t *)device_info, sizeof(device_typedef));	
	//HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, 0x800A000, device_info.hardware_ver);
}

void get_number_of_channel(device_typedef *device_info, uint8_t *data)
{
	(void)data;
	take_device_info_from_flash(device_info); 
	res_init.r_Message_Type = 0x0008;
	res_init.r_Data_Len = 0x01000000;
	respone_message_init();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
	memcpy(res_frame_no_err, &r_frame, 12);
	memcpy(res_frame_no_err + 12, (uint8_t*)&(device_info->number_of_channel), 1);
	memcpy(res_frame_no_err + 13, &r_frame.End_Sync, 2);
	response_hex(res_frame_no_err, 15);
}

void set_number_of_channel(device_typedef *device_info, uint8_t *data)
{
	take_device_info_from_flash(device_info);
	memcpy((uint8_t *)&(device_info->number_of_channel), data, 1);
	flash_erase_and_write(ADDR_DEVICE_INFO, (uint8_t *)device_info, sizeof(device_typedef));		
}

void set_max_current_can_provide(device_typedef *device_info, uint8_t *data)
{
	take_device_info_from_flash(device_info);
	uint8_t chan = data[0];
	if(chan == 0x00)
	{
		for(int i = 0; i < device_info->number_of_channel; i++)
		{
			//memcpy((uint8_t *)&(device_info->channel[i].channel_index), &chan, 1);
			memcpy((uint8_t *)&(device_info->channel[i].max_curr_prov), data+1, 2);
		}
	}else if(chan != 0x00)
	{
		memcpy((uint8_t *)&(device_info->channel[chan].max_curr_prov), data+1, 2);
	}
	flash_erase_and_write(ADDR_DEVICE_INFO, (uint8_t *)device_info, sizeof(device_typedef));		
	//transmit_data_to_max5215(MAX5215_ADDRESS, CODE_LOAD, data+1, 300);
}

void get_max_current_can_provide(device_typedef *device_info, uint8_t *data)
{
	(void)data;
	take_device_info_from_flash(device_info); 
	uint8_t chan = data[0];
	res_init.r_Message_Type = 0x0808; 
	res_init.r_Data_Len = 0x02000000;
	if(chan == 0x00)
	{
		res_init.r_Data_Len = 2 * device_info->number_of_channel;
		invert_integer32(&res_init.r_Data_Len);
		respone_message_init();
		memcpy(res_frame_no_err, &r_frame, 12); 
		for(int i = 0; i < device_info->number_of_channel; i++)
		{
			//reverse_uint16((uint16_t*)(&device_info->channel[i].max_curr_prov));
			memcpy(res_frame_no_err + 12 + i*2, (uint8_t*)&(device_info->channel[i].max_curr_prov), 2);
		}
		memcpy(res_frame_no_err + 12 + device_info->number_of_channel * 2, &r_frame.End_Sync, 2);
		response_hex(res_frame_no_err, 14 + device_info->number_of_channel * 2);
	}else if(chan != 0x00)
	{
		memcpy(res_frame_no_err, &r_frame, 12);
		memcpy(res_frame_no_err + 12, (uint8_t*)&(device_info->channel[chan].max_curr_prov), 2);
		memcpy(res_frame_no_err + 14, &r_frame.End_Sync, 2);
		memcpy(res_frame_no_err, &r_frame, 12);
		response_hex(res_frame_no_err, 16);
	}
	
}

void set_max_current_can_take(device_typedef *device_info, uint8_t *data)
{
	take_device_info_from_flash(device_info);
	uint8_t chan = data[0];
	if(chan == 0x00)
	{
		for(int i = 0; i < device_info->number_of_channel; i++)
		{
			//memcpy((uint8_t *)&(device_info->channel[i].channel_index), &chan, 1);
			memcpy((uint8_t *)&(device_info->channel[i].max_curr_can_take), data+1, 2);
		}
	}else if(chan != 0x00)
	{
		memcpy((uint8_t *)&(device_info->channel[chan].max_curr_can_take), data+1, 2);
	}
	flash_erase_and_write(ADDR_DEVICE_INFO, (uint8_t *)device_info, sizeof(device_typedef));
//	memcpy((uint8_t *)&(device_info->channel[chan].channel_index), &chan, 1);
//	memcpy((uint8_t *)&(device_info->channel[chan].max_curr_can_take), data+1, 2);
	//flash_erase_and_write(ADDR_DEVICE_INFO, (uint8_t *)device_info, sizeof(device_typedef));		
	//response_print("%d", sizeof(device_typedef));
}

void get_max_current_can_take(device_typedef *device_info, uint8_t *data)
{
	(void)data;
	take_device_info_from_flash(device_info);
	uint8_t chan = data[0];
	res_init.r_Message_Type = 0x0B08;
	res_init.r_Data_Len = 0x02000000;
	if(chan == 0x00)
	{
		res_init.r_Data_Len = 2 * device_info->number_of_channel;
		invert_integer32(&res_init.r_Data_Len);
		respone_message_init();
		memcpy(res_frame_no_err, &r_frame, 12); 
		for(int i = 0; i < device_info->number_of_channel; i++)
		{
			memcpy(res_frame_no_err + 12 + i*2, (uint8_t*)&(device_info->channel[i].max_curr_can_take), 2);
		}
		memcpy(res_frame_no_err + 12 + device_info->number_of_channel * 2, &r_frame.End_Sync, 2);
		response_hex(res_frame_no_err, 14 + device_info->number_of_channel * 2);
	}else if(chan != 0x00)
	{
		memcpy(res_frame_no_err, &r_frame, 12);
		memcpy(res_frame_no_err + 12, (uint8_t*)&(device_info->channel[chan].max_curr_can_take), 2);
		memcpy(res_frame_no_err + 14, &r_frame.End_Sync, 2);
		memcpy(res_frame_no_err, &r_frame, 12);
		response_hex(res_frame_no_err, 16);
	}
}

void turn_on_channel(device_typedef *device_info, uint8_t *data) // message type 0x0802
{
	// Nhan cai dat dong dien o cac ham set_current va truyen I2C o ham nay
	take_device_info_from_flash(device_info);
	uint8_t chan = data[0];
	if(chan == 0x00)
	{
		for(int i = 0; i < device_info->number_of_channel; i++)
		{
			memcpy((uint8_t *)&(device_info->channel[i].channel_current), data+1, 2);
		}
	}else if(chan != 0x00)
	{
		memcpy((uint8_t *)&(device_info->channel[chan].channel_current), data+1, 2);
	}
	//memcpy((uint8_t *)&(device_info->channel[chan].channel_index), &chan, 1);
	//flash_erase_and_write(ADDR_DEVICE_INFO, (uint8_t *)device_info, sizeof(device_typedef));	
	// C� nhi�u k�nh hon th� c�n sua doan code n�y
	//transmit_data_to_max5215(MAX5215_ADDRESS, CODE_LOAD, (uint8_t *)&(device_info->channel[chan].channel_current), 300);
	turn_on((uint8_t*)&(device_info->channel[chan].channel_current));
}

void turn_off_channel(device_typedef *device_info, uint8_t *data)
{
	// set dong dien ve 0 va truyen I2C
	take_device_info_from_flash(device_info);
	uint8_t chan = data[0];
	//memcpy((uint8_t *)&(device_info->channel[chan].channel_index), &chan, 1);
	if(chan == 0x00)
	{
		for(int i = 0; i < device_info->number_of_channel; i++)
		{
			device_info->channel[i].channel_current = 0;
		}
	}else if (chan != 0x00)
	{
		device_info->channel[chan].channel_current = 0;
	}
	//flash_erase_and_write(ADDR_DEVICE_INFO, (uint8_t *)device_info, sizeof(device_typedef));	
	//transmit_data_to_max5215(MAX5215_ADDRESS, CODE_LOAD, (uint8_t *)&(device_info->channel[chan].channel_current), 300);
	turn_off();
}

void save_curr_setting(device_typedef *device_info, uint8_t *data) // 0x080D
{
	(void)data;
	for(int i = 0; i < device_info->number_of_channel; i++)
	{
		reverse_uint16((uint16_t*)(&device_info->channel[i].channel_current));
	}
	flash_erase_and_write(ADDR_DEVICE_INFO, (uint8_t *)device_info, sizeof(device_typedef));	
}

void get_curr_setting(device_typedef *device_info, uint8_t *data)
{
	(void)data;
	take_device_info_from_flash(device_info);
	//uint8_t chan = data[0];
	res_init.r_Message_Type = 0x0E08;
	res_init.r_Data_Len = device_info->number_of_channel * 2;
	invert_integer32(&res_init.r_Data_Len);
	respone_message_init();
	memcpy(res_frame_no_err, &r_frame, 12); 
	for(int i = 0; i < device_info->number_of_channel; i++)
	{
		memcpy(res_frame_no_err + 12 + i*2, (uint8_t*)&(device_info->channel[i].channel_current), 2);
	}
	memcpy(res_frame_no_err + 12 + device_info->number_of_channel * 2, &r_frame.End_Sync, 2);
	response_hex(res_frame_no_err, 14 + device_info->number_of_channel * 2);
}

void device_infomation_init(device_typedef *device_info)
{
	take_device_info_from_flash(device_info);
	memset(not_serial, 0xFF, 50);
	if(device_info->hardware_ver == 0xFFFF)
	{
		device_info->hardware_ver = def_hardware_ver;
		hardware_flag = 1;
	}
	if(device_info->firmware_ver == 0xFFFF)
	{
		device_info->firmware_ver = def_firmware_ver;
		firmware_flag = 1;
	}
	if(memcmp(device_info->serial_number, not_serial, 50) == 0)
	{
		memcpy(device_info->serial_number, def_serial_number, sizeof(def_serial_number));
		serial_flag = 1;
	}
	if(device_info->number_of_channel == 0xFF)
	{
		device_info->number_of_channel = def_number_of_channel;
		number_channel_flag = 1;
	}
	number_index_init(device_info);
	for(int i = 0; i < 8; i++)
	{
		if(device_info->channel[i].max_curr_prov == 0xFFFF)
		{
			device_info->channel[i].max_curr_prov = def_max_curr_can_pro;
			max_curr_pro_flag = 1;
		}
		if(device_info->channel[i].max_curr_can_take == 0xFFFF)
		{
			device_info->channel[i].max_curr_can_take = def_max_curr_can_take;
			max_curr_take_flag = 1;
		}
		if(device_info->channel[i].channel_current == 0xFFFF)
		{
			device_info->channel[i].channel_current = def_current;
			curr_flag = 1;
		}
	}
	if(hardware_flag == 1 || firmware_flag == 1 || serial_flag == 1 || number_channel_flag == 1 || max_curr_pro_flag == 1 || max_curr_take_flag == 1 || curr_flag == 1)
	{
		flash_erase_and_write(ADDR_DEVICE_INFO, (uint8_t *)device_info, sizeof(device_typedef));
	}
}
