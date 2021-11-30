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

void testCreate()
{
	puts("\tTesting createHxTable()");

	HxTable *table = createHxTableOf(int);

	assert(table != NULL);
	assert(table->data != NULL);
	assert(table->element_size == sizeof(int));
	assert(table->count == 0);
	assert(table->size_index == 0);

	destroyHxTable(table);
}

#define assert_node(name) assert(#name " is valid" && name->key == NULL && name->value == NULL && name->deleted == false)

void testResize()
{
	puts("\tTesting resizeHxTable()");

	HxTable *table = createHxTableOf(int);
	assert(table != NULL);

	size_t size = HxTable_sizes[0];
	HxTableNode *front = table->data;
	HxTableNode *back = table->data + size - 1;
	assert_node(front);
	assert_node(back);

	resizeHxTable(table, 1);
	assert(table->size_index == 1);
	size = HxTable_sizes[1];
	front = table->data;
	back = table->data + size - 1;
	assert_node(front);
	assert_node(back);

	destroyHxTable(table);

	table = createHxTable(35);
	assert(table != NULL);

	resizeHxTable(table, 3);
	assert(table->size_index == 3);
	size = HxTable_sizes[3];
	front = table->data;
	back = table->data + size - 1;
	assert_node(front);
	assert_node(back);

	destroyHxTable(table);
}

void testReserve()
{
	puts("\tTesting reserveHxTable()");
	HxTable *table = createHxTableOf(bool);
	assert(table != NULL);

	reserveHxTable(table, 10);
	size_t size = HxTable_sizes[table->size_index];
	HxTableNode *front = table->data;
	HxTableNode *back = front + size - 1;
	assert(size > 10 * 2);
	assert_node(front);
	assert_node(back);

	reserveHxTable(table, 500);
	size = HxTable_sizes[table->size_index];
	front = table->data;
	back = front + size - 1;
	assert(size > 500 * 2);
	assert_node(front);
	assert_node(back);

	destroyHxTable(table);
}

void testShrink()
{
	puts("\tTesting shrinkHxTable()");

	HxTable *table = createHxTableOf(int);
	assert(table != NULL);

	resizeHxTable(table, 3);
	shrinkHxTable(table);
	assert(table->size_index == 0);

	resizeHxTable(table, 3);
	table->count = 10;
	shrinkHxTable(table);
	assert(table->size_index == 1);

	destroyHxTable(table);
}

void testSet()
{
	puts("\tTesting setHxTable()");
	HxTable *table = createHxTableOf(int);

	for (size_t i = 0; i < valid_key_count; ++i)
	{
		const char *key = valid_keys[i];
		int value = i * 3;

		assert(setHxTable(table, key, &value));
		assert(table->count == i + 1);
	}

	size_t table_size = sizeHxTable(table);
	for (size_t key_index = 0; key_index < valid_key_count; ++key_index)
	{
		bool found = false;
		const char *search_key = valid_keys[key_index];
		for (size_t node_index = 0; node_index < table_size; ++node_index)
		{
			HxTableNode *node = table->data + node_index;

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

	destroyHxTable(table);
}

void testErase()
{
	puts("\tTesting eraseHxTable()");
	HxTable *table = createHxTableOf(int);
	assert(table != NULL);

	for (size_t i = 0; i < valid_key_count; ++i)
	{
		const char *key = valid_keys[i];
		int value = i * 8;

		assert(setHxTable(table, key, &value));
	}

	for (size_t i = 0; i < valid_key_count; ++i)
	{
		const char *key = valid_keys[i];

		eraseHxTable(table, key);
	}

	size_t size = HxTable_sizes[table->size_index];
	unsigned deleted_count = 0;

	for (size_t i = 0; i < size; ++i)
	{
		const HxTableNode* node = table->data + i;
		assert(node->key == NULL);
		assert(node->value == NULL);
		if (node->deleted)
			++deleted_count;
	}

	assert(deleted_count == valid_key_count);

	destroyHxTable(table);
}

void testClear()
{
	puts("\tTesting clearHxTable()");
	HxTable *table = createHxTableOf(int);

	for (size_t i = 0; i < valid_key_count; ++i)
	{
		const char *key = valid_keys[i];
		int value = i;

		assert(setHxTable(table, key, &value));
	}

	clearHxTable(table);

	for (size_t i = 0; i < HxTable_sizes[table->size_index]; ++i)
	{
		HxTableNode *node = table->data + i;

		assert(node->key == NULL);
		assert(node->value == NULL);
		assert(node->deleted == false);
	}

	assert(table != NULL);
	destroyHxTable(table);
}

void testSwap()
{
	puts("\tTesting swapHxTable()");

	HxTable *table = createHxTableOf(int);
	assert(table != NULL);

	for (size_t i = 0; i < valid_key_count; ++i)
	{
		const char *key = valid_keys[i];
		int value = i;

		assert(setHxTable(table, key, &value));
	}

	int tmp;
	swapHxTable(table, &tmp, valid_keys[0], valid_keys[1]);
	swapHxTable(table, &tmp, valid_keys[1], valid_keys[2]);

	int expected[] = { 1, 2, 0 };
	size_t expected_count = sizeof(expected) / sizeof(*expected);

	for (size_t e = 0; e < expected_count; ++e)
	{
		const char *key = valid_keys[e];
		int value = expected[e];

		for (size_t i = 0; i < HxTable_sizes[table->size_index]; ++i)
		{
			HxTableNode *node = table->data + i;

			if (node->key && !strcmp(node->key, key))
			{
				int *ptr = node->value;
				assert(*ptr == value);

				break;
			}
		}
	}

	destroyHxTable(table);
}

void testGet()
{
	puts("\tTesting getHxTable()");
	HxTable *table = createHxTableOf(double);
	assert(table != NULL);

	for (size_t i = 0; i < valid_key_count; ++i)
	{
		const char *key = valid_keys[i];
		double value = 8.25 * i;

		assert(setHxTable(table, key, &value));
		double retrieved;
		assert(getHxTable(table, &retrieved, key));
		assert(value == retrieved);
	}

	for (size_t i = 0; i < valid_key_count; ++i)
	{
		const char *key = valid_keys[i];
		double retrieved;

		assert(getHxTable(table, &retrieved, key));
		assert(retrieved == 8.25 * i);
	}

	destroyHxTable(table);
}


void testAt()
{
	puts("\tTesting atHxTable()");
	HxTable *table = createHxTableOf(int);
	assert(table != NULL);

	for (size_t i = 0; i < valid_key_count; ++i)
	{
		const char *key = valid_keys[i];
		int value = i;

		assert(setHxTable(table, key, &value));
	}

	for (size_t i = 0; i < valid_key_count; ++i)
	{
		int *ptr = atHxTable(table, valid_keys[i]);
		assert(ptr);
		assert(*ptr == (int)i);
	}

	destroyHxTable(table);
}

void testContains()
{
	puts("\tTesting HxTable()");

	HxTable *table = createHxTableOf(int);
	assert(table != NULL);

	for (size_t i = 0; i < valid_key_count; ++i)
		assert(!containsHxTable(table, valid_keys[i]));

	for (size_t i = 0; i < invalid_key_count; ++i)
		assert(!containsHxTable(table, invalid_keys[i]));

	for (size_t i = 0; i < valid_key_count; ++i)
	{
		int value = i;
		assert(setHxTable(table, valid_keys[i], &value));
		assert(containsHxTable(table, valid_keys[i]));
	}

	for (size_t i = 0; i < valid_key_count; ++i)
		assert(containsHxTable(table, valid_keys[i]));

	for (size_t i = 0; i < invalid_key_count; ++i)
		assert(!containsHxTable(table, invalid_keys[i]));

	destroyHxTable(table);
}

void testSize()
{
	puts("\tTesting sizeHxTable()");

	HxTable *table = createHxTableOf(int);
	assert(table != NULL);

	for (size_t i = 0; i < HxTable_size_count; ++i)
	{
		table->size_index = i;
		assert(sizeHxTable(table) == HxTable_sizes[i]);
	}

	table->size_index = 0;

	destroyHxTable(table);
}

void testIsEmpty()
{
	puts("\tTesting isEmptyHxTable()");

	HxTable *table = createHxTableOf(int);
	assert(table != NULL);

	assert(isEmptyHxTable(table));
	table->count = 1;
	assert(!isEmptyHxTable(table));
	table->count = 100;
	assert(!isEmptyHxTable(table));

	destroyHxTable(table);
}

int main(void)
{
	puts("Testing HxTable...");
	testCreate();
	testResize();
	testReserve();
	testShrink();
	testSet();
	testErase();
	testClear();
	testSwap();
	testGet();
	testAt();
	testContains();
	testSize();
	testIsEmpty();

	puts("All tests passed");

	return 0;
}
