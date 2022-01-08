#ifndef HIRZEL_TABLE_H
#define HIRZEL_TABLE_H

#include <stddef.h>
#include <stdbool.h>

typedef struct HxTable HxTable;

extern size_t hxtable_hash_string(const char *string);

#define hxtable_create_of(type) hxtable_create(sizeof(type))
extern HxTable *hxtable_create(size_t element_size);
extern void hxtable_destroy(HxTable *table);
extern bool hxtable_resize(HxTable *table, size_t new_size_index);
extern bool hxtable_reserve(HxTable *table, size_t min_count);
extern bool hxtable_shrink(HxTable *table);
extern bool hxtable_set(HxTable *table, const char* key, const void *value);
extern void hxtable_erase(HxTable *table, const char *key);
extern void hxtable_clear(HxTable *table);
extern bool hxtable_swap(HxTable *table, void *tmp, const char *a, const char *b);

extern bool hxtable_get(const HxTable *table, void *out, const char *key);
extern void *hxtable_at(const HxTable *table, const char *key);
extern bool hxtable_contains(const HxTable *table, const char *key);
extern size_t hxtable_size(const HxTable *table);
extern bool hxtable_is_empty(const HxTable *table);

#endif

#if defined(HIRZEL_IMPLEMENT) && !defined(HIRZEL_UTIL_TABLE_I)
#define HIRZEL_UTIL_TABLE_I

#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct HxTableNode
{
	char *key;
	void *value;
	bool deleted;
};

typedef size_t (*HxHashFunction)(const char *key);

struct HxTable
{
	struct HxTableNode *data;
	HxHashFunction hash_function;
	size_t size_index;
	size_t count;
	size_t element_size;
};

static const size_t hxtable_sizes[] = {
	11, 23, 47, 97, 197, 397, 797, 1597, 3203, 6421, 12853,
	25717, 51437, 102877, 205759, 411527, 823117, 1646237, 3292489, 6584983,
	13169977, 26339969, 52679969, 105359939, 210719881, 421439783, 842879579,
	1685759167, 3371518343, 6743036717
};
static const size_t HxTable_size_count = sizeof(hxtable_sizes) / sizeof(*hxtable_sizes);

static size_t hxtable_get_min_size_index(size_t count)
{
	size_t size_index = 0;

	while (size_index < HxTable_size_count)
	{
		if (hxtable_sizes[size_index] >= count * 2)
			break;

		++size_index;
	}

	return size_index;
}

static bool hxtable_init_node(struct HxTableNode *out, const HxTable *table, const char *key, const void *value)
{
	assert(table != NULL);
	assert(out != NULL);
	assert(key != NULL);
	assert(value != NULL);

	size_t key_len = strlen(key);
	void *buffer = malloc(table->element_size + key_len + 1);

	if (!buffer)
		return false;

	*out = (struct HxTableNode) {
		.key = (char*)buffer + table->element_size,
		.value = buffer,
		.deleted = false
	};

	strcpy(out->key, key);
	memcpy(out->value, value, table->element_size);

	return true;
}

static void hxtable_delete_node(struct HxTableNode *node)
{
	assert(node != NULL);

	free(node->value);

	node->key = NULL;
	node->value = NULL;
	node->deleted = true;
}

static void hxtable_clear_node(struct HxTableNode *node)
{
	assert(node != NULL);

	free(node->value);

	node->key = NULL;
	node->value = NULL;
	node->deleted = false;
}

struct HxTableNode *hxtable_find_node(const HxTable *table, const char *key)
{
	assert(table != NULL);
	assert(key != NULL);

	size_t size = hxtable_sizes[table->size_index];
	size_t hash = table->hash_function(key);

	size_t i = hash % size;
	size_t step = 0;

	while (true)
	{
		struct HxTableNode *node = table->data + i;

		if (!node->key && !node->deleted)	
			break;		
		else if (!strcmp(node->key, key))
			break;
		step += 1;
		i = (hash + step * step) % size;
	}

	struct HxTableNode *out = table->data + i;

	return out;
}

HxTable *hxtable_create(size_t element_size)
{
	assert(element_size > 0);

	HxTable *table = malloc(sizeof(HxTable));
	void *data = calloc(hxtable_sizes[0], sizeof(struct HxTableNode));

	if (!table || !data)
	{
		free(table);
		free(data);

		return NULL;
	}

	*table = (HxTable) {
		.data = data,
		.hash_function = hxtable_hash_string,
		.size_index = 0,
		.count = 0,
		.element_size = element_size
	};

	return table;
}

void hxtable_destroy(HxTable *table)
{
	assert(table != NULL);

	size_t size = hxtable_sizes[table->size_index];

	for (size_t i = 0; i < size; ++i)
	{
		void *value = table->data[i].value;

		if (value)
			free(value);		
	}

	free(table);
}

bool hxtable_resize(HxTable *table, size_t new_size_index)
{
	assert(table != NULL);
	assert(new_size_index < HxTable_size_count);

	if (new_size_index == table->size_index)
		return true;

	if (hxtable_sizes[new_size_index] < table->count)
		return false;

	void *new_data = calloc(hxtable_sizes[new_size_index], sizeof(struct HxTableNode));

	if (!new_data)
		return false;

	struct HxTableNode *old_data = table->data;
	size_t old_size = hxtable_sizes[table->size_index];

	table->data = new_data;
	table->size_index = new_size_index;

	for (size_t i = 0; i < old_size; ++i)
	{
		if (old_data[i].key)
		{
			struct HxTableNode *tnode = hxtable_find_node(table, old_data[i].key);
			*tnode = old_data[i];
		}
	}

	free(old_data);

	return true;
}

bool hxtable_reserve(HxTable *table, size_t min_count)
{
	assert(table != NULL);
	
	size_t size_index = hxtable_get_min_size_index(min_count);
	bool is_resized = hxtable_resize(table, size_index);

	return is_resized;
}

bool hxtable_set(HxTable *table, const char* key, const void *value)
{
	assert(table != NULL);
	assert(key != NULL);
	assert(value != NULL);

	size_t size = hxtable_sizes[table->size_index];
	bool is_table_half_full = (size / (table->count + 1)) <= 1;

	if (is_table_half_full)
	{
		size_t new_size_index = table->size_index;
		
		if (new_size_index < HxTable_size_count - 1)
			new_size_index += 1;

		if (!hxtable_resize(table, new_size_index))
			return false;
	}
	
	struct HxTableNode *node = hxtable_find_node(table, key);
	
	if (!node->key)
	{
		if (!hxtable_init_node(node, table, key, value))
			return false;

		table->count += 1;
	}
	else
	{
		memcpy(node->value, value, table->element_size);
	}

	return true;
}

bool hxtable_get(const HxTable *table, void *out, const char *key)
{
	assert(table != NULL);
	assert(out != NULL);
	assert(key != NULL);

	struct HxTableNode *node = hxtable_find_node(table, key);

	if (!node->key)
		return false;
	
	memcpy(out, node->value, table->element_size);

	return true;
}

void *hxtable_at(const HxTable *table, const char *key)
{
	assert(table != NULL);
	assert(key != NULL);

	struct HxTableNode *node = hxtable_find_node(table, key);
	
	void *out = node != NULL
		? node->value
		: NULL;

	return out;
}

bool hxtable_contains(const HxTable *table, const char *key)
{
	assert(table != NULL);
	assert(key != NULL);

	struct HxTableNode *node = hxtable_find_node(table, key);
	bool contains_key = node->key ? true : false;

	return contains_key;
}

void hxtable_erase(HxTable *table, const char *key)
{
	assert(table != NULL);
	assert(key != NULL);

	struct HxTableNode *node = hxtable_find_node(table, key);

	if (!node->key)
		return;

	hxtable_delete_node(node);
	table->count -= 1;
}

size_t hxtable_hash_string(const char *string)
{
	assert(string != NULL);

	const size_t p = 97;
	const size_t m = 1000000009;
	size_t hash = 0;
	size_t pop = 1;

	while (*string)
	{
		hash = (hash + (*string - 'a' + 1) * pop) % m;
		pop = (pop * p) % m;
		string += 1;
	}

	return hash;
}

bool hxtable_is_empty(const HxTable *table)
{
	assert(table != NULL);

	return table->count == 0;
}

void hxtable_clear(HxTable *table)
{
	assert(table != NULL);
	
	size_t size = hxtable_sizes[table->size_index];

	for (size_t i = 0; i < size; ++i)
	{
		struct HxTableNode *node = table->data + i;

		hxtable_clear_node(node);
	}

	table->count = 0;
}

size_t hxtable_size(const HxTable *table)
{
	assert(table != NULL);

	return hxtable_sizes[table->size_index];
}

bool hxtable_shrink(HxTable *table)
{
	assert(table != NULL);

	size_t new_size_index = hxtable_get_min_size_index(table->count);

	if (new_size_index >= table->size_index)
		return true;

	bool is_resized = hxtable_resize(table, new_size_index);

	return is_resized;
}

bool hxtable_swap(HxTable *table, void *tmp, const char *key_a, const char *key_b)
{
	assert(table != NULL);
	assert(tmp != NULL);
	assert(key_a != NULL);
	assert(key_b != NULL);

	struct HxTableNode *node_a = hxtable_find_node(table, key_a);

	if (!node_a->key)
		return false;

	struct HxTableNode *node_b = hxtable_find_node(table, key_b);

	if (!node_b->key)
		return false;

	memcpy(tmp, node_a->value, table->element_size);
	memcpy(node_a->value, node_b->value, table->element_size);
	memcpy(node_b->value, tmp, table->element_size);

	return true;
}

#endif
