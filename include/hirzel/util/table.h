
#define HIRZEL_UTIL_TABLE_T int
// Error if no type declared
#ifndef HIRZEL_UTIL_TABLE_T
#error An element type must be defined for container
#endif

// Util macros
#define CONCAT(a, b) a##b
#define LSTR(s) #s
#define STR(s) LSTR(s)

// Type agnostic macros
#define HXT_BASE_NAME(type) 	CONCAT(hxtbl_, type)
#define HXT_ELEM_NAME(type)		CONCAT(hxtbl_elem_, type)
#define HXT_STRUCT_NAME(base)	struct base
#define HXT_TYPEDEF_NAME(base)	CONCAT(base, _t)

// Convenient macro aliases
#define HXT_BASE				HXT_BASE_NAME(HIRZEL_UTIL_TABLE_T)
#define HXT_TYPE				HIRZEL_UTIL_TABLE_T
#define HXT_STRUCT				HXT_STRUCT_NAME(HXT_BASE)
#define HXT_ELEM_STRUCT			HXT_STRUCT_NAME(HXT_ELEM)
#define HXT_TYPEDEF				HXT_TYPEDEF_NAME(HXT_BASE)
#define HXT_ELEM_TYPEDEF		HXT_TYPEDEF_NAME(HXT_ELEM)

#define HXT_FUNC_BASE(base, postfix) CONCAT(base, postfix)
#define HXT_FUNC_NAME(name) HXT_FUNC_BASE(HXT_BASE, _##name)
#define HXT_FUNC_SIG(ret_type, func_name, ...)\
ret_type HXT_FUNC_NAME(func_name)(__VA_ARGS__)

// Declarations
#ifndef HIRZEL_UTIL_TABLE_H
#define HIRZEL_UTIL_TABLE_H

#include <stddef.h>

#define HXT_HASH_P	(97)
#define HXT_HASH_M	(1000000009)

typedef HXT_ELEM_STRUCT
{
	char *key;
	HXT_TYPE value;
} HXT_ELEM_TYPEDEF;

typedef HXT_STRUCT
{
	size_t size;	// size of data
	size_t count;	// amount of elements
	HXT_ELEM_STRUCT *data;
} HXT_TYPEDEF;

extern long long unsigned hxhash(const char *str);


#endif

#ifdef HIRZEL_UTIL_TABLE_I
#undef HIRZEL_UTIL_TABLE_I

long long unsigned hxhash(const char *str)
{
	long long unsigned hash = 0;
	long long unsigned pop = 1;

	while (*str)
	{
		hash = (hash + (*str - 'a' + 1) * pop) % HXT_HASH_M;
		pop = (pop * HXT_HASH_P) % HXT_HASH_M;
		str += 1;
	}

	return hash;
}

#endif
