#ifndef _DEVICE_CONTROL_H_
#define _DEVICE_CONTROL_H_

#include <stdint.h>
#include "data_frame.h"
#include "err_types.h"

//error_type err_cli_handle(data_field s_frame, uint16_t data_len);
//error_type detect_invalid_data_unknow_mes_error(device_typedef *device_info, data_field s_frame, uint8_t *data);
error_type error_excute(device_typedef *device_info, data_field s_frame, uint8_t *data, uint16_t data_len);
void take_device_info_from_flash(device_typedef *device_info);
void erase_device_info(void);
void get_hardware_version(device_typedef *device_info, uint8_t *data);
void get_firmware_version(device_typedef *device_info, uint8_t *data);
void get_serial_number(device_typedef *device_info, uint8_t *data);
void set_hardware_version(device_typedef *device_info, uint8_t *data);
void set_firmware_version(device_typedef *device_info, uint8_t *data);
void set_serial_number(device_typedef *device_info, uint8_t *data);
void get_number_of_channel(device_typedef *device_info, uint8_t *data);
void set_number_of_channel(device_typedef *device_info, uint8_t *data);
void set_max_current_can_provide(device_typedef *device_info, uint8_t *data);
void get_max_current_can_provide(device_typedef *device_info, uint8_t *data);
void set_max_current_can_take(device_typedef *device_info, uint8_t *data);
void get_max_current_can_take(device_typedef *device_info, uint8_t *data);
void turn_on_channel(device_typedef *device_info, uint8_t *data);
void turn_off_channel(device_typedef *device_info, uint8_t *data);
void save_curr_setting(device_typedef *device_info, uint8_t *data);
void get_curr_setting(device_typedef *device_info, uint8_t *data);
void device_infomation_init(device_typedef *device_info);

#endif

