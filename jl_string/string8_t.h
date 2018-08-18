#ifndef JL_STRING_STRING8_T_H_
#define JL_STRING_STRING8_T_H_
#include <stdint.h>

typedef uint8_t* jl_string8_t;
typedef void*	 jl_string_container_t;

#define STORAGE_SIZE_OF_size_t	4

#define JL_STRING8_TYPE_L8		1
#define JL_STRING8_TYPE_L16		2
#define JL_STRING8_TYPE_L24		3
#define JL_STRING8_TYPE_L32		4
#define JL_STRING8_TYPE_L40		5
#define JL_STRING8_TYPE_L48		6
#define JL_STRING8_TYPE_L56		7
#define JL_STRING8_TYPE_L64		8



size_t C_String8_GetSize(uint8_t* c_string);
jl_string8_t Jl_String8_New(uint8_t* c_string);
size_t Jl_String8_GetSize(jl_string8_t jl_string);
jl_string8_t Jl_String8_ChangeSize(jl_string8_t jl_string, size_t new_data_size);

#endif
