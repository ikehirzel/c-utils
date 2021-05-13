#define HIRZEL_UTIL_LIST_ELEM int
#define HIRZEL_UTIL_LIST_NAME	int_list

// Error if no type declared
#ifndef HIRZEL_UTIL_LIST_ELEM
#error An element type must be defined for list
#endif

#ifndef HIRZEL_UTIL_LIST_NAME
#error A struct name must be defined for list
#endif


// Util macros
#define CONCAT(a, b) a##b
#define LSTR(s) #s
#define STR(s) LSTR(s)

#define HXTYPEDEF_NAME(base) CONCAT(base, _t)
#define HXSTRUCT_NAME(base) struct base

#define HXSTRUCT 	HXSTRUCT_NAME(HIRZEL_UTIL_LIST_NAME)
#define HXTYPEDEF 	HXTYPEDEF_NAME(HIRZEL_UTIL_LIST_NAME)
#define HXELEM		HIRZEL_UTIL_LIST_ELEM

#define HXFUNC_BASE(base, postfix) CONCAT(base, postfix)
#define HXFUNC(name) HXFUNC_BASE(HIRZEL_UTIL_LIST_NAME, _##name)

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef HXSTRUCT
{
	size_t len; 
	HXELEM *data;
} HXTYPEDEF;


/**
 * @brief	Allocates instance of hxlist
 * 
 * The data is dynamically allocated and the list should be
 * freed with hxlist_destory
 * 
 * @return	pointer to list
 */
extern HXSTRUCT *HXFUNC(create)();

/**
 * @brief	Frees memory used by list instance
 * 
 * @param	list	hxlist pointer
 */
extern void HXFUNC(destroy)(HXSTRUCT *list);

/**
 * @brief	Pushes item to end of list.
 * 
 * @param	list	hxlist pointer
 * @param	item	element to add to list
 * 
 * @return	true on success, false on failure
 */
extern bool HXFUNC(push)(HXSTRUCT *list, HXELEM item);

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
extern HXELEM HXFUNC(at)(HXSTRUCT *list, size_t i);

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

extern bool HXFUNC(pop)(HXSTRUCT* list);
extern bool HXFUNC(insert)(HXSTRUCT *list, size_t pos, HXELEM item);
extern bool HXFUNC(pushf)(HXSTRUCT *list, HXELEM item);
extern bool HXFUNC(erase)(HXSTRUCT *list, size_t pos);
extern bool HXFUNC(popf)(HXSTRUCT *list);
extern bool HXFUNC(resize)(HXSTRUCT *list, size_t new_size);
extern bool HXFUNC(swap)(HXSTRUCT *list, size_t a, size_t b);
extern HXELEM *HXFUNC(atr)(HXSTRUCT *list, size_t i);
extern bool HXFUNC(put)(HXSTRUCT *list, size_t i, HXELEM val);
extern bool HXFUNC(putr)(HXSTRUCT *list, size_t i, HXELEM *ref);

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
#define hxlist_set(list, i, val) (list->data[i] = val)

#define hxlist_setr(list, i, ref) (list->data[i] = *ref)

/*******************************
 * OTHER MACROS TO DOCUMENT
 ******************************/

#define hxlist_front(list) (list->data[0])
#define hxlist_back(list) (list->data[list->len - 1])
#define hxlist_is_empty(list) (list->len == 0)
#define hxlist_size(list) (list->len)
#define hxlist_clear(list) { list->len = 0; free(list->data); list->data = NULL; }

// Definitions
#ifdef HIRZEL_UTIL_LIST_IMPL
#undef HIRZEL_UTIL_LIST_IMPL

// CREATE
HXSTRUCT *HXFUNC(create)()
{
	// allocate buffer
	HXSTRUCT *out = (HXSTRUCT*)malloc(sizeof(HXSTRUCT));
	// check for failure
	if (!out) return NULL;
	// preinitialize array so it can be resized/ destroy won't fail
	out->data = NULL;
	// initialize as empty
	out->len = 0;

	return out;
}

// DESTROY
void HXFUNC(destroy)(HXSTRUCT *list)
{
	free(list->data);
	free(list);
}

// PUSH
bool HXFUNC(push)(HXSTRUCT *list, HXELEM item)
{
	// calculate current isze of buffer in bytes
	size_t size = list->len * sizeof(HXELEM);
	// allocate size + 1
	HXELEM *tmp = (HXELEM*)realloc(list->data, size + sizeof(HXELEM));
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
bool HXFUNC(pop)(HXSTRUCT* list)
{
	if (list->len == 0) return false;
	HXELEM *tmp = realloc(list->data, (list->len - 1)	* sizeof(HXELEM));
	if (!tmp) return false;
	list->data = tmp;
	list->len -= 1;
	return true;
}

// INSERT
bool HXFUNC(insert)(HXSTRUCT *list, size_t pos, HXELEM item)
{
	// bounds check
	if (pos > list->len) return false;

	// allocate new buffer
	HXELEM *tmp = (HXELEM*)malloc((list->len + 1) * sizeof(HXSTRUCT));
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
bool HXFUNC(pushf)(HXSTRUCT *list, HXELEM item)
{
	// allocating new buffer
	HXELEM *tmp = (HXELEM*)malloc((list->len + 1) * sizeof(HXSTRUCT));
	if (!tmp) return false;

	tmp[0] = item;

	for (size_t i = 0; i < list->len; ++i)
	{
		tmp[i + 1] = list->data[i];
	}

	// freeing old buffer and repointing
	free(list->data);
	list->data = tmp;

	list->len += 1;
	return true;
}

// ERASE
bool HXFUNC(erase)(HXSTRUCT *list, size_t pos)
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
	HXELEM *tmp = realloc(list->data, (list->len - 1) * sizeof(HXELEM));
	if (!tmp) return false;
	list->data = tmp;
	list->len -= 1;

	return true;
}

// POPF
bool HXFUNC(popf)(HXSTRUCT *list)
{
	// bounds checking
	if (list->len == 0) return false;

	list->len -= 1;

	// moving data forwards
	for (size_t i = 0; i < list->len; ++i)
	{
		list->data[i] = list->data[i + 1];
	}

	HXELEM *tmp = realloc(list->data, list->len * sizeof(HXELEM));
	if (!tmp) return false;

	list->data = tmp;

	return true;

}

// RESIZE
bool HXFUNC(resize)(HXSTRUCT *list, size_t new_size)
{
	HXELEM *tmp = realloc(list->data, new_size * sizeof(HXELEM));
	if (!tmp) return false;
	list->data = tmp;
	list->len = new_size;
	return true;	
}

// SWAP
bool HXFUNC(swap)(HXSTRUCT *list, size_t a, size_t b)
{
	// bounds checking
	if (a >= list->len || b >= list->len) return false;

	// swapping elements
	HXELEM t;
	t = list->data[a];
	list->data[a] = list->data[b];
	list->data[b] = t;

	return true;
}

// AT
HXELEM HXFUNC(at)(HXSTRUCT *list, size_t i)
{
	if (i >= list->len)
	{
		HXELEM t = {0};
		return t;
	}
	return list->data[i];
}

// ATR
HXELEM *HXFUNC(atr)(HXSTRUCT *list, size_t i)
{
	return (i >= list->len) ? NULL : (list->data + i);
}

// PUT
bool HXFUNC(put)(HXSTRUCT *list, size_t i, HXELEM val)
{
	if (i >= list->len) return false;
	list->data[i] = val;
	return true;
}

// PUTR
bool HXFUNC(putr)(HXSTRUCT *list, size_t i, HXELEM *ref)
{
	if (i >= list->len) return false;
	list->data[i] = *ref;
	return true;
}

#endif // HIRZEL_UTIL_LIST_IMPL

// Preprocessor cleanup
#undef HXSTRUCT
#undef HXELEMDEF
#undef HXELEM
#undef HXL_BASE
#undef HXL_NULL
#undef HIRZEL_UTIL_LIST_T
