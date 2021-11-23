#include <hirzel/util/array.h>

#define HIRZEL_DEBUG
#define HIRZEL_IMPLEMENT
#include <hirzel/util/array.h>

// standard library
#include <stdio.h>
#include <assert.h>

void assertDefaultArray(HxArray *arr, size_t element_size, size_t capacity)
{
	assert(arr != NULL);
	assert(arr->length == 0);
	assert(arr->capacity == capacity);
	assert(arr->element_size == element_size);
	if (capacity > 0)
		assert(arr->data != NULL);
	else
		assert(arr->data == NULL);
}

int main(void)
{
	HxArray *arr = createHxArray(sizeof(int), 10);
	assertDefaultArray(arr, sizeof(int), 10);

	for (int i = 0; i < 15; ++i)
	{
		int value = i * 3;
		pushHxArray(arr, &value);
	}

	for (size_t i = 0; i < 15; ++i)
	{
		int value;
		getHxArray(arr, &value, i);
		assert(value == (int)i * 3);
	}

	return 0;
}
