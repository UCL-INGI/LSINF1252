#include <stdio.h>
#include"student_code.h"

uint32_t unset_strong_bit(uint32_t x)
{
	if (!x)
		return 0;
	for (short i = 1; i<32; i++)
		if (!(x>>i))
			return x - (1<<(i-1));
	return x;
}
