#ifndef HIRZEL_ARRAY_H
#define HIRZEL_ARRAY_H

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define HXARRAY_STRUCT(TYPE, NAME)\
typedef struct __##NAME\
{\
	TYPE *buffer;\
	size_t length;\
	size_t capacity;\
} NAME;

#define HXARRAY_DECLARE(TYPE, NAME)\
HXARRAY_STRUCT(TYPE, NAME)\
NAME NAME##_init();\
void NAME##_free(NAME *array);\
bool NAME##_reserve(NAME *array, size_t capacity);\
bool NAME##_resize(NAME *array, size_t length);\
bool NAME##_increment_length(NAME *array);\
bool NAME##_push(NAME *array, TYPE item);\
bool NAME##_push_ptr(NAME *array, const TYPE *item);\
bool NAME##_pushf(NAME *array, TYPE item);\
bool NAME##_pushf_ptr(NAME *array, const TYPE *item);\
bool NAME##_insert(NAME *array, size_t pos, const TYPE *item);\
bool NAME##_insert_ptr(NAME *array, size_t pos, const TYPE *item);\
void NAME##_pop(NAME *array);\
void NAME##_popf(NAME *array);\
void NAME##_erase(NAME *array, size_t pos);\
void NAME##_set(NAME *array, size_t pos, TYPE item);\
void NAME##_set_ptr(NAME *array, size_t pos, const TYPE *item);\
void NAME##_swap(NAME *array, size_t a, size_t b);\
TYPE NAME##_get(NAME *array, size_t i);\
TYPE *NAME##_get_ptr(NAME *array, size_t i);\
TYPE NAME##_front(NAME *array);\
TYPE *NAME##_front_ptr(NAME *array);\
TYPE NAME##_back(NAME *array);\
TYPE *NAME##_back_ptr(NAME *array);\
inline static void NAME##_clear(NAME *array) { assert(array != NULL); array->length = 0; }\
inline static bool NAME##_is_empty(NAME *array) { assert(array != NULL); return array->length != 0; }\
inline static size_t NAME##_length(NAME *array) { assert(array != NULL); return array->length; }\
inline static size_t NAME##_capacity(NAME *array) { assert(array != NULL); return array->capacity; }


#define HXARRAY_DEFINE(TYPE, NAME)\
NAME NAME##_init()\
{\
	return (NAME) { NULL, 0, 0 };\
}\
\
void NAME##_free(NAME *array)\
{\
	assert(array != NULL);\
	free(array->buffer);\
}\
\
bool NAME##_reserve(NAME *array, size_t capacity)\
{\
	assert(array != NULL);\
	if (capacity == 0)\
	{\
		free(array->buffer);\
		array->buffer = NULL;\
	}\
	else\
	{\
		if (capacity < array->length) array->length = capacity;\
		TYPE *tmp = realloc(array->buffer, capacity * sizeof(TYPE));\
		if (!tmp)\
			return false;\
		array->buffer = tmp;\
	}\
	return true;\
}\
\
bool NAME##_resize(NAME *array, size_t length)\
{\
	assert(array != NULL);\
	if (array->length == length)\
		return true;\
	if (length < array->length)\
	{\
		array->length = length;\
	}\
	else\
	{\
		TYPE *tmp = realloc(array->buffer, length * sizeof(TYPE));\
		if (!tmp) return false;\
		array->buffer = tmp;\
		array->length = length;\
		array->capacity = length;\
	}\
	return true;\
}\
\
bool NAME##_increment_size(NAME *array)\
{\
	assert(array != NULL);\
	if (array->length == array->capacity)\
	{\
		TYPE *tmp = realloc(array->buffer, (array->length + 1) * sizeof(TYPE));\
		if (!tmp) return false;\
		array->buffer = tmp;\
		array->capacity += 1;\
	}\
	array->length += 1;\
	return true;\
}\
\
bool NAME##_push_ptr(NAME *array, const TYPE *item)\
{\
	assert(array != NULL);\
	assert(item != NULL);\
	if (!NAME##_increment_size(array)) return false;\
	array->buffer[array->length - 1] = *item;\
	return true;\
}\
\
bool NAME##_push(NAME *array, TYPE item)\
{\
	return NAME##_push_ptr(array, &item);\
}\
\
bool NAME##_pushf_ptr(NAME *array, const TYPE *item)\
{\
	assert(array != NULL);\
	assert(item != NULL);\
	if (!NAME##_increment_size(array)) return false;\
	for (size_t i = array->length - 1; i >= 1; --i) array->buffer[i] = array->buffer[i - 1];\
	array->buffer[0] = *item;\
	return true;\
}\
\
bool NAME##_pushf(NAME *array, TYPE item)\
{\
	return NAME##_pushf_ptr(array, &item);\
}\
\
bool NAME##_insert(NAME *array, size_t pos, const TYPE *item)\
{\
	assert(array != NULL);\
	assert(pos <= array->length);\
	assert(item != NULL);\
	if (!NAME##_increment_size(array)) return false;\
	for (size_t i = array->length; i > pos; --i) array->buffer[i - 1] = array->buffer[i - 2];\
	array->buffer[pos] = *item;\
	return true;\
}\
\
void NAME##_pop(NAME *array)\
{\
	assert(array != NULL);\
	if (array->length > 0) array->length -= 1;\
}\
\
void NAME##_popf(NAME *array)\
{\
	assert(array != NULL);\
	if (array->length == 0) return;\
	array->length -= 1;\
	size_t end = array->length - 1;\
	for (size_t i = 0; i < end; ++i) array->buffer[i] = array->buffer[i + 1];\
}\
\
void NAME##_erase(NAME *array, size_t pos)\
{\
	assert(array != NULL);\
	assert(pos < array->length);\
	for (size_t i = pos + 1; i < array->length; ++i) array->buffer[i - 1] = array->buffer[i];\
	array->length -= 1;\
}\
\
void NAME##_set_ptr(NAME *array, size_t pos, const TYPE *item)\
{\
	assert(array != NULL);\
	assert(item != NULL);\
	assert(pos < array->length);\
	array->buffer[pos] = *item;\
}\
\
void NAME##_set(NAME *array, size_t pos, TYPE item)\
{\
	NAME##_set_ptr(array, pos, &item);\
}\
\
void NAME##_swap(NAME *array, size_t pos_a, size_t pos_b)\
{\
	assert(array != NULL);\
	assert(pos_a < array->length);\
	assert(pos_b < array->length);\
	TYPE tmp = array->buffer[pos_a];\
	array->buffer[pos_a] = array->buffer[pos_b];\
	array->buffer[pos_b] = tmp;\
}\
\
TYPE *NAME##_get_ptr(NAME *array, size_t i)\
{\
	assert(array != NULL);\
	assert(i < array->length);\
	return array->buffer + i;\
}\
\
TYPE NAME##_get(NAME *array, size_t i)\
{\
	return *NAME##_get_ptr(array, i);\
}\
\
TYPE *NAME##_front_ptr(NAME *array)\
{\
	assert(array != NULL);\
	assert(array->length > 0);\
	return array->buffer;\
}\
\
TYPE NAME##_front(NAME *array)\
{\
	return *NAME##_front_ptr(array);\
}\
\
TYPE *NAME##_back_ptr(NAME *array)\
{\
	assert(array != NULL);\
	assert(array->length > 0);\
	return array->buffer + (array->length - 1);\
}\
\
TYPE NAME##_back(NAME *array)\
{\
	return *NAME##_back_ptr(array);\
}

#endif
