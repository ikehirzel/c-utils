#define HIRZEL_UTIL_LIST_T int

// skip everything if no type declared
#ifdef HIRZEL_UTIL_LIST_T

#define CONCAT(a, b) a ## b
#define MAKE_STRUCT_NAME(type) CONCAT(hxlist_, type)
#define MAKE_TYPE_NAME(name) CONCAT(name, _t)
// defining names

#define HXLIST_TYPE HIRZEL_UTIL_LIST_T
#define HXLIST_SNAME MAKE_STRUCT_NAME(HIRZEL_UTIL_LIST_T)
#define HXLIST_TNAME MAKE_TYPE_NAME(HXLIST_STRUCT)
#define HXLIST_FUNC(postfix) hxlist_ ## HXLIST_TYPE ## postfix
// Declarations
#ifndef HIRZEL_UTIL_LIST_H
#define HIRZEL_UITL_LIST_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct hxlist
{
	unsigned short esize;
	void *data;
	size_t length;
} hxlist_t;

typedef struct HXLIST_SNAME
{
	HXLIST_TYPE *data;
	size_t len;
}
HXLIST_TNAME;

HXLIST_TNAME *HXLIST_FUNC(_create)()
{
	return NULL;
}

hxlist_t *_hxlist_create(unsigned short esize)
{
	hxlist_t *out = (hxlist_t*)malloc(sizeof(hxlist_t));
	if (!out) return NULL;
	out->data = malloc(1);
	if (!out->data)
	{
		free(out);
		return NULL;
	}
	out->esize = esize;
	out->length = 0;
	return out;
}

bool _hxlist_push(hxlist_t *list, const char* item)
{
	size_t size = list->length * list->esize;
	void *tmp = realloc(list->data, size + list->esize);
	if (!tmp) return false;
	memcpy((char*)tmp + size, item, list->esize);
	list->length += 1;
	list->data = tmp;
	return true;
}

#endif // HIRZEL_UITL_LIST_H

// Definitions
#ifdef HIRZEL_UTIL_LIST_I
#undef HIRZEL_UTIL_LIST_I

#endif // HIRZEL_UTIL_LIST_I

#endif // HIRZEL_UTIL_LIST_T
