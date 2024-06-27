#ifndef _SUPPLEMENT_H_
#define _SUPPLEMENT_H_

#include <stdint.h>
#include <math.h>

void reverse_array(uint8_t *arr, uint16_t start, uint16_t end);
uint32_t tranfer_hex_to_dec(uint8_t *n, uint8_t length);
uint16_t transfer_array2_to_hex(uint8_t *arr);
void invert_integer32(uint32_t *a);
void reverse_uint16(uint16_t *a);

#endif
