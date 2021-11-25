#define HIRZEL_DEBUG
#define HIRZEL_IMPLEMENT
#include <hirzel/util/table.h>

// standard library
#include <assert.h>
#include <string.h>

void testCreate()
{
	puts("\tTesting createHxTable()");

	HxTable *table = createHxTable(sizeof(int));

	assert(table != NULL);
	assert(table->data != NULL);
	assert(table->element_size == sizeof(int));
	assert(table->count == 0);
	assert(table->size_index == 0);

	destroyHxTable(table);
}

void testResize()
{
	puts("\tTesting resizeHxTable()");

	HxTable *table = createHxTable(sizeof(int));

	resizeHxTable(table, 1);
	assert(table->size_index == 1);
	size_t size = sizeHxTable(table);
	HxTableNode* data = table->data;
	HxTableNode* back = data + size - 1;
	assert(back->key == NULL);
	assert(back->value == NULL);
	assert(back->deleted == false);

	destroyHxTable(table);
}

void testSet()
{
	puts("\tTesting setHxTable()");
	HxTable *table = createHxTable(sizeof(int));

	int value = 35;
	setHxTable(table, "abc", &value);

	assert(table->count == 1);
	HxTableNode *data = table->data;
	size_t size = sizeHxTable(table);

	for (size_t i = 0; i < size; ++i)
	{
		if (data[i].key && !strcmp("abc", data[i].key))
		{
			int *value_ptr = data[i].value;
			assert(*value_ptr == value);
		}
	}

	destroyHxTable(table);
}

void testGet()
{
	puts("\tTesting getHxTable()");
	HxTable *table = createHxTable(sizeof(int));
	destroyHxTable(table);
}

void test()
{
	puts("\tTesting HxTable()");
	HxTable *table = createHxTable(sizeof(int));
	destroyHxTable(table);
}

int main(void)
{
	testCreate();
	testResize();
	testGet();
	testSet();

	puts("All tests passed");

	return 0;
}
