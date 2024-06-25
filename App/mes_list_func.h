#ifndef _MES_LIST_FUNC_H_
#define _MES_LIST_FUNC_H_

#include "mes_type.h"
#include "device_control.h"

message_function_type message_func_arr[] = {
	{get_hardware_version, 0x0010},
	{get_firmware_version, 0x0011},
	{get_serial_number, 0x0020},
	{set_hardware_version, 0x0012},
	{set_serial_number, 0x0021},
	{get_number_of_channel, 0x0800},
	{set_number_of_channel, 0x0801},
	{get_max_current_can_provide, 0x0808},
	{set_max_current_can_provide, 0x0807},
	{get_max_current_can_take, 0x080B},
	{set_max_current_can_take, 0x080A},
	{turn_on_channel, 0x0802},
	{turn_off_channel , 0x0803},
	{save_curr_setting, 0x080D},
	{get_curr_setting, 0x080E},
	{0, 0}
	
};

#endif
