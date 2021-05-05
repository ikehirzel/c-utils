#define HIRZEL_UTIL_LIST_T int

// Error if no type declared
#ifndef HIRZEL_UTIL_LIST_T
#error An element type must be defined for container
#endif

// Util macros
#define CONCAT(a, b) a##b
#define LSTR(s) #s
#define STR(s) LSTR(s)

// Type agnostic macros
#define HXL_BASE_NAME(type) 	CONCAT(hxlist_, type)
#define HXL_STRUCT_NAME(base)	struct base
#define HXL_TYPEDEF_NAME(base)	CONCAT(base, _t)

// Convenient macro aliases
#define HXL_BASE				HXL_BASE_NAME(HIRZEL_UTIL_LIST_T)
#define HXL_TYPE				HIRZEL_UTIL_LIST_T
#define HXL_STRUCT				HXL_STRUCT_NAME(HXL_BASE)
#define HXL_TYPEDEF				HXL_TYPEDEF_NAME(HXL_BASE)

#define HXL_FUNC_NAME(base, postfix) CONCAT(base, postfix)
#define HXL_FUNC(name) HXL_FUNC_NAME(HXL_BASE, _##name)
#define HXL_FUNC_DEC(ret_type, func_name, ...)\
ret_type HXL_FUNC(func_name)(__VA_ARGS__)


// Declarations
#ifndef HIRZEL_UTIL_LIST_H
#define HIRZEL_UITL_LIST_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef HXL_STRUCT
{
	size_t len; 
	HXL_TYPE *data;
} HXL_TYPEDEF;


/**
 * @brief	Allocates instance of list
 * @return	pointer to list
 */
HXL_FUNC_DEC(HXL_STRUCT*, create,)
{
	HXL_STRUCT *out = (HXL_STRUCT*)malloc(sizeof(HXL_STRUCT));
	if (!out) return NULL;
	out->data = (HXL_TYPE*)malloc(1);
	if (!out->data)
	{
		free(out);
		return NULL;
	}
	out->len = 0;
	return out;
}

/**
 * @brief	Frees memory used by list instance
 */
HXL_FUNC_DEC(void, destroy, HXL_STRUCT *list)
{
	free(list->data);
	free(list);
}


HXL_FUNC_DEC(bool, push, HXL_STRUCT* list, HXL_TYPE item)
{
	size_t size = list->len * sizeof(HXL_TYPE);
	HXL_TYPE *tmp = (HXL_TYPE*)realloc(list->data, size + sizeof(HXL_TYPE));
	if (!tmp) return false;
	list->data = tmp;
	list->data[list->len] = item;
	list->len += 1;
	return true;
}

#endif // HIRZEL_UITL_LIST_H

// Definitions
#ifdef HIRZEL_UTIL_LIST_I
#undef HIRZEL_UTIL_LIST_I

#endif // HIRZEL_UTIL_LIST_I
