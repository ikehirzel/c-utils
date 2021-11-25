#ifndef HIRZEL_UTIL_TABLE_H
#define HIRZEL_UTIL_TABLE_H

#include <stddef.h>
#include <stdbool.h>

typedef struct HxTableNode
{
	char *key;
	void *value;
	bool deleted;
}
HxTableNode;

typedef struct HxTable
{
	HxTableNode *data;
	size_t size_index;
	size_t count;
	size_t element_size;
}
HxTable;

extern size_t hashHxTable(const char *str);
extern HxTable *createHxTable(size_t element_size);
extern void destroyHxTable(HxTable *table);
extern bool resizeHxTable(HxTable *table, size_t new_size_index);
extern bool shrinkHxTable(HxTable *table);
extern bool setHxTable(HxTable *table, const char* key, void *value);
extern void eraseHxTable(HxTable *table, const char *key);
extern bool getHxTable(const HxTable *table, void *out, const char *key);
extern void *atHxTable(const HxTable *table, const char *key);
extern HxTableNode *findNodeHxTable(const HxTable *table, const char *key);
extern bool containsHxTable(HxTable *table, const char *key);
extern size_t sizeHxTable(HxTable *table);
extern bool swapHxTable(HxTable *table, void *tmp, const char *a, const char *b);
extern void clearHxTable(HxTable *table);
extern bool isEmptyHxTable(const HxTable *table);

#endif

#if defined(HIRZEL_IMPLEMENT) && !defined(HIRZEL_UTIL_TABLE_I)
#define HIRZEL_UTIL_TABLE_I

#define HIRZEL_UTIL_TABLE_HASH_P	(97)
#define HIRZEL_UTIL_TABLE_HASH_M	(1000000009)

#include <stdlib.h>
#include <string.h>

#ifdef HIRZEL_DEBUG
#include <stdio.h>
#include <stdarg.h>
#endif

static const size_t HxTable_sizes[] = {
	11, 23, 47, 97, 197, 397, 797, 1597, 3203, 6421, 12853,
	25717, 51437, 102877, 205759, 411527, 823117, 1646237, 3292489, 6584983,
	13169977, 26339969, 52679969, 105359939, 210719881, 421439783, 842879579,
	1685759167, 3371518343, 6743036717
};
static const size_t HxTable_size_count = sizeof(HxTable_sizes) / sizeof(*HxTable_sizes);

void errorHxTable(const char *func_name, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	fprintf(stderr, "%s: ", func_name);
	vfprintf(stderr, fmt, args);
	putchar('\n');
	abort();
}

static void nullTableErrorHxTable(const char *func_name)
{
	errorHxTable(func_name, "attempted to access table with table = NULL");
}

static void nullNodeErrorHxTable(const char *func_name, const HxTable *table)
{
	errorHxTable(func_name, "attempted to access (%zu) byte element node with node = NULL",
		table->element_size);
}

static void nullKeyErrorHxTable(const char *func_name, const HxTable *table)
{
	errorHxTable(func_name, "attempted to access (%zu) byte element with key = NULL",
		table->element_size);
}

static void nullValueErrorHxTable(const char *func_name, const HxTable *table)
{
	errorHxTable(func_name, "attempted to write (%zu) byte element with value = NULL",
		table->element_size);
}

static bool initHxTableNode(const HxTable *table, HxTableNode *out, const char *key, const void *value)
{
#ifdef HIRZEL_DEBUG
	if (!table)
		nullTableErrorHxTable(__func__);
	if (!out)
		nullNodeErrorHxTable(__func__, table);
	if (!key)
		nullKeyErrorHxTable(__func__, table);
	if (!value)
		nullValueErrorHxTable(__func__, table);
#endif

	size_t key_len = strlen(key);
	void *buffer = malloc(table->element_size + key_len + 1);

	if (!buffer)
		return false;

	*out = (HxTableNode) {
		.key = (char*)buffer + table->element_size,
		.value = buffer,
		.deleted = false
	};

	strcpy(out->key, key);
	memcpy(out->value, value, table->element_size);

	return true;
}

static void deleteHxTableNode(HxTableNode *node)
{
#ifdef HIRZEL_DEBUG
	if (!node)
		errorHxTable(__func__, "attempted to delete node with node = NULL");
#endif

	free(node->value);

	node->key = NULL;
	node->value = NULL;
	node->deleted = true;
}

HxTable *createHxTable(size_t element_size)
{
#ifdef HIRZEL_DEBUG
	if (element_size == 0)
		errorHxTable(__func__, "attempted to create table with 0 byte elements");
#endif

	HxTable *table = malloc(sizeof(HxTable));
	void *data = calloc(HxTable_sizes[0], sizeof(HxTableNode));

	if (!table || !data)
	{
		free(table);
		free(data);

		return NULL;
	}

	*table = (HxTable) {
		.data = data,
		.size_index = 0,
		.count = 0,
		.element_size = element_size
	};

	return table;
}

void destroyHxTable(HxTable *table)
{
#ifdef HIRZEL_DEBUG
	if (!table)
		nullTableErrorHxTable(__func__);
#endif

	size_t size = HxTable_sizes[table->size_index];
	
	for (size_t i = 0; i < size; ++i)
	{
		void *value = table->data[i].value;

		if (value)
			free(value);		
	}

	free(table);
}

bool resizeHxTable(HxTable *table, size_t new_size_index)
{
#ifdef HIRZEL_DEBUG
	if (!table)
		nullTableErrorHxTable(__func__);
	if (new_size_index >= HxTable_size_count)
		errorHxTable(__func__, "attempted to resize table with out of bounds size index: %zu",
			new_size_index);
#endif

	if (new_size_index == table->size_index)
		return true;

	if (HxTable_sizes[new_size_index] < table->count)
		return false;

	void *new_data = calloc(HxTable_sizes[new_size_index], sizeof(HxTableNode));

	if (!new_data)
		return false;

	HxTableNode *old_data = table->data;
	size_t old_size = HxTable_sizes[table->size_index];

	table->data = new_data;
	table->size_index = new_size_index;

	for (size_t i = 0; i < old_size; ++i)
	{
		if (old_data[i].key)
		{
			HxTableNode *tnode = findNodeHxTable(table, old_data[i].key);
			*tnode = old_data[i];
		}
	}

	free(old_data);

	return true;
}

bool setHxTable(HxTable *table, const char* key, void *value)
{
#ifdef HIRZEL_DEBUG
	if (!table)
		nullTableErrorHxTable(__func__);
	if (!key)
		nullKeyErrorHxTable(__func__, table);
	if (!value)
		nullValueErrorHxTable(__func__, table);
#endif

	size_t size = HxTable_sizes[table->size_index];
	bool is_table_half_full = (size / (table->count + 1)) <= 1;

	if (is_table_half_full)
	{
		size_t new_size_index = table->size_index;
		
		if (new_size_index < HxTable_size_count - 1)
			new_size_index += 1;

		if (!resizeHxTable(table, new_size_index))
			return false;
	}
	
	HxTableNode *node = findNodeHxTable(table, key);
	
	if (!node->key)
	{
		if (!initHxTableNode(table, node, key, value))
			return false;

		table->count += 1;
	}
	else
	{
		memcpy(node->value, value, table->element_size);
	}

	return true;
}

HxTableNode *findNodeHxTable(const HxTable *table, const char *key)
{
	size_t size = HxTable_sizes[table->size_index];
	size_t hash = hashHxTable(key);

	size_t i = hash % size;
	size_t step = 0;

	while (true)
	{
		HxTableNode *node = table->data + i;

		if (!node->key && !node->deleted)	
			break;		
		else if (!strcmp(node->key, key))
			break;
		step += 1;
		i = (hash + step * step) % size;
	}

	HxTableNode *out = table->data + i;

	return out;
}

bool getHxTable(const HxTable *table, void *out, const char *key)
{
	HxTableNode *node = findNodeHxTable(table, key);

	if (!node->key)
		return false;
	
	memcpy(out, node->value, table->element_size);

	return true;
}

bool containsHxTable(HxTable *table, const char *key)
{
	HxTableNode *node = findNodeHxTable(table, key);
	bool contains_key = node->key ? true : false;

	return contains_key;
}

void eraseHxTable(HxTable *table, const char *key)
{
	HxTableNode *node = findNodeHxTable(table, key);

	if (!node->key)
		return;

	deleteHxTableNode(node);
	table->count -= 1;
}

size_t hashHxTable(const char *str)
{
	size_t hash = 0;
	size_t pop = 1;

	while (*str)
	{
		hash = (hash + (*str - 'a' + 1) * pop) % HIRZEL_UTIL_TABLE_HASH_M;
		pop = (pop * HIRZEL_UTIL_TABLE_HASH_P) % HIRZEL_UTIL_TABLE_HASH_M;
		str += 1;
	}

	return hash;
}

bool isEmptyHxTable(const HxTable *table)
{
	return table->count == 0;
}

void clearHxTable(HxTable *table)
{
	size_t size = HxTable_sizes[table->size_index];

	for (size_t i = 0; i < size; ++i)
	{
		table->data[i].key = NULL;
	}

	table->count = 0;
}

size_t sizeHxTable(HxTable *table)
{
	return HxTable_sizes[table->size_index];
}

bool shrinkHxTable(HxTable *table)
{
	size_t new_size_index = 0;
	
	for (size_t i = 0; i < table->size_index; ++i)
	{
		size_t new_size = HxTable_sizes[i];
		bool would_be_less_than_half_full = (new_size / table->count) > 1;

		if (would_be_less_than_half_full)
		{
			new_size_index = i;
			break;
		}
	}

	bool is_resized = resizeHxTable(table, new_size_index);

	return is_resized;
}


bool swapHxTable(HxTable *table, void *tmp, const char *a, const char *b)
{
	HxTableNode *node_a = findNodeHxTable(table, a);

	if (!node_a->key)
		return false;

	HxTableNode *node_b = findNodeHxTable(table, b);

	if (!node_b->key)
		return false;

	memcpy(tmp, node_a->value, table->element_size);
	memcpy(node_a->value, node_b->value, table->element_size);
	memcpy(node_b->value, tmp, table->element_size);

	return true;
}

#endif
