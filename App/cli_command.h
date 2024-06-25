#ifndef _CLI_COMMAND_H_
#define _CLI_COMMAND_H_
#include <stdint.h>
#include "err_types.h"
#include "data_frame.h"

void cli_command_handle(uint8_t *buff, uint16_t len);
void device_excute(void);
void device_handle(uint8_t *buffer, uint16_t len);
void initialize_device(void);
void erase_device(void);
//error_type err_cli_handle(void);
//void get_hardware_version(void);
//void get_firmware_version(void);
//void get_serial_number(void);
//void set_hardware_version(void);
//void set_firmware_version(void);
//void set_serial_number(void);
//void get_number_of_channel(void);
//void set_number_of_channel(void);
//void set_max_current_can_provide(void);
//void get_max_current_can_provide(void);
//void set_max_current_can_take(void);
//void get_max_current_can_take(void);

#endif
