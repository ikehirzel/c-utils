#include <hirzel/table.h>

HIRZEL_TABLE_DECLARE(int, IntTable)
HIRZEL_TABLE_DEFINE(int, IntTable)

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

void test_init()
{
	puts("\tTesting init()");

	IntTable table;
	assert(IntTable_init(&table));

	assert(table.data != NULL);
	assert(table.count == 0);
	assert(table.size_index == 0);

	IntTable_free(&table);
}

#define assert_node(name) assert(#name " is valid" && name->key == NULL && name->is_deleted == false)

void test_resize()
{
	puts("\tTesting resize()");

	IntTable table;
	assert(IntTable_init(&table));

	size_t size = IntTable_sizes[0];
	IntTableNode *front = table.data;
	IntTableNode *back = table.data + size - 1;
	assert_node(front);
	assert_node(back);

	IntTable_resize(&table, 1);
	assert(table.size_index == 1);
	size = IntTable_sizes[1];
	front = table.data;
	back = table.data + size - 1;
	assert_node(front);
	assert_node(back);

	IntTable_free(&table);

	IntTable_init(&table);

	IntTable_resize(&table, 3);
	assert(table.size_index == 3);
	size = IntTable_sizes[3];
	front = table.data;
	back = table.data + size - 1;
	assert_node(front);
	assert_node(back);

	IntTable_free(&table);
}

void test_reserve()
{
	puts("\tTesting reserve()");
	IntTable table;
	assert(IntTable_init(&table));

	IntTable_reserve(&table, 10);
	size_t size = IntTable_sizes[table.size_index];
	IntTableNode *front = table.data;
	IntTableNode *back = front + size - 1;
	assert(size > 10 * 2);
	assert_node(front);
	assert_node(back);

	IntTable_reserve(&table, 500);
	size = IntTable_sizes[table.size_index];
	front = table.data;
	back = front + size - 1;
	assert(size > 500 * 2);
	assert_node(front);
	assert_node(back);

	IntTable_free(&table);
}

void test_shrink()
{
	puts("\tTesting shrink()");

	IntTable table;
	assert(IntTable_init(&table));

	IntTable_resize(&table, 3);
	IntTable_shrink(&table);
	assert(table.size_index == 0);

	IntTable_resize(&table, 3);
	table.count = 10;
	IntTable_shrink(&table);
	assert(table.size_index == 1);

	IntTable_free(&table);
}

void test_set()
{
	puts("\tTesting set()");

	IntTable table;
	assert(IntTable_init(&table));

	for (size_t i = 0; i < valid_key_count; ++i)
	{
		const char *key = valid_keys[i];		
		assert(IntTable_set(&table, key, i * 3));
		assert(table.count == i + 1);
	}

	size_t table_size = IntTable_size(&table);
	for (size_t key_index = 0; key_index < valid_key_count; ++key_index)
	{
		bool found = false;
		const char *search_key = valid_keys[key_index];
		for (size_t node_index = 0; node_index < table_size; ++node_index)
		{
			IntTableNode *node = table.data + node_index;

			if (node->key && !strcmp(node->key, search_key))
			{
				found = true;
				int expected = key_index * 3;
				assert(node->value == expected);
			}
		}

		assert(found == true);
	}

	IntTable_free(&table);
}

void test_erase()
{
	puts("\tTesting erase()");

	IntTable table;
	assert(IntTable_init(&table));

	for (size_t i = 0; i < valid_key_count; ++i)
		assert(IntTable_set(&table, valid_keys[i], i * 8));

	for (size_t i = 0; i < valid_key_count; ++i)
		IntTable_erase(&table, valid_keys[i]);

	size_t size = IntTable_sizes[table.size_index];
	unsigned deleted_count = 0;

	for (size_t i = 0; i < size; ++i)
	{
		const IntTableNode *node = table.data + i;
		assert(node->key == NULL);
		if (node->is_deleted)
			++deleted_count;
	}

	assert(deleted_count == valid_key_count);

	IntTable_free(&table);
}

void test_clear()
{
	puts("\tTesting clear()");

	IntTable table;
	assert(IntTable_init(&table));

	for (size_t i = 0; i < valid_key_count; ++i)
		assert(IntTable_set(&table, valid_keys[i], i));

	IntTable_clear(&table);

	for (size_t i = 0; i < IntTable_sizes[table.size_index]; ++i)
	{
		IntTableNode *node = table.data + i;

		assert(node->key == NULL);
		assert(node->is_deleted == false);
	}

	
	IntTable_free(&table);
}

void test_swap()
{
	puts("\tTesting swap()");

	IntTable table;
	assert(IntTable_init(&table));

	for (size_t i = 0; i < valid_key_count; ++i)
		assert(IntTable_set(&table, valid_keys[i], i));

	IntTable_swap(&table, valid_keys[0], valid_keys[1]);
	IntTable_swap(&table, valid_keys[1], valid_keys[2]);

	int expected[] = { 1, 2, 0 };
	size_t expected_count = sizeof(expected) / sizeof(*expected);

	for (size_t e = 0; e < expected_count; ++e)
	{
		const char *key = valid_keys[e];
		int value = expected[e];

		for (size_t i = 0; i < IntTable_sizes[table.size_index]; ++i)
		{
			IntTableNode *node = table.data + i;

			if (node->key && !strcmp(node->key, key))
			{
				assert(node->value == value);

				break;
			}
		}
	}

	IntTable_free(&table);
}

void test_get()
{
	puts("\tTesting get()");

	IntTable table;
	assert(IntTable_init(&table));

	for (size_t i = 0; i < valid_key_count; ++i)
	{
		const char *key = valid_keys[i];
		int value = 8 * i;

		assert(IntTable_set(&table, key, value));
		int retrieved;
		assert(IntTable_get(&table, &retrieved, key));
		assert(value == retrieved);
	}

	for (size_t i = 0; i < valid_key_count; ++i)
	{
		const char *key = valid_keys[i];
		int retrieved;
		assert(IntTable_get(&table, &retrieved, key));
		assert(retrieved == 8 * (int)i);
	}

	IntTable_free(&table);
}


void test_get_ptr()
{
	puts("\tTesting at()");

	IntTable table;
	assert(IntTable_init(&table));

	for (size_t i = 0; i < valid_key_count; ++i)
		assert(IntTable_set(&table, valid_keys[i], i));

	for (size_t i = 0; i < valid_key_count; ++i)
	{
		int *ptr = IntTable_get_ptr(&table, valid_keys[i]);
		assert(ptr);
		assert(*ptr == (int)i);
	}

	IntTable_free(&table);
}

void test_contains()
{
	puts("\tTesting contains()");

	IntTable table;
	assert(IntTable_init(&table));

	for (size_t i = 0; i < valid_key_count; ++i)
		assert(!IntTable_contains(&table, valid_keys[i]));

	for (size_t i = 0; i < invalid_key_count; ++i)
		assert(!IntTable_contains(&table, invalid_keys[i]));

	for (size_t i = 0; i < valid_key_count; ++i)
	{
		assert(IntTable_set(&table, valid_keys[i], i));
		assert(IntTable_contains(&table, valid_keys[i]));
	}

	for (size_t i = 0; i < valid_key_count; ++i)
		assert(IntTable_contains(&table, valid_keys[i]));

	for (size_t i = 0; i < invalid_key_count; ++i)
		assert(!IntTable_contains(&table, invalid_keys[i]));

	IntTable_free(&table);
}

void test_size()
{
	puts("\tTesting size()");

	IntTable table;
	assert(IntTable_init(&table));

	for (size_t i = 0; i < IntTable_size_count; ++i)
	{
		table.size_index = i;
		assert(IntTable_size(&table) == IntTable_sizes[i]);
	}

	table.size_index = 0;

	IntTable_free(&table);
}

void test_is_empty()
{
	puts("\tTesting is_empty()");

	IntTable table;
	assert(IntTable_init(&table));

	assert(IntTable_is_empty(&table));
	table.count = 1;
	assert(!IntTable_is_empty(&table));
	table.count = 100;
	assert(!IntTable_is_empty(&table));

	IntTable_free(&table);
}

int main(void)
{
	puts("Testing Table...");
	test_init();
	test_resize();
	test_reserve();
	test_shrink();
	test_set();
	test_erase();
	test_clear();
	test_swap();
	test_get();
	test_get_ptr();
	test_contains();
	test_size();
	test_is_empty();

	puts("All tests passed");

	return 0;
}
