#include "flash.h"

// unlock flash
void flash_unlock()
{
	HAL_FLASH_Unlock();
}

// lock flash
void flash_lock()
{
	HAL_FLASH_Lock();
}

// erase one page of flash
void flash_erease(uint32_t addr)
{
	FLASH_EraseInitTypeDef eraseInit;
	eraseInit.PageAddress = addr;
	eraseInit.NbPages = 1;
	eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	uint32_t PageError;
	HAL_FLASHEx_Erase(&eraseInit,&PageError);
}

// write to flash half word per time
void flash_write_arr(uint32_t addr,uint8_t *p,uint16_t len)
{
	for(uint16_t i =0;i<len;i+=2)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addr +i,*(p +i)|((uint16_t)*(p+i+1)<<8));
	}
	
}

// read flash
void flash_read_arr(uint32_t addr,uint8_t *data,uint16_t len)
{
	for(uint16_t i =0;i<len;i+=2)
	{
		uint16_t data_temp = *(volatile uint32_t *)(addr + i);
		*(data + i) = data_temp;
		*(data + i+1) = data_temp>>8;
	}
}

// erase and write to flash
void flash_erase_and_write(uint32_t addr,uint8_t *p,uint16_t len)
{
	flash_unlock();
	flash_erease(addr);
	flash_write_arr(addr, p, len);
	flash_lock();
}
