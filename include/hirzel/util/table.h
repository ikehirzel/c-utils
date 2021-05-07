
#define HIRZEL_UTIL_TABLE_T int
// Error if no type declared
#ifndef HIRZEL_UTIL_TABLE_T
#error An element type must be defined for container
#endif

// Util macros
#define CONCAT(a, b) a##b
#define LSTR(s) #s
#define STR(s) LSTR(s)

// Type agnostic macros
#define HXT_BASE_NAME(type) 	CONCAT(hxtbl_, type)
#define HXT_ITEM_NAME(type)		CONCAT(hxtbl_item_, type)
#define HXT_STRUCT_NAME(base)	struct base
#define HXT_TYPEDEF_NAME(base)	CONCAT(base, _t)

// Convenient macro aliases
#define HXT_TYPE				HIRZEL_UTIL_TABLE_T
#define HXT_BASE				HXT_BASE_NAME(HXT_TYPE)
#define HXT_ITEM_BASE			HXT_ITEM_NAME(HXT_TYPE)
#define HXT_STRUCT				HXT_STRUCT_NAME(HXT_BASE)
#define HXT_ITEM				HXT_STRUCT_NAME(HXT_ITEM_BASE)
#define HXT_TYPEDEF				HXT_TYPEDEF_NAME(HXT_BASE)
#define HXT_ITEM_TYPEDEF		HXT_TYPEDEF_NAME(HXT_ITEM_BASE)

#define HXT_FUNC_BASE(base, postfix) CONCAT(base, postfix)
#define HXT_FUNC_NAME(name) HXT_FUNC_BASE(HXT_BASE, _##name)
#define HXT_FUNC_SIG(ret_type, func_name, ...)\
ret_type HXT_FUNC_NAME(func_name)(__VA_ARGS__)

// Declarations
#ifndef HIRZEL_UTIL_TABLE_H
#define HIRZEL_UTIL_TABLE_H

#include <stddef.h>
#include <stdio.h>

#define HXT_HASH_P		(97)
#define HXT_HASH_M		(1000000009)
#define HXT_INIT_SIZE	(10)

typedef HXT_ITEM
{
	char *key;
	HXT_TYPE value;
} HXT_ITEM_TYPEDEF;

typedef HXT_STRUCT
{
	size_t size;	// size of data
	size_t count;	// amount of elements
	HXT_ITEM *data;
} HXT_TYPEDEF;

extern long long unsigned hxhash(const char *str);

extern HXT_FUNC_SIG(HXT_STRUCT*, create,);
extern HXT_FUNC_SIG(void, destroy, HXT_STRUCT* table);
extern HXT_FUNC_SIG(bool, set, HXT_STRUCT *table, const char *key, HXT_TYPE value);
extern HXT_FUNC_SIG(HXT_TYPE, get, HXT_STRUCT *table, const char *key);

#endif

#ifdef HIRZEL_UTIL_TABLE_I
#undef HIRZEL_UTIL_TABLE_I

HXT_FUNC_SIG(HXT_STRUCT*, create,)
{
	HXT_STRUCT *table = malloc(sizeof(HXT_STRUCT));
	if (!table) return NULL;
	table->data = malloc(HXT_INIT_SIZE * sizeof(HXT_ITEM));
	if (!table->data)
	{
		free(table);
		return NULL;
	}
	table->size = HXT_INIT_SIZE;
	table->count = 0;
	return table;
}

HXT_FUNC_SIG(void, destroy, HXT_STRUCT* table)
{
	free(table->data);
	free(table);
}

// for now, doesn't handle collisions
HXT_FUNC_SIG(bool, set, HXT_STRUCT *table, const char *key, HXT_TYPE value)
{
	size_t pos = hxhash(key) % table->size;
	printf("Pos: %zu\n", pos);
	table->data[pos].value = value;
	return true;
}

HXT_FUNC_SIG(HXT_TYPE, get, HXT_STRUCT *table, const char *key)
{
	size_t pos = hxhash(key) % table->size;
	return table->data[pos].value;
}

long long unsigned hxhash(const char *str)
{
	long long unsigned hash = 0;
	long long unsigned pop = 1;

	while (*str)
	{
		hash = (hash + (*str - 'a' + 1) * pop) % HXT_HASH_M;
		pop = (pop * HXT_HASH_P) % HXT_HASH_M;
		str += 1;
	}

	return hash;
}

#endif
