#include <stdio.h>
#include <assert.h>

#define HIRZEL_UTIL_LIST_T int
#include <hirzel/util/list.h>
#define HIRZEL_UTIL_LIST_T int
#include <hirzel/util/table.h>
#include <hirzel/util/file.h>

void test_file()
{
}

void test_list()
{
	// testing list creation
	hxlist_int_t *list = hxlist_int_create();
	assert(list != NULL);
	assert(list->data == NULL);
	assert(list->len == 0);
	assert(hxlist_is_empty(list));

	// pushing
	assert(hxlist_int_push(list, 3));
	// 3
	assert(hxlist_size(list) == 1);
	assert(hxlist_get(list, 0) == 3);
	assert(hxlist_back(list) == 3);
	assert(hxlist_front(list) == 3);
	assert(!hxlist_is_empty(list));

	assert(hxlist_int_push(list, 4));
	// 3 4
	assert(hxlist_size(list) == 2);
	assert(hxlist_get(list, 1) == 4);
	assert(hxlist_back(list) == 4);
	assert(hxlist_front(list) == 3);
	assert(!hxlist_is_empty(list));

	assert(hxlist_int_pushf(list, 9));
	// 9 3 4
	assert(hxlist_size(list) == 3);
	assert(hxlist_front(list) == 9);
	assert(hxlist_back(list) == 4);

	// inserting

	assert(hxlist_int_insert(list, 1, 12));
	// 9 12 3 4
	assert(hxlist_front(list) == 9);
	assert(hxlist_get(list, 1) == 12);
	assert(hxlist_get(list, 2) == 3);
	assert(hxlist_back(list) == 4);
	assert(hxlist_size(list) == 4);

	// popping

	assert(hxlist_int_pop(list));
	// 9 12 3
	assert(hxlist_back(list) == 3);
	assert(hxlist_size(list) == 3);
	assert(hxlist_size(list) == 3);
	assert(hxlist_front(list) == 9);

	assert(hxlist_int_popf(list));
	// 12 3
	assert(hxlist_front(list) == 12);
	assert(hxlist_back(list) == 3);
	assert(hxlist_size(list) == 2);

	// freeing data
	hxlist_int_destroy(list);
}

void test_table()
{
	// CREATION
	hxtbl_int_t *table = hxtbl_int_create();
	assert(table != NULL);
	assert(table->data != NULL);
	assert(table->size == HXT_INIT_SIZE);
	assert(table->count == 0);

	const char *keys[] =
	{
		"a", "b", "c", "d", "e", "f", "h", "i", "j", "k", "l", "m", "n",
		"o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"
	};
	// SETTING
	for (size_t i = 0; i < (sizeof(keys) / sizeof(const char*)); ++i)
	{
		assert(hxtbl_int_set(table, keys[i], i * 2));
		assert(table->count == i + 1);
	}

	// GETTING
	for (size_t i = 0; i < (sizeof(keys) / sizeof(const char*)); ++i)
	{
		printf("Testing get @ %zu\n", i);
		assert(hxtbl_int_get(table, keys[i]) == i * 2);
	}

	// DESTRUCTION
	hxtbl_int_destroy(table);
}

int main(void)
{
	test_table();
	//print_primes();
	return 0;
}
