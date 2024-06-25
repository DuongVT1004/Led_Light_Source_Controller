#ifndef _MAX5215_H_
#define _MAX5215_H_

#define MAX5215_ADDRESS 			0x1F << 1 // Pull up ADDR pin
#define CODE_LOAD 						0x01			// Write and load data to the CODE and DAC resgisters
#define CODE 									0x02			// Write data to the CODE register
#define LOAD_CMD							0x03			// Load current CODE register content to the DAC register
#define CODE_LOAD_M						0x05			// Similar to CODE_LOAD command, but accepts multiple sets of dual-byte data following the initial command byte
#define CODE_M								0x06			// Similar to CODE command, but accepts multiple sets of dual-byte data following the initial command byte 
#define USER_CONFIG						0x08			// User configuration command
#define SW_RESET							0x09			// Software reset
#define SW_CLEAR							0x0A			// Software command

#endif
