#ifndef _MES_TYPE_H_
#define _MES_TYPE_H_

#include "data_frame.h"

typedef void (*device_function)(device_typedef* ,uint8_t*);

typedef struct
{
	device_function d_function;
	uint16_t message;
}message_function_type;

#endif
