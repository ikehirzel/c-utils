#ifndef HIRZEL_UTIL_ARRAY_H
#define HIRZEL_UTIL_ARRAY_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct HxArray
{
	void *data;
	size_t element_size;
	size_t capacity;
	size_t length;
} 
HxArray;

extern HxArray *createHxArray(size_t element_size, size_t capacity);
extern void destroyHxArray(HxArray *array);
extern bool incrementHxArray(HxArray *array);
extern bool resizeHxArray(HxArray *array, size_t new_size);
extern bool pushHxArray(HxArray *array, const void *item);
extern bool pushFrontHxArray(HxArray *array, const void *item);
extern bool insertHxArray(HxArray *array, size_t pos, const void *item);
extern void popHxArray(HxArray *array);
extern void popFrontHxArray(HxArray *array);
extern void eraseHxArray(HxArray *array, size_t pos);
extern void setHxArray(HxArray *array, size_t pos, const void *item);
extern void getHxArray(HxArray *array, void *out, size_t pos);
extern void *atHxArray(HxArray *array, size_t i);
extern void *frontHxArray(HxArray *array);
extern void *backHxArray(HxArray *array);
extern void swapHxArray(HxArray *array, void *tmp, size_t a, size_t b);
extern void clearHxArray(HxArray *array);
extern bool isEmptyHxArray(HxArray *array);

#endif

#if defined(HIRZEL_IMPLEMENT) && !defined(HIRZEL_UTIL_ARRAY_I)
#define HIRZEL_UTIL_ARRAY_I

#ifdef HIRZEL_DEBUG

#include <stdio.h>
#include <stdarg.h>

void errorHxArray(const char *func_name, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	fprintf(stderr, "%s: ", func_name);
	vfprintf(stderr, fmt, args);
	putchar('\n');
	abort();
}

void accessErrorHxArray(const char *func_name, HxArray *array, size_t pos)
{
	errorHxArray(func_name, "attempted to access (%zu) byte element at position (%zu) but length was (%zu)",
		array->element_size, pos, array->length);
}

void nullArrayErrorHxArray(const char *func_name)
{
	errorHxArray(func_name, "attempted to access array at NULL");
}

void nullSourceErrorHxArray(const char *func_name, HxArray *array)
{
	errorHxArray(func_name, "attempted to read (%zu) byte element from NULL",
		array->element_size);
}

void nullDestErrorHxArray(const char *func_name, HxArray *array)
{
	errorHxArray(func_name, "attempted to write (%zu) byte element to NULL",
		array->element_size);
}

#endif

HxArray *createHxArray(size_t element_size, size_t capacity)
{

	HxArray *out = malloc(sizeof(HxArray));
	void *data = capacity > 0
		? malloc(element_size * capacity)
		: NULL;

	if (!out || (capacity > 0 && !data))
	{
		free(out);
		free(data);
	}
	else 
	{
		out->data = data;
		out->element_size = element_size;
		out->length = 0;
		out->capacity = capacity;
	}

	return out;
}

void destroyHxArray(HxArray *array)
{
#ifdef HIRZEL_DEBUG
	if (!array)
		nullArrayErrorHxArray(__func__);
#endif

	free(array->data);
	free(array);
}

bool incrementHxArray(HxArray *array)
{
#ifdef HIRZEL_DEBUG
	if (!array)
		nullArrayErrorHxArray(__func__);
#endif

	if (array->length == array->capacity)
	{
		size_t new_size = (array->length + 1) * array->element_size;
		void *new_buffer = realloc(array->data, new_size);

		if (!new_buffer)
			return false;

		array->data = new_buffer;
		array->capacity += 1;
	}

	array->length += 1;

	return true;
}

bool pushHxArray(HxArray *array, const void *item)
{
#ifdef HIRZEL_DEBUG
	if (!array)
		nullArrayErrorHxArray(__func__);
#endif

	if (!incrementHxArray(array))
		return false;

	char *dest = (char*)array->data + (array->length - 1) * array->element_size;

	memcpy(dest, item, array->element_size);	
	
	return true;
}

bool pushFrontHxArray(HxArray *array, const void *item)
{
#ifdef HIRZEL_DEBUG
	if (!array)
		nullArrayErrorHxArray(__func__);
#endif

	if (!incrementHxArray(array))
		return false;

	for (size_t i = array->length - 1; i >= 1; --i)
	{
		char *dest = (char*)array->data + i * array->element_size;
		const char *src = (char*)array->data + (i - 1) * array->element_size;

		memcpy(dest, src, array->element_size);
	}

	memcpy(array->data, item, array->element_size);

	return true;
}

bool insertHxArray(HxArray *array, size_t pos, const void *item)
{
#ifdef HIRZEL_DEBUG
	if (!array)
		nullArrayErrorHxArray(__func__);
	if (!item)
		nullSourceErrorHxArray(__func__, array);
	if (pos > array->length)
		accessErrorHxArray(__func__, array, pos);
#endif

	if (!incrementHxArray(array))
		return false;

	for (size_t i = array->length; i > pos; --i)
	{
		char *dest = (char*)array->data + (i - 1) * array->element_size;
		const char *src = (char*)array->data + (i - 2) * array->element_size;

		memcpy(dest, src, array->element_size);
	}
	
	char *dest = (char*)array->data + pos * array->element_size;

	memcpy(dest, item, array->element_size);

	return true;
}

void popHxArray(HxArray *array)
{
#ifdef HIRZEL_DEBUG
	if (!array)
		nullArrayErrorHxArray(__func__);
#endif

	if (array->length == 0)
		return;
	
	array->length -= 1;
}

void popFrontHxArray(HxArray *array)
{
#ifdef HIRZEL_DEBUG
	if (!array)
		nullArrayErrorHxArray(__func__);
#endif

	if (array->length == 0)
		return;

	array->length -= 1;

	const size_t element_size = array->element_size;
	const char *end = (char*)array->data + array->length * element_size;

	for (char *dest = array->data; dest < end; dest += element_size)
	{
		memcpy(dest, dest + element_size, element_size);
	}
}

void eraseHxArray(HxArray *array, size_t pos)
{	
#ifdef HIRZEL_DEBUG
	if (!array)
		nullArrayErrorHxArray(__func__);
	if (pos >= array->length)
		accessErrorHxArray(__func__, array, pos);
#endif

	for (size_t i = pos + 1; i < array->length; ++i)
	{
		char *dest = (char*)array->data + (i - 1) * array->element_size;
		const char *src = (char*)array->data + i * array->element_size;

		memcpy(dest, src, array->element_size);
	}

	array->length -= 1;
}

bool resizeHxArray(HxArray *array, size_t new_length)
{
#ifdef HIRZEL_DEBUG
	if (!array)
		nullArrayErrorHxArray(__func__);
#endif

	if (array->length == new_length)
		return true;

	if (new_length < array->length)
	{
		array->length = new_length;
	}
	else
	{
		size_t new_size = new_length * array->element_size;		
		void *new_data = realloc(array->data, new_size);

		if (!new_data)
			return false;

		array->data = new_data;
		array->length = new_length;
		array->capacity = new_length;
	}

	return true;	
}

void getHxArray(HxArray *array, void *out, size_t pos)
{
#ifdef HIRZEL_DEBUG
	if (!array)
		nullArrayErrorHxArray(__func__);
	if (!out)
		nullDestErrorHxArray(__func__, array);		
	if (pos >= array->length)
		accessErrorHxArray(__func__, array, pos);
#endif

	void *src = (char*)array->data + pos * array->element_size;

	memcpy(out, src, array->element_size);
}

void *atHxArray(HxArray *array, size_t pos)
{
#ifdef HIRZEL_DEBUG
	if (!array)
		nullArrayErrorHxArray(__func__);
	if (pos >= array->length)
		accessErrorHxArray(__func__, array, pos);
#endif

	return (char*)array->data + pos * array->element_size;
}

void setHxArray(HxArray *array, size_t pos, const void *item)
{
#ifdef HIRZEL_DEBUG
	if (!array)
		nullArrayErrorHxArray(__func__);
	if (!item)
		nullSourceErrorHxArray(__func__, array);
	if (pos >= array->length)
		accessErrorHxArray(__func__, array, pos);
#endif

	void *dest = (char*)array->data + pos * array->element_size;
	memcpy(dest, item, array->element_size);
}

void swapHxArray(HxArray *array, void *tmp, size_t a, size_t b)
{
#ifdef HIRZEL_DEBUG
	if (!array)
		nullArrayErrorHxArray(__func__);
	if (!tmp)
		nullDestErrorHxArray(__func__, array);
	if (a >= array->length)
		accessErrorHxArray(__func__, array, a);
	if (b >= array->length)
		accessErrorHxArray(__func__, array, b);
#endif
	void *a_ptr = (char*)array->data + a * array->element_size;
	void *b_ptr = (char*)array->data + b * array->element_size;

	memcpy(tmp, a_ptr, array->element_size);
	memcpy(a_ptr, b_ptr, array->element_size);
	memcpy(b_ptr, tmp, array->element_size);
}

void *frontHxArray(HxArray *array)
{
#ifdef HIRZEL_DEBUG
	if (!array)
		nullArrayErrorHxArray(__func__);
	if (array->length == 0)
		accessErrorHxArray(__func__, array, 0);
#endif

	return array->data;
}

void *backHxArray(HxArray *array)
{
#ifdef HIRZEL_DEBUG
	if (!array)
		nullArrayErrorHxArray(__func__);
	if (array->length == 0)
		accessErrorHxArray(__func__, array, array->length - 1);
#endif

	return (char*)array->data + (array->length - 1) * array->element_size;
}

void clearHxArray(HxArray *array)
{
#ifdef HIRZEL_DEBUG
	if (!array)
		nullArrayErrorHxArray(__func__);
#endif

	array->length = 0;
}

bool isEmptyHxArray(HxArray *array)
{
#ifdef HIRZEL_DEBUG
	if (!array)
		nullArrayErrorHxArray(__func__);
#endif

	return array->length == 0;
}

#endif
