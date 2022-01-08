#define HIRZEL_IMPLEMENT
#include <hirzel/table.h>

// standard library
#include <assert.h>
#include <string.h>
#include <stdio.h>

const char * const valid_keys[] = {
	"abc", "def", "hij", "klm", "nop", "qrs", "tuv", "wxy", "z"
};

const size_t valid_key_count = sizeof(valid_keys) / sizeof(*valid_keys);

const char * const invalid_keys[] = {
	"hello",
	"my",
	"name",
	"is",
	"Ike"
};

const size_t invalid_key_count = sizeof(invalid_keys) / sizeof(*invalid_keys);

void test_create()
{
	puts("\tTesting hxtable_create()");

	HxTable table = hxtable_create_of(int);

	assert(table->data = NULL);
	assert(table->element_size == sizeof(int));
	assert(table->count == 0);
	assert(table->size_index == 0);

	hxtable_destroy(table);
}

#define assert_node(name) assert(#name " is valid" && name->key == NULL && name->value == NULL && name->deleted == false)

void test_resize()
{
	puts("\tTesting hxtable_resize()");

	HxTable *table = hxtable_create_of(int);
	assert(table != NULL);

	size_t size = hxtable_sizes[0];
	struct HxTableNode *front = table->data;
	struct HxTableNode *back = table->data + size - 1;
	assert_node(front);
	assert_node(back);

	hxtable_resize(table, 1);
	assert(table->size_index == 1);
	size = hxtable_sizes[1];
	front = table->data;
	back = table->data + size - 1;
	assert_node(front);
	assert_node(back);

	hxtable_destroy(table);

	table = hxtable_create(35);
	assert(table != NULL);

	hxtable_resize(table, 3);
	assert(table->size_index == 3);
	size = hxtable_sizes[3];
	front = table->data;
	back = table->data + size - 1;
	assert_node(front);
	assert_node(back);

	hxtable_destroy(table);
}

void test_reserve()
{
	puts("\tTesting hxtable_reserve()");
	HxTable *table = hxtable_create_of(bool);
	assert(table != NULL);

	hxtable_reserve(table, 10);
	size_t size = hxtable_sizes[table->size_index];
	struct HxTableNode *front = table->data;
	struct HxTableNode *back = front + size - 1;
	assert(size > 10 * 2);
	assert_node(front);
	assert_node(back);

	hxtable_reserve(table, 500);
	size = hxtable_sizes[table->size_index];
	front = table->data;
	back = front + size - 1;
	assert(size > 500 * 2);
	assert_node(front);
	assert_node(back);

	hxtable_destroy(table);
}

void test_shrink()
{
	puts("\tTesting hxtable_shrink()");

	HxTable *table = hxtable_create_of(int);
	assert(table != NULL);

	hxtable_resize(table, 3);
	hxtable_shrink(table);
	assert(table->size_index == 0);

	hxtable_resize(table, 3);
	table->count = 10;
	hxtable_shrink(table);
	assert(table->size_index == 1);

	hxtable_destroy(table);
}

void test_set()
{
	puts("\tTesting hxtable_set()");
	HxTable *table = hxtable_create_of(int);

	for (size_t i = 0; i < valid_key_count; ++i)
	{
		const char *key = valid_keys[i];
		int value = i * 3;

		assert(hxtable_set(table, key, &value));
		assert(table->count == i + 1);
	}

	size_t table_size = hxtable_size(table);
	for (size_t key_index = 0; key_index < valid_key_count; ++key_index)
	{
		bool found = false;
		const char *search_key = valid_keys[key_index];
		for (size_t node_index = 0; node_index < table_size; ++node_index)
		{
			struct HxTableNode *node = table->data + node_index;

			if (node->key && !strcmp(node->key, search_key))
			{
				found = true;
				int *value = node->value;
				int expected = key_index * 3;
				assert(*value == expected);
			}
		}

		assert(found == true);
	}

	hxtable_destroy(table);
}

void test_erase()
{
	puts("\tTesting hxtable_erase()");
	HxTable *table = hxtable_create_of(int);
	assert(table != NULL);

	for (size_t i = 0; i < valid_key_count; ++i)
	{
		const char *key = valid_keys[i];
		int value = i * 8;

		assert(hxtable_set(table, key, &value));
	}

	for (size_t i = 0; i < valid_key_count; ++i)
	{
		const char *key = valid_keys[i];

		hxtable_erase(table, key);
	}

	size_t size = hxtable_sizes[table->size_index];
	unsigned deleted_count = 0;

	for (size_t i = 0; i < size; ++i)
	{
		const struct HxTableNode *node = table->data + i;
		assert(node->key == NULL);
		assert(node->value == NULL);
		if (node->deleted)
			++deleted_count;
	}

	assert(deleted_count == valid_key_count);

	hxtable_destroy(table);
}

void test_clear()
{
	puts("\tTesting hxtable_clear()");
	HxTable *table = hxtable_create_of(int);

	for (size_t i = 0; i < valid_key_count; ++i)
	{
		const char *key = valid_keys[i];
		int value = i;

		assert(hxtable_set(table, key, &value));
	}

	hxtable_clear(table);

	for (size_t i = 0; i < hxtable_sizes[table->size_index]; ++i)
	{
		struct HxTableNode *node = table->data + i;

		assert(node->key == NULL);
		assert(node->value == NULL);
		assert(node->deleted == false);
	}

	assert(table != NULL);
	hxtable_destroy(table);
}

void test_swap()
{
	puts("\tTesting hxtable_swap()");

	HxTable *table = hxtable_create_of(int);
	assert(table != NULL);

	for (size_t i = 0; i < valid_key_count; ++i)
	{
		const char *key = valid_keys[i];
		int value = i;

		assert(hxtable_set(table, key, &value));
	}

	int tmp;
	hxtable_swap(table, &tmp, valid_keys[0], valid_keys[1]);
	hxtable_swap(table, &tmp, valid_keys[1], valid_keys[2]);

	int expected[] = { 1, 2, 0 };
	size_t expected_count = sizeof(expected) / sizeof(*expected);

	for (size_t e = 0; e < expected_count; ++e)
	{
		const char *key = valid_keys[e];
		int value = expected[e];

		for (size_t i = 0; i < hxtable_sizes[table->size_index]; ++i)
		{
			struct HxTableNode *node = table->data + i;

			if (node->key && !strcmp(node->key, key))
			{
				int *ptr = node->value;
				assert(*ptr == value);

				break;
			}
		}
	}

	hxtable_destroy(table);
}

void test_get()
{
	puts("\tTesting hxtable_get()");
	HxTable *table = hxtable_create_of(double);
	assert(table != NULL);

	for (size_t i = 0; i < valid_key_count; ++i)
	{
		const char *key = valid_keys[i];
		double value = 8.25 * i;

		assert(hxtable_set(table, key, &value));
		double retrieved;
		assert(hxtable_get(table, &retrieved, key));
		assert(value == retrieved);
	}

	for (size_t i = 0; i < valid_key_count; ++i)
	{
		const char *key = valid_keys[i];
		double retrieved;

		assert(hxtable_get(table, &retrieved, key));
		assert(retrieved == 8.25 * i);
	}

	hxtable_destroy(table);
}


void test_at()
{
	puts("\tTesting hxtable_at()");
	HxTable *table = hxtable_create_of(int);
	assert(table != NULL);

	for (size_t i = 0; i < valid_key_count; ++i)
	{
		const char *key = valid_keys[i];
		int value = i;

		assert(hxtable_set(table, key, &value));
	}

	for (size_t i = 0; i < valid_key_count; ++i)
	{
		int *ptr = hxtable_at(table, valid_keys[i]);
		assert(ptr);
		assert(*ptr == (int)i);
	}

	hxtable_destroy(table);
}

void test_contains()
{
	puts("\tTesting hxtable_contains()");

	HxTable *table = hxtable_create_of(int);
	assert(table != NULL);

	for (size_t i = 0; i < valid_key_count; ++i)
		assert(!hxtable_contains(table, valid_keys[i]));

	for (size_t i = 0; i < invalid_key_count; ++i)
		assert(!hxtable_contains(table, invalid_keys[i]));

	for (size_t i = 0; i < valid_key_count; ++i)
	{
		int value = i;
		assert(hxtable_set(table, valid_keys[i], &value));
		assert(hxtable_contains(table, valid_keys[i]));
	}

	for (size_t i = 0; i < valid_key_count; ++i)
		assert(hxtable_contains(table, valid_keys[i]));

	for (size_t i = 0; i < invalid_key_count; ++i)
		assert(!hxtable_contains(table, invalid_keys[i]));

	hxtable_destroy(table);
}

void test_size()
{
	puts("\tTesting hxtable_size()");

	HxTable *table = hxtable_create_of(int);
	assert(table != NULL);

	for (size_t i = 0; i < HxTable_size_count; ++i)
	{
		table->size_index = i;
		assert(hxtable_size(table) == hxtable_sizes[i]);
	}

	table->size_index = 0;

	hxtable_destroy(table);
}

void test_is_empty()
{
	puts("\tTesting hxtable_is_empty()");

	HxTable *table = hxtable_create_of(int);
	assert(table != NULL);

	assert(hxtable_is_empty(table));
	table->count = 1;
	assert(!hxtable_is_empty(table));
	table->count = 100;
	assert(!hxtable_is_empty(table));

	hxtable_destroy(table);
}

int main(void)
{
	puts("Testing HxTable...");
	test_create();
	test_resize();
	test_reserve();
	test_shrink();
	test_set();
	test_erase();
	test_clear();
	test_swap();
	test_get();
	test_at();
	test_contains();
	test_size();
	test_is_empty();

	puts("All test_s passed");

	return 0;
}
