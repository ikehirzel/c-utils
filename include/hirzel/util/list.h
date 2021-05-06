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
#define hxlist_destroy(list) free(list->data); free(list)

/**
 * @brief	Pushes item to end of list
 */
extern HXL_FUNC_SIG(bool, push, HXL_STRUCT *list, HXL_TYPE item);

/**
 * @brief	Gets item at a given index.
 * 
 * This function is bounds-checked and will return the desired element if i
 * is in bounds, but an uninitialized instance if it is not. 
 * 
 * @param	list	hxlist instance
 * @param	i		index of element
 * 
 * @return	Copy of item
 */
#define hxlist_at(list, i) ((i < list->len) ? list->data[i] : 0)

/**
 * @brief	Gets reference to item at given index.
 * 
 * This function is bounds-checked and will return a valid reference if i is
 * in bounds, but NULL if it is not.
 * 
 * @param	list	hxlist instance
 * @param	i		index of element
 * 
 * @return	Reference to item or NULL
 */
extern HXL_FUNC_SIG(HXL_TYPE*, getr, HXL_STRUCT *list, size_t i);

extern HXL_FUNC_SIG(HXL_TYPE*, set, HXL_STRUCT *list, size_t i);

/**
 * @brief	Gets item at given position.
 * 
 * This function is implemented as a macro for efficiency, but is
 * not bounds-checked.
 * 
 * @param	list	hxlist instance
 * @param	i		index of element
 * 
 * @return	Copy of item
 */
#define hxlist_get(list, i) (list->data[i])



/**
 * @brief	Gets pointer to item at given position.
 * 
 * This function is implemented as a macro for efficiency, but is
 * not bounds-checked.
 * 
 * @param	list	hxlist (of any type) instance
 * @param	i		index of element
 * 
 * @return	Refence to item
 */
#define hxlist_getr(list, i) (list->data + i)

/**
 * @brief	Sets item at given position.
 * 
 * This function is implemented as a macro for efficiency, but is
 * not bounds-checked
 * 
 * @param	list	hxlist (of any type) instance
 * @param	i		index of element
 * @param	val		new value to assign
 */
#define hxlist_set(list, i, val) list->data[i] = val

#define hxlist_set_c(list, i, val) if (i < list->len) list->data[i] = val

/*******************************
 * OTHER MACROS TO DOCUMENT
 ******************************/

#define hxlist_front(list) (list->data[0])
#define hxlist_back(list) (list->data[list->len - 1])
#define hxlist_empty(list) (list->len == 0)




#endif // HIRZEL_UITL_LIST_H

// Definitions
#ifdef HIRZEL_UTIL_LIST_I
#undef HIRZEL_UTIL_LIST_I

// CREATE
HXL_FUNC_SIG(HXL_STRUCT*, create,)
{
	// allocate buffer
	HXL_STRUCT *out = (HXL_STRUCT*)malloc(sizeof(HXL_STRUCT));
	// check for failure
	if (!out) return NULL;
	// preinitialize array so it can be resized/ destroy won't fail
	out->data = NULL;
	// initialize as empty
	out->len = 0;

	return out;
}

// PUSH
HXL_FUNC_SIG(bool, push, HXL_STRUCT* list, HXL_TYPE item)
{
	// calculate current isze of buffer in bytes
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

// POP
HXL_FUNC_SIG(bool, pop, HXL_STRUCT* list, HXL_TYPE item)
{
	if (list->len == 0) return false;
	HXL_TYPE *tmp = realloc(list->data, (list->len - 1)	* sizeof(HXL_TYPE));
	if (!tmp) return false;
	list->data = tmp;
	list->len -= 1;
	return true;
}

// CLEAR
HXL_FUNC_SIG(void, clear, HXL_STRUCT *list)
{
	// reset length
	list->len = 0;
	// free data
	free(list->data);
	// attempt to reset buffer
	list->data = NULL;
}

// INSERT
HXL_FUNC_SIG(bool, insert, HXL_STRUCT *list, HXL_TYPE item, size_t pos)
{
	// bounds check
	if (pos > list->len) return false;

	// allocate new buffer
	HXL_TYPE *tmp = (HXL_TYPE*)malloc((list->len + 1) * sizeof(HXL_STRUCT));
	if (!tmp) return false;

	// copy over data before new pos
	for (size_t i = 0; i < pos; ++i)
	{
		tmp[i] = list->data[i];
	}

	// insert new element
	tmp[pos] = item;

	// copying over latter portion of data
	for (size_t i = pos; i < list->len; ++i)
	{
		tmp[i + 1] = list->data[i];
	}

	// freeing old buffer
	free(list->data);

	// repointing list data
	list->data = tmp;

	// incrementing size
	list->len += 1;

	return true;
}

// PUSHF
HXL_FUNC_SIG(bool, pushf, HXL_STRUCT *list, HXL_TYPE item)
{
	// allocating new buffer
	HXL_TYPE *tmp = (HXL_TYPE*)malloc((list->len + 1) * sizeof(HXL_STRUCT));
	if (!tmp) return false;

	tmp[0] = item;

	for (size_t i = 0; i < list->len; ++i)
	{
		tmp[i + 1] = list->data[i];
	}

	list->len += 1;
	return true;
}

// ERASE
HXL_FUNC_SIG(bool, erase, HXL_STRUCT *list, size_t pos)
{
	// bounds check
	if (pos > list->len) return false;

	// copying over data passed pos
	size_t oi = pos;
	for (size_t i = pos + 1; i < list->len; ++i)
	{
		list->data[oi] = list->data[i];
	}

	// resizing buffer
	HXL_TYPE *tmp = realloc(list->data, (list->len - 1) * sizeof(HXL_TYPE));
	if (!tmp) return false;
	list->data = tmp;
	list->len -= 1;

	return true;
}

// POPF
HXL_FUNC_SIG(bool, popf, HXL_STRUCT *list)
{
	// bounds checking
	if (list->len == 0) return false;

	HXL_TYPE *tmp = malloc((list->len - 1) * sizeof(HXL_TYPE));
	if (!tmp) return false;

	// moving data forwards
	for (size_t i = 0; i < (list->len - 1); ++i)
	{
		tmp[i] = list->data[i + 1];
	}

	free(list->data);
	list->data = tmp;

	return true;

}

// RESIZE
HXL_FUNC_SIG(bool, resize, HXL_STRUCT *list, size_t new_size)
{
	HXL_TYPE *tmp = realloc(list->data, new_size * sizeof(HXL_TYPE));
	if (!tmp) return false;
	list->data = tmp;
	list->len = new_size;
	return true;	
}

// SWAP
HXL_FUNC_SIG(bool, swap, HXL_STRUCT *list, size_t a, size_t b)
{
	// bounds checking
	if (a >= list->len || b >= list->len) return false;

	// swapping elements
	HXL_TYPE t;
	t = list->data[a];
	list->data[a] = list->data[b];
	list->data[b] = t;

	return true;
}

// Other functions to implement
/*
	pushf
	popf
*/


#endif // HIRZEL_UTIL_LIST_I

// Preprocessor cleanup
#undef HXL_STRUCT
#undef HXL_TYPEDEF
#undef HXL_TYPE
#undef HXL_BASE
#undef HXL_NULL
#undef HIRZEL_UTIL_LIST_T
