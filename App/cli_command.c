#include "cli_command.h"
#include "string.h"
#include "mes_list_func.h"
#include "device_control.h"

uint8_t data[100]; 

data_field s_frame; // frame to receive data
uint16_t data_len;  // length of frame
uint16_t mes_type;  // mesage type
device_typedef device_infomation;  // global variable to save infmation of device

// initialize device message types
device_message_type mess_code = {
	.get_hardware_ver = 0x0010,
	.get_firmware_ver = 0x0011,
	.get_serial_number = 0x0020,
	.set_serial_number = 0x0021,
	.set_prepro_mode = 0x0002,
	.set_hardware_ver = 0x0012,
	.wire_firmware = 0x0003
};

// initialize led mesage types
led_message_type led_code = {
	.get_number_of_channel = 0x0800,
	.set_number_of_channel = 0x0801,
	.turn_on_channel = 0x0802,
	.turn_off_channel = 0x0803,
	.set_max_current_pro = 0x0807,
	.get_max_current_pro = 0x0808,
	.set_max_current_can_take = 0x080A,
	.get_max_current_can_take = 0x080B,
	.save_current_setting_all = 0x080D,
	.get_current_setting_all = 0x080E,
	.turn_on_off_shutter = 0x080F
};

// save data into s_frame from buffer
void cli_command_handle(uint8_t *buffer, uint16_t len)
{
	memcpy(s_frame.Start_Sync, buffer, 2);
	memcpy(s_frame.Protocol_Ver, buffer + 2, 2);
	memcpy(s_frame.Err_Number, buffer + 4, 2);
	memcpy(s_frame.Message_Type, buffer + 6, 2);
	memcpy(s_frame.Data_Len, buffer + 8, 4);
	memcpy(s_frame.End_Sync, buffer + len - 2, 2);
	data_len = len - 14;  // length of data field
	for(int i = 12; i < (len - 2); i++)
	{
		data[i - 12] = buffer[i];
	}
	mes_type = (((uint16_t)s_frame.Message_Type[0]) << 8) | s_frame.Message_Type[1];  // message type of received frame
}

/*
	function to excute the work to be processed
	compare received message type to message type in library and excute corresponding function
*/
void device_excute()
{
	int i = 0;
	while(message_func_arr[i].message != 0)
	{
		if(message_func_arr[i].message == mes_type)
		{
			message_func_arr[i].d_function(&device_infomation, data);
		}
	  i++;	
	}
	
}

/*
	Handle device infomation
	- Take device infomation from flash and save to device_infomation variable
	- Save data to s_frame and calculate data field length and message type
	- Excute handle : 
		+ If error, response error
		+ If not error, excute command
*/
void device_handle(uint8_t *buffer, uint16_t len)
{
	take_device_info_from_flash(&device_infomation);
	cli_command_handle(buffer, len);
	if(error_excute(&device_infomation, s_frame, data, data_len) == SUCCES)
	{
		device_excute();
	}
}

// initialize device infomation (hardware version, firmware vesion, serial number, number of channel, type of currents)
void initialize_device()
{
	device_infomation_init(&device_infomation);
}

// erase device infomation
void erase_device()
{
	erase_device_info();
}
