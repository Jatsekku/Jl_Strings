#include <stdint.h>
#include <stdlib.h>

size_t C_String32_GetLen(uint32_t* c_string)
{
	size_t c_string_len;
	for(c_string_len = 0; c_string[c_string_len] != '\0'; c_string_len++);
	return c_string_len;
}

