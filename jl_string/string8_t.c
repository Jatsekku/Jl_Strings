#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include "string8_t.h"

size_t C_String8_GetSize(uint8_t* c_string)
{
	size_t c_string_size;
	for(c_string_size = 0; c_string[c_string_size] != '\0'; c_string_size++);
	return c_string_size;
}

uint8_t C_String8_ReduceWhiteChars(uint8_t* pre, uint8_t* post)
{
	uint16_t idx_pre = 0;
	uint16_t idx_post = 0;
	uint8_t  flag_white_char = 0;

	while(pre[idx_pre] != '\0')
	{
		if( !((pre[idx_pre] == ' ') || (pre[idx_pre] == '\t')) )
		{
			flag_white_char = 0;
			post[idx_post++] = pre[idx_pre];
		}
		else if(!flag_white_char)
		{
			flag_white_char = 1;
			post[idx_post++] = ' ';
		}

		post[idx_post] = '\0';
		idx_pre++;
	}
	return idx_post;
}

static inline uint8_t Jl_String8_Size2Bytes(size_t string_size)
{
	size_t len_bytes = 1;
	while(string_size > len_bytes * 0xFF) len_bytes++;
	return len_bytes;
}

//jl_string_container_t Jl_String8_NewHeader(size_t data_size)

jl_string8_t Jl_String8_NewSkeleton(size_t data_size)
{
	size_t  jl_string_size_bytes;
	uint8_t jl_string_header_size;
	uint8_t jl_string_type;
	jl_string_container_t jl_string;
	jl_string8_t c_string_fake;
	uint8_t* ptr_tmp;

	if(!data_size)
		return 0;

	jl_string_size_bytes = Jl_String8_Size2Bytes(data_size);
	jl_string_type = jl_string_size_bytes;
	jl_string_header_size = jl_string_type + 1;
	jl_string = malloc(jl_string_header_size + data_size + 1);

	if(!jl_string)
		return 0;

	c_string_fake = (uint8_t*)jl_string + jl_string_header_size;
	ptr_tmp = jl_string;

	switch(jl_string_type)
	{
		case(JL_STRING8_TYPE_L8):
		{
			ptr_tmp[0] = data_size;
			ptr_tmp[1] = jl_string_type;
		}
		break;

		case(JL_STRING8_TYPE_L16):
		{

			ptr_tmp[0] = data_size;
			ptr_tmp[1] = data_size >> 8;
			ptr_tmp[2] = jl_string_type;
		}
		break;

		case(JL_STRING8_TYPE_L32):
		{
			ptr_tmp[0] = data_size;
			ptr_tmp[1] = data_size >> 8;
			ptr_tmp[2] = data_size >> 16;
			ptr_tmp[3] = jl_string_type;
		}
		break;
	}

	return c_string_fake;
}

jl_string8_t Jl_String8_New(uint8_t* c_string)
{
	size_t c_string_size;
	jl_string8_t c_string_fake;

	if(!c_string) return 0;
	else c_string_size = C_String8_GetSize(c_string);

	c_string_fake = Jl_String8_NewSkeleton(c_string_size);

	if(c_string_size && c_string_fake)
		memcpy(c_string_fake, c_string, c_string_size);

	c_string_fake[c_string_size] = '\0';

	return c_string_fake;
}

jl_string8_t Jl_String8_ChangeSize(jl_string8_t jl_string, size_t new_data_size)
{
	size_t  jl_string_size_bytes;
	uint8_t jl_string_header_size;
	uint8_t jl_string_type;
	jl_string8_t c_string_fake;

	uint8_t* ptr_tmp;
	int8_t idx_tmp = -1;

	jl_string_size_bytes = Jl_String8_Size2Bytes(new_data_size);
	jl_string_type = jl_string_size_bytes;
	jl_string_header_size = jl_string_type + 1;

	ptr_tmp = realloc(jl_string, jl_string_header_size + new_data_size + 1);

	c_string_fake = ptr_tmp + jl_string_header_size;


	switch(jl_string_type)
	{
		case(JL_STRING8_TYPE_L8):
		{
			ptr_tmp[0] = new_data_size;
			ptr_tmp[1] = jl_string_type;
		}
		break;

		case(JL_STRING8_TYPE_L16):
		{

			ptr_tmp[0] = new_data_size;
			ptr_tmp[1] = new_data_size >> 8;
			ptr_tmp[2] = jl_string_type;
		}
		break;

		case(JL_STRING8_TYPE_L32):
		{
			ptr_tmp[0] = new_data_size;
			ptr_tmp[1] = new_data_size >> 8;
			ptr_tmp[2] = new_data_size >> 16;
			ptr_tmp[3] = jl_string_type;
		}
		break;
	}

	return c_string_fake;

}

size_t Jl_String8_GetSize(jl_string8_t jl_string)
{
	size_t  data_size = 0;
	uint8_t jl_string_size_bytes = jl_string[-1];
	int8_t idx_tmp = -1;

	while(jl_string_size_bytes--)
		data_size |= jl_string[--idx_tmp] << (jl_string_size_bytes * 8);

	return data_size;
}

jl_string8_t Jl_String8_ReduceWhiteChars(jl_string8_t jl_string_to_reduce)
{
	size_t idx_pre = 0;
	size_t idx_post = 0;
	uint8_t  flag_white_char = 0;

	size_t jl_string_to_reduce_size = Jl_String8_GetSize(jl_string_to_reduce);
	jl_string8_t jl_reduced_string = Jl_String8_NewSkeleton(jl_string_to_reduce_size);

	while(jl_string_to_reduce[idx_pre] != '\0')
	{
		if( !((jl_string_to_reduce[idx_pre] == ' ') || (jl_string_to_reduce[idx_pre] == '\t')) )
		{
			flag_white_char = 0;
			jl_reduced_string[idx_post++] = jl_string_to_reduce[idx_pre];
		}
		else if(!flag_white_char)
		{
			flag_white_char = 1;
			jl_reduced_string[idx_post++] = ' ';
		}

		jl_reduced_string[idx_post] = '\0';
		idx_pre++;
	}
	return idx_post;
}
