#ifndef HIRZEL_TABLE_H
#define HIRZEL_TABLE_H

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define HIRZEL_TABLE_DECLARE(TYPE, NAME)\
\
typedef struct __##NAME##Node\
{\
	char *key;\
	TYPE value;\
	bool is_deleted;\
} NAME##Node;\
\
typedef struct __##NAME\
{\
	NAME##Node *data;\
	size_t(*hash_function)(const char*);\
	size_t size_index;\
	size_t count;\
} NAME;\
\
bool NAME##_init(NAME *table);\
void NAME##_free(NAME *table);\
bool NAME##_resize(NAME *table, size_t new_size_index);\
bool NAME##_reserve(NAME *table, size_t min_count);\
bool NAME##_shrink(NAME *table);\
bool NAME##_set(NAME *table, const char* key, TYPE value);\
bool NAME##_set_ptr(NAME *table, const char* key, const TYPE *value);\
void NAME##_erase(NAME *table, const char *key);\
void NAME##_clear(NAME *table);\
bool NAME##_swap(NAME *table, const char *a, const char *b);\
bool NAME##_get(const NAME *table, TYPE *out, const char *key);\
TYPE *NAME##_get_ptr(const NAME *table, const char *key);\
bool NAME##_contains(const NAME *table, const char *key);\
size_t NAME##_size(const NAME *table);\
bool NAME##_is_empty(const NAME *table);\
size_t NAME##_hash_string(const char *key);


#define HIRZEL_TABLE_DEFINE(TYPE, NAME)\
\
static const size_t NAME##_sizes[] = {\
	11, 23, 47, 97, 197, 397, 797, 1597, 3203, 6421, 12853,\
	25717, 51437, 102877, 205759, 411527, 823117, 1646237, 3292489, 6584983,\
	13169977, 26339969, 52679969, 105359939, 210719881, 421439783, 842879579,\
	1685759167, 3371518343, 6743036717\
};\
\
static const size_t NAME##_size_count = sizeof(NAME##_sizes) / sizeof(*NAME##_sizes);\
\
static size_t NAME##_get_min_size_index(size_t count)\
{\
	size_t size_index = 0;\
	while (size_index < NAME##_size_count)\
	{\
		if (NAME##_sizes[size_index] >= count * 2)\
			break;\
\
		size_index += 1;\
	}\
	return size_index;\
}\
\
static bool NAME##_init_node(NAME##Node *out, const NAME *table, const char *key, const TYPE *value)\
{\
	assert(table != NULL);\
	assert(out != NULL);\
	assert(key != NULL);\
	assert(value != NULL);\
\
	size_t key_len = strlen(key) + 1;\
	char *key_buffer = malloc(key_len);\
\
	if (!key_buffer)\
		return false;\
\
	*out = (NAME##Node) { key_buffer, *value, false };\
	strcpy(out->key, key);\
	out->value = *value;\
\
	return true;\
}\
\
static void NAME##_delete_node(NAME##Node *node)\
{\
	assert(node != NULL);\
\
	free(node->key);\
\
	node->key = NULL;\
	node->is_deleted = true;\
}\
\
static void NAME##_clear_node(NAME##Node *node)\
{\
	assert(node != NULL);\
\
	free(node->key);\
\
	node->key = NULL;\
	node->is_deleted = false;\
}\
\
static NAME##Node *NAME##_find_node(const NAME *table, const char *key)\
{\
	assert(table != NULL);\
	assert(key != NULL);\
\
	size_t size = NAME##_sizes[table->size_index];\
	size_t hash = table->hash_function(key);\
	size_t i = hash % size;\
	size_t step = 0;\
\
	while (true)\
	{\
		NAME##Node *node = table->data + i;\
\
		if (node->key == NULL && !node->is_deleted)\
			break;\
		else if (!strcmp(node->key, key))\
			break;\
\
		step += 1;\
		i = (hash + step * step) % size;\
	}\
\
	NAME##Node *out = table->data + i;\
\
	return out;\
}\
\
bool NAME##_init(NAME *table)\
{\
	assert(table != NULL);\
\
	NAME##Node *data = calloc(NAME##_sizes[0], sizeof(NAME##Node));\
\
	if (data == NULL)\
		return false;\
\
	*table = (NAME) { data, NAME##_hash_string, 0, 0 };\
	return true;\
}\
\
void NAME##_free(NAME *table)\
{\
	assert(table != NULL);\
\
	size_t size = NAME##_sizes[table->size_index];\
\
	for (size_t i = 0; i < size; ++i)\
		free(table->data[i].key);\
}\
\
bool NAME##_resize(NAME *table, size_t new_size_index)\
{\
	assert(table != NULL);\
	assert(new_size_index < NAME##_size_count);\
\
	if (new_size_index == table->size_index)\
		return true;\
	if (NAME##_sizes[new_size_index] < table->count)\
		return false;\
\
	NAME##Node *new_data = calloc(NAME##_sizes[new_size_index], sizeof(NAME##Node));\
\
	if (new_data == NULL)\
		return false;\
\
	NAME##Node *old_data = table->data;\
	size_t old_size = NAME##_sizes[table->size_index];\
\
	table->data = new_data;\
	table->size_index = new_size_index;\
\
	for (size_t i = 0; i < old_size; ++i)\
	{\
		if (old_data[i].key)\
		{\
			NAME##Node *tnode = NAME##_find_node(table, old_data[i].key);\
			*tnode = old_data[i];\
		}\
	}\
\
	free(old_data);\
\
	return true;\
}\
\
bool NAME##_reserve(NAME *table, size_t min_count)\
{\
	assert(table != NULL);\
	\
	size_t size_index = NAME##_get_min_size_index(min_count);\
	bool is_resized = NAME##_resize(table, size_index);\
\
	return is_resized;\
}\
\
bool NAME##_set_ptr(NAME *table, const char* key, const TYPE *value)\
{\
	assert(table != NULL);\
	assert(key != NULL);\
	assert(value != NULL);\
\
	size_t size = NAME##_sizes[table->size_index];\
	bool is_table_half_full = (size / (table->count + 1)) <= 1;\
\
	if (is_table_half_full)\
	{\
		size_t new_size_index = table->size_index;\
		\
		if (new_size_index < NAME##_size_count - 1)\
			new_size_index += 1;\
\
		if (!NAME##_resize(table, new_size_index))\
			return false;\
	}\
	\
	NAME##Node *node = NAME##_find_node(table, key);\
	\
	if (!node->key)\
	{\
		if (!NAME##_init_node(node, table, key, value))\
			return false;\
\
		table->count += 1;\
	}\
	else\
	{\
		node->value = *value;\
	}\
\
	return true;\
}\
bool NAME##_set(NAME *table, const char *key, TYPE value)\
{\
	return NAME##_set_ptr(table, key, &value);\
}\
\
bool NAME##_get(const NAME *table, TYPE *out, const char *key)\
{\
	assert(table != NULL);\
	assert(out != NULL);\
	assert(key != NULL);\
\
	NAME##Node *node = NAME##_find_node(table, key);\
\
	if (!node->key)\
		return false;\
\
	*out = node->value;\
\
	return true;\
}\
\
TYPE *NAME##_get_ptr(const NAME *table, const char *key)\
{\
	assert(table != NULL);\
	assert(key != NULL);\
\
	NAME##Node *node = NAME##_find_node(table, key);\
	\
	TYPE *out = node != NULL\
		? &node->value\
		: NULL;\
\
	return out;\
}\
\
bool NAME##_contains(const NAME *table, const char *key)\
{\
	assert(table != NULL);\
	assert(key != NULL);\
\
	NAME##Node *node = NAME##_find_node(table, key);\
	bool contains_key = node->key ? true : false;\
\
	return contains_key;\
}\
\
void NAME##_erase(NAME *table, const char *key)\
{\
	assert(table != NULL);\
	assert(key != NULL);\
\
	NAME##Node *node = NAME##_find_node(table, key);\
\
	if (!node->key)\
		return;\
\
	NAME##_delete_node(node);\
	table->count -= 1;\
}\
\
size_t NAME##_hash_string(const char *string)\
{\
	assert(string != NULL);\
\
	const size_t p = 97;\
	const size_t m = 1000000009;\
	size_t hash = 0;\
	size_t pop = 1;\
\
	while (*string)\
	{\
		hash = (hash + (*string - 'a' + 1) * pop) % m;\
		pop = (pop * p) % m;\
		string += 1;\
	}\
\
	return hash;\
}\
\
bool NAME##_is_empty(const NAME *table)\
{\
	assert(table != NULL);\
\
	return table->count == 0;\
}\
\
void NAME##_clear(NAME *table)\
{\
	assert(table != NULL);\
	\
	size_t size = NAME##_sizes[table->size_index];\
\
	for (size_t i = 0; i < size; ++i)\
	{\
		NAME##Node *node = table->data + i;\
\
		NAME##_clear_node(node);\
	}\
\
	table->count = 0;\
}\
\
size_t NAME##_size(const NAME *table)\
{\
	assert(table != NULL);\
\
	return NAME##_sizes[table->size_index];\
}\
\
bool NAME##_shrink(NAME *table)\
{\
	assert(table != NULL);\
\
	size_t new_size_index = NAME##_get_min_size_index(table->count);\
\
	if (new_size_index >= table->size_index)\
		return true;\
\
	bool is_resized = NAME##_resize(table, new_size_index);\
\
	return is_resized;\
}\
\
bool NAME##_swap(NAME *table, const char *key_a, const char *key_b)\
{\
	assert(table != NULL);\
	assert(key_a != NULL);\
	assert(key_b != NULL);\
\
	NAME##Node *node_a = NAME##_find_node(table, key_a);\
\
	if (!node_a->key)\
		return false;\
\
	NAME##Node *node_b = NAME##_find_node(table, key_b);\
\
	if (!node_b->key)\
		return false;\
\
	TYPE tmp = node_a->value;\
	node_a->value = node_b->value;\
	node_b->value = tmp;\
\
	return true;\
}\

#endif
