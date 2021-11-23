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

void testCreate()
{
	puts("\tTesting creation");

	HxArray *a = createHxArray(sizeof(int), 0);
	assertDefaultArray(a, sizeof(int), 0);
	destroyHxArray(a);

	HxArray *b = createHxArray(sizeof(double), 10);
	assertDefaultArray(b, sizeof(double), 10);
	destroyHxArray(b);
}

void testIncrement()
{
	puts("\tTesting increment");

	HxArray *arr = createHxArray(sizeof(float), 15);

	for (size_t i = 0; i < 15; ++i)
	{
		assert(arr->length == i);
		assert(incrementHxArray(arr));
		assert(arr->capacity == 15);
	}

	for (size_t i = 15; i < 30; ++i)
	{
		assert(arr->length == i);
		assert(incrementHxArray(arr));
		assert(arr->capacity == i + 1);
	}	

	destroyHxArray(arr);
}

void testResize()
{
	
}

int main(void)
{
	testCreate();
	testIncrement();


	puts("All tests passed");

	return 0;
}
