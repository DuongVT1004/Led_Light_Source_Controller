#include "uart.h"
#include "response_ci.h"
#include "cli_command.h"

#define MAX_SIZE_DATA 100
uint8_t buffer[MAX_SIZE_DATA];
uint16_t len = 0;
uint8_t flag = 0;
extern uint8_t counter;
extern TIM_HandleTypeDef htim2;

void receive_data(uint8_t data)
{
		__HAL_TIM_SET_COUNTER(&htim2, 0);
		if(len == 0)
		{
			 HAL_TIM_Base_Start_IT(&htim2);
		}
		buffer[len++] = data;
		counter = 0;
}

void uart_handle()
{
	if(flag)
	{
		response_print("%d", len);
//		cli_command_handle(buffer, len);
//		device_excute();
		device_handle(buffer, len);
		len = 0;
		flag = 0;
	}
}
