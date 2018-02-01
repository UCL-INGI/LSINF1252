#include<stdint.h>
#include<stdbool.h>
uint8_t nbits(uint32_t n);
uint64_t set_bit(uint64_t x, int pos, bool value);
uint8_t get_3_leftmost_bits(uint32_t x);
uint8_t get_4_rightmost_bits(uint32_t x);
uint32_t unset_strong_bit(uint32_t x);
uint32_t cycle_bits(uint32_t x, uint32_t n);
