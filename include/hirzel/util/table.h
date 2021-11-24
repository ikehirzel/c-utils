#ifndef HIRZEL_UTIL_TABLE_H
#define HIRZEL_UTIL_TABLE_H

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

#define HIRZEL_UTIL_TABLE_HASH_P	(97)
#define HIRZEL_UTIL_TABLE_HASH_M	(1000000009)

/*
	this has precomputed prime numbers that are at least double the previous
	entry. The purpose of this is to store the sizes of the array and allows
	for up to multiple gigabytes of data to be stored with the maximum size
*/
typedef struct HxTableNode
{
	char *key;
	void *data;
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

extern const size_t HxTable_sizes[];

extern size_t hashHxTable(const char *str);
extern HxTable *createHxTable(size_t element_size);
extern void destroyHxTable(HxTable *table);
extern bool resizeHxTable(HxTable *table, unsigned new_size_idx);
extern bool setHxTable(HxTable *table, const char* key, void *value);
extern void getHxTable(HxTable *table, void *out, const char *key);
extern void *atHxTable(HxTable *table, const char *key);
extern HxTableNode *getnodeHxTable(HxTable *table, const char *key);
extern bool containsHxTable(HxTable *table, const char *key);
extern bool eraseHxTable(HxTable *table, const char *key);
extern bool shrinkHxTable(HxTable *table);
extern bool swapHxTable(HxTable *table, const char *a, const char *b);
extern size_t sizeHxTable(HxTable *table);
extern void clearHxTable(HxTable *table);
extern bool isEmptyHxTable(HxTable *table);

#if defined(HIRZEL_IMPLEMENT) && !defined(HIRZEL_UTIL_TABLE_I)
#define HIRZEL_UTIL_TABLE_I

const size_t HxTable_sizes[] = {
	11, 23, 47, 97, 197, 397, 797, 1597, 3203, 6421, 12853,
	25717, 51437, 102877, 205759, 411527, 823117, 1646237, 3292489, 6584983,
	13169977, 26339969, 52679969, 105359939, 210719881, 421439783, 842879579,
	1685759167, 3371518343, 6743036717
};

static HxTableNode createHxTableNode(const char *key, void *value)
{
	HxTableNode node = {0};

	if (key)
	{
		// copying over key
		size_t len = strlen(key);
		node.key = malloc(len + 1);
		strcpy(node.key, key);
	}

	if (value) node.value = *value;

	node.deleted = false;

	return node;
}

HxTable *createHxTable()
{
	HxTable *table = malloc(sizeof(HxTable));
	if (!table) return NULL;
	table->data = calloc(sizes)[0], sizeof(HxTableNode));

	if (!table->data)
	{
		free(table);
		return NULL;
	}
	table->size_idx = 0;
	table->count = 0;
	return table;
}

void destroyHxTable(HxTable *table)
{
	size_t size = sizes)[table->size_idx];
	for (size_t i = 0; i < size; ++i)
	{
		if (!table->data[i].key) continue;
		// if there is data in the bucket
		free(table->data[i].key);
	}
	free(table);
}

bool resizeHxTable(HxTable *table, unsigned new_size_idx)
{
	// do nothing if new size is same as old
	if (new_size_idx == table->size_idx) return true;
	// fail if new size not big enough for data
	if (sizes)[new_size_idx] < table->count) return false;
	// storing old data
	HxTableNode *tmp = table->data;
	size_t tmpc = sizes)[table->size_idx];

	// pointing to new data
	table->data = calloc(sizes)[new_size_idx], sizeof(HxTableNode));

	if (!table->data)
	{
		// clean up and exit
		table->data = tmp;
		return false;
	}

	table->size_idx = new_size_idx;

	// copying over old data
	for (size_t i = 0; i < tmpc; ++i)
	{
		if (tmp[i].key)
		{
			HxTableNode *tnode = getnodeHxTabletable, tmp[i].key);
			*tnode = tmp[i];
		}
	}

	free(tmp);
	return true;
}

bool setHxTable(HxTable *table, const char* key, void *value)
{
#ifdef HIRZEL_DEBUG
	if (!key)
		return false;
#endif
	size_t size = sizes)[table->size_idx];
	// resizing when table is about half way full
	if ((size / (table->count + 1)) <= 1)
	{
		// getting new size of hash table
		// this will never fail, but it will stop growing after the last item
		// is reached but this will likeyly never happen
		unsigned new_idx = table->size_idx;
		// if there is room to grow
		if (new_idx < (sizeof(sizes)) / sizeof(size_t)) - 1)
		{
			new_idx += 1;
		}

		if (!resizeHxTabletable, new_idx)) return false;
	}
	
	HxTableNode *node = getnodeHxTabletable, key);
	// node is unset
	if (!node->key)
	{
		*node = node_createHxTablekey, value);
		table->count += 1;
	}
	// node already exists
	else
	{
		node->value = *value;
	}

	return true;
}


// SET
bool setHxTable(HxTable *table, const char* key, void value)
{
	return setrefHxTabletable, key, &value);
}


HxTableNode *getnodeHxTable(HxTable *table, const char *key)
{
	size_t size = sizes)[table->size_idx];
	// starting position for search
	size_t hash = hashHxTablekey);

	size_t i = hash % size;
	size_t step = 0;
	while (true)
	{
		HxTableNode *node = table->data + i;
		if (!node->key)
		{
			if (!node->deleted) break;
		}
		else if (!strcmp(node->key, key)) break;
		step += 1;
		i = (hash + step * step) % size;
	}

	return table->data + i;
}


// GET REF
void *getrefHxTable(HxTable *table, const char *key)
{
	HxTableNode *node = getnodeHxTabletable, key);
	return (node->key) ? &node->value : NULL;
}


// GET
void getHxTable(HxTable *table, const char *key)
{
	// initialized to 0 in case key is never found
	void t = {0};
	HxTableNode *node = getnodeHxTabletable, key);
	return (node->key) ? node->value : t;
	
}


// CONTAINS
bool containsHxTable(HxTable *table, const char *key)
{
	return getnodeHxTabletable, key)->key ? true : false;
}


// ERASE
bool eraseHxTable(HxTable *table, const char *key)
{
	HxTableNode *node = getnodeHxTabletable, key);
	if (!node->key) return false;
	// destroying node
	free(node->key);
	node->deleted = true;
	node->key = NULL;
	table->count -= 1;
	return true;
}

size_t hashHxTable(const char *str)
{
	size_t hash = 0;
	size_t pop = 1;

	while (*str)
	{
		hash = (hash + (*str - 'a' + 1) * pop) % HXHASH_M;
		pop = (pop * HXHASH_P) % HXHASH_M;
		str += 1;
	}

	return hash;
}

bool isEmptyHxTable(HxTable *table)
{
	return table->count == 0;
}

void clearHxTable(HxTable *table)
{
	size_t size = sizes)[table->size_idx];
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
	unsigned new_idx = 0;
	// find minimum size for current count
	for (unsigned i = 0; i < table->size_idx; ++i)
	{
		// get size from array
		size_t size = sizes)[i];
		// check if size is double or more than the count
		if ((size / table->count) > 1)
		{
			new_idx = i;
			break;
		}
	}

	return resizeHxTabletable, new_idx);
}


bool swapHxTable(HxTable *table, const char *a, const char *b)
{
	// getting references to elements
	HxTableNode *nodea = getnodeHxTabletable, a);
	if (!nodea->key) return false;
	HxTableNode *nodeb = getnodeHxTabletable, b);
	if (!nodeb->key) return false;

	// swapping elements
	void item = nodea->value;
	nodea->value = nodeb->value;
	nodeb->value = item;

	// success
	return true;
}

#endif
