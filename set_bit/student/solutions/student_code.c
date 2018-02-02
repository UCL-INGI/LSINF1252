#include"student_code.h"

uint64_t set_bit(uint64_t x, int pos, bool value)
{
if (value)
    return x | (1 << pos);
else
    return x & ~(1 << pos);
}
