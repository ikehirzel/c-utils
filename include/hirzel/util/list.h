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

#define HXL_FUNC_BASE(base, postfix) CONCAT(base, postfix)
#define HXL_FUNC_NAME(name) HXL_FUNC_BASE(HXL_BASE, _##name)
#define HXL_FUNC_SIG(ret_type, func_name, ...)\
ret_type HXL_FUNC_NAME(func_name)(__VA_ARGS__)


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
extern HXL_FUNC_SIG(HXL_STRUCT*, create,);

/**
 * @brief	Frees memory used by list instance
 */
extern HXL_FUNC_SIG(void, destroy, HXL_STRUCT *list);

/**
 * @brief	Pushes item to end of list
 */
extern HXL_FUNC_SIG(bool, push, HXL_STRUCT* list, HXL_TYPE item);


#endif // HIRZEL_UITL_LIST_H

// Definitions
#ifdef HIRZEL_UTIL_LIST_I
#undef HIRZEL_UTIL_LIST_I

HXL_FUNC_SIG(HXL_STRUCT*, create,)
{
	// allocate buffer
	HXL_STRUCT *out = (HXL_STRUCT*)malloc(sizeof(HXL_STRUCT));
	// check for failure
	if (!out) return NULL;
	// preinitialize array so it can be resized/ destroy won't fail
	out->data = (HXL_TYPE*)malloc(1);
	// check for failure
	if (!out->data)
	{
		free(out);
		return NULL;
	}
	// initialize as empty
	out->len = 0;

	return out;
}


HXL_FUNC_SIG(void, destroy, HXL_STRUCT *list)
{
	// free array
	free(list->data);
	// free struct
	free(list);
}


HXL_FUNC_SIG(bool, push, HXL_STRUCT* list, HXL_TYPE item)
{
	// calculate current isze of bufer in bytes
	size_t size = list->len * sizeof(HXL_TYPE);
	// allocate size + 1
	HXL_TYPE *tmp = (HXL_TYPE*)realloc(list->data, size + sizeof(HXL_TYPE));
	// check for failure
	if (!tmp) return false;
	// repoint buffer
	list->data = tmp;
	// put new item at end
	list->data[list->len] = item;
	// increment size
	list->len += 1;
	// success
	return true;
}

#endif // HIRZEL_UTIL_LIST_I

// Preprocessor cleanup
#undef HXL_STRUCT
#undef HXL_TYPEDEF
#undef HXL_TYPE
#undef HXL_BASE
#undef HIRZEL_UTIL_LIST_T
