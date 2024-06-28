#ifndef _CLI_COMMAND_H_
#define _CLI_COMMAND_H_
#include <stdint.h>
#include "err_types.h"
#include "data_frame.h"

// save data into s_frame from buffer
void cli_command_handle(uint8_t *buff, uint16_t len);
//excute function corresponding command
void device_excute(void);
// excute works: take data from flash, handle error, run device_excute function
void device_handle(uint8_t *buffer, uint16_t len);
// initialize device infomation
void initialize_device(void);
// erase device infomation
void erase_device(void);

#endif
