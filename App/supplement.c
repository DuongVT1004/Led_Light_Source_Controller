#include "supplement.h"


void reverse_array(uint8_t *arr, uint16_t start, uint16_t end)
{
  uint8_t a;
	uint16_t length = end - start + 1;
	for(int i = start; i <= (length + start)/2; i++)
	{
	  a = arr[i];
		arr[i] = arr[end];
		arr[end] = a;
		end--;
	}
}

uint32_t tranfer_hex_to_dec(uint8_t *n, uint8_t length)
{
	uint32_t s = 0;
	for(int i = 0; i < length; i++)
	{
		s += ((uint8_t)(n[i] >> 4)) * pow(16, (length-i)*2-1) + ((uint8_t)(n[i] & 0x0F)) * pow(16, (length-i)*2-2);
  }
  return s;
}

uint16_t transfer_array2_to_hex(uint8_t *arr)
{
	uint16_t s;
	s = ((uint16_t)arr[0]) | (((uint16_t)arr[1]) << 8);
	return s;
}

void invert_integer32(uint32_t *a)
{
	*a = (*a >> 24) | (((*a) >> 8) & 0x0000FF00) | (((*a) << 8) & 0x00FF0000) | (*a << 24);
}
