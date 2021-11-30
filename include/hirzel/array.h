#ifndef HIRZEL_ARRAY_H
#define HIRZEL_ARRAY_H

#include <stdbool.h>
#include <stddef.h>

typedef struct HxArray HxArray;

extern HxArray *createHxArray(size_t element_size);
extern void destroyHxArray(HxArray *array);
extern bool reserveHxArray(HxArray *array, size_t new_capacity);
extern bool resizeHxArray(HxArray *array, size_t new_size);
extern bool incrementHxArray(HxArray *array);
extern bool pushHxArray(HxArray *array, const void *item);
extern bool pushFrontHxArray(HxArray *array, const void *item);
extern bool insertHxArray(HxArray *array, size_t pos, const void *item);
extern void popHxArray(HxArray *array);
extern void popFrontHxArray(HxArray *array);
extern void eraseHxArray(HxArray *array, size_t pos);
extern void setHxArray(HxArray *array, size_t pos, const void *item);
extern void swapHxArray(HxArray *array, void *tmp, size_t a, size_t b);
extern void clearHxArray(HxArray *array);

extern void getHxArray(const HxArray *array, void *out, size_t pos);
extern void *atHxArray(const HxArray *array, size_t i);
extern void *frontHxArray(const HxArray *array);
extern void *backHxArray(const HxArray *array);
extern bool isEmptyHxArray(const HxArray *array);
extern size_t lengthHxArray(const HxArray *array);

#endif

#if defined(HIRZEL_IMPLEMENT) && !defined(HIRZEL_UTIL_ARRAY_I)
#define HIRZEL_UTIL_ARRAY_I

#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct HxArray {
	void *data;
	size_t element_size;
	size_t capacity;
	size_t length;
};

HxArray *createHxArray(size_t element_size)
{
	assert(element_size > 0);

	HxArray *out = malloc(sizeof(HxArray));

	if (!out)
		return NULL;

	*out = (HxArray) {
		.data = NULL,
		.element_size = element_size,
		.length = 0,
		.capacity = 0
	};

	return out;
}

void destroyHxArray(HxArray *array)
{
	assert(array != NULL);

	free(array->data);
	free(array);
}

bool reserveHxArray(HxArray *array, size_t capacity)
{
	assert(array != NULL);

	if (capacity == 0)
	{
		free(array->data);
		array->data = NULL;
	}
	else
	{
		void *tmp = realloc(array->data, capacity * array->element_size);

		if (!tmp)
			return false;

		array->data = tmp;
	}

	return true;
}

bool resizeHxArray(HxArray *array, size_t new_length)
{
	assert(array != NULL);

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

bool incrementHxArray(HxArray *array)
{
	assert(array != NULL);

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
	assert(array != NULL);
	assert(item != NULL);

	if (!incrementHxArray(array))
		return false;

	char *dest = (char*)array->data + (array->length - 1) * array->element_size;

	memcpy(dest, item, array->element_size);	
	
	return true;
}

bool pushFrontHxArray(HxArray *array, const void *item)
{
	assert(array != NULL);
	assert(item != NULL);

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
	assert(array != NULL);
	assert(pos <= array->length);
	assert(item != NULL);

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
	assert(array != NULL);

	if (array->length == 0)
		return;
	
	array->length -= 1;
}

void popFrontHxArray(HxArray *array)
{
	assert(array != NULL);

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
	assert(array != NULL);
	assert(pos < array->length);

	for (size_t i = pos + 1; i < array->length; ++i)
	{
		char *dest = (char*)array->data + (i - 1) * array->element_size;
		const char *src = (char*)array->data + i * array->element_size;

		memcpy(dest, src, array->element_size);
	}

	array->length -= 1;
}

void getHxArray(const HxArray *array, void *out, size_t pos)
{
	assert(array != NULL);
	assert(out != NULL);
	assert(pos < array->length);

	void *src = (char*)array->data + pos * array->element_size;

	memcpy(out, src, array->element_size);
}

void *atHxArray(const HxArray *array, size_t pos)
{
	assert(array != NULL);
	assert(pos < array->length);

	return (char*)array->data + pos * array->element_size;
}

void setHxArray(HxArray *array, size_t pos, const void *item)
{
	assert(array != NULL);
	assert(pos < array->length);
	assert(item != NULL);

	void *dest = (char*)array->data + pos * array->element_size;
	memcpy(dest, item, array->element_size);
}

void swapHxArray(HxArray *array, void *tmp, size_t pos_a, size_t pos_b)
{
	assert(array != NULL);
	assert(tmp != NULL);
	assert(pos_a < array->length);
	assert(pos_b < array->length);

	void *a_ptr = (char*)array->data + pos_a * array->element_size;
	void *b_ptr = (char*)array->data + pos_b * array->element_size;

	memcpy(tmp, a_ptr, array->element_size);
	memcpy(a_ptr, b_ptr, array->element_size);
	memcpy(b_ptr, tmp, array->element_size);
}

void *frontHxArray(const HxArray *array)
{
	assert(array != NULL);
	assert(array->length > 0);

	return array->data;
}

void *backHxArray(const HxArray *array)
{
	assert(array != NULL);
	assert(array->length > 0);

	return (char*)array->data + (array->length - 1) * array->element_size;
}

void clearHxArray(HxArray *array)
{
	assert(array != NULL);

	array->length = 0;
}

bool isEmptyHxArray(const HxArray *array)
{
	assert(array != NULL);

	return array->length == 0;
}

size_t lengthHxArray(const HxArray *array)
{
	assert(array != NULL);

	return array->length;
}

#endif
