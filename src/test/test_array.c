#define HIRZEL_IMPLEMENT
#include <hirzel/array.h>

// standard library
#include <stdio.h>
#include <assert.h>

void test_hxarray_create()
{
	puts("\tTesting create()");

	HxArray *array = hxarray_create_of(int);
	assert(array != NULL);
	assert(array->data == NULL);
	assert(array->element_size == sizeof(int));
	assert(array->length == 0);
	assert(array->capacity == 0);
	hxarray_destroy(array);

	array = hxarray_create_of(double);
	assert(array != NULL);
	assert(array->data == NULL);
	assert(array->element_size == sizeof(double));
	assert(array->length == 0);
	assert(array->capacity == 0);
	hxarray_destroy(array);
}

void test_hxarray_reserve()
{
	puts("\tTesting hxarray_reserve()");
	HxArray *arr = hxarray_create_of(int);
	assert(arr != NULL);

	assert(hxarray_reserve(arr, 2));
	assert(arr->length == 0);
	assert(arr->data != NULL);
	assert(arr->capacity == 2);
	assert(arr->element_size == sizeof(int));

	int *data = arr->data;
	data[0] = 7;
	data[1] = 5;
	assert(data[0] == 7);
	assert(data[1] == 5);

	assert(hxarray_reserve(arr, 4));
	assert(arr->length == 0);
	assert(arr->data != NULL);
	assert(arr->capacity == 4);
	assert(arr->element_size == sizeof(int));

	data = arr->data;
	data[2] = 76;
	data[3] = 87;
	assert(data[0] == 7);
	assert(data[1] == 5);
	assert(data[2] == 76);
	assert(data[3] == 87);

	hxarray_destroy(arr);
}

void test_hxarray_resize()
{
	puts("\tTesting resize()");
	HxArray *arr = hxarray_create_of(int);
	assert(arr != NULL);
	
	assert(hxarray_resize(arr, 2));
	assert(arr->length == 2);
	assert(arr->capacity == 2);

	int *data = arr->data;
	data[0] = 2;
	data[1] = 600;
	assert(data[0] == 2);
	assert(data[1] == 600);

	assert(hxarray_resize(arr, 4));
	assert(arr->length == 4);
	assert(arr->capacity == 4);

	data = arr->data;
	data[2] = 5;
	data[3] = 984;
	assert(data[0] == 2);
	assert(data[1] == 600);
	assert(data[2] == 5);
	assert(data[3] == 984);

	assert(hxarray_resize(arr, 0));
	assert(arr->length == 0);
	assert(arr->capacity == 4);

	hxarray_destroy(arr);
}

void test_hxarray_increment_size()
{
	puts("\tTesting hxarray_increment_size()");

	HxArray *arr = hxarray_create(sizeof(float));
	assert(arr != NULL);

	for (size_t i = 0; i < 10; ++i)
	{
		assert(hxarray_increment_size(arr));
		assert(arr->length == i + 1);
		assert(arr->capacity == i + 1);
		float *data = arr->data;
		data[arr->length - 1] = (float)(i * 3);
		assert(data[arr->length - 1] == (float)(i * 3));
	}	

	hxarray_destroy(arr);
}

void test_hxarray_push()
{
	puts("\tTesting hxarray_push()");

	HxArray *arr = hxarray_create(sizeof(int));
	assert(arr != NULL);	

	int value = 3;
	assert(hxarray_push(arr, &value));

	int *data = arr->data;

	assert(arr->length == 1);
	assert(arr->capacity == 1);
	assert(data[0] == 3);

	value = 4;
	assert(hxarray_push(arr, &value));
	data = arr->data;

	assert(arr->length == 2);
	assert(arr->capacity == 2);
	assert(data[0] == 3 && data[1] == 4);

	hxarray_destroy(arr);
}

void test_hxarray_push_front()
{
	puts("\tTesting hxarray_push_front()");

	HxArray *arr = hxarray_create(sizeof(float));
	assert(arr != NULL);
	hxarray_reserve(arr, 1);

	float value = 6.8f;
	assert(hxarray_push_front(arr, &value));
	float *data = arr->data;

	assert(arr->length == 1);
	assert(arr->capacity == 1);
	assert(data[0] == 6.8f);

	value = 2.7f;
	assert(hxarray_push_front(arr, &value));
	data = arr->data;
	assert(arr->length == 2);
	assert(arr->capacity == 2);
	assert(data[0] == 2.7f);
	assert(data[1] == 6.8f);

	value = 8.0f;
	assert(hxarray_push_front(arr, &value));
	data = arr->data;
	assert(arr->length == 3);
	assert(arr->capacity == 3);
	assert(data[0] == 8.0f);
	assert(data[1] == 2.7f);
	assert(data[2] == 6.8f);

	hxarray_destroy(arr);
}

void test_hxarray_insert()
{
	puts("\tTesting hxarray_insert()");

	HxArray *arr = hxarray_create_of(double);
	assert(arr != NULL);

	double value = 3.2;
	hxarray_insert(arr, 0, &value);
	double *data = arr->data;

	assert(arr->length == 1);
	assert(arr->capacity == 1);
	assert(data[0] == 3.2);

	value = 5.75;
	hxarray_insert(arr, 1, &value);
	data = arr->data;

	assert(arr->length == 2);
	assert(arr->capacity == 2);
	assert(data[0] == 3.2);
	assert(data[1] == 5.75);

	value = 2.345;
	hxarray_insert(arr, 0, &value);
	data = arr->data;

	assert(arr->length == 3);
	assert(arr->capacity == 3);
	assert(data[0] == 2.345);
	assert(data[1] == 3.2);
	assert(data[2] == 5.75);

	value = 1.0;
	hxarray_insert(arr, 1, &value);
	data = arr->data;

	assert(arr->length == 4);
	assert(arr->capacity == 4);
	assert(data[0] == 2.345);
	assert(data[1] == 1.0);
	assert(data[2] == 3.2);
	assert(data[3] == 5.75);

	hxarray_destroy(arr);
}

void test_hxarray_pop()
{
	puts("\tTesting hxarray_pop()");

	HxArray *arr = hxarray_create(sizeof(char));
	assert(arr != NULL);
	hxarray_resize(arr, 3);

	hxarray_pop(arr);
	assert(arr->length == 2);
	assert(arr->capacity == 3);
	
	hxarray_pop(arr);
	assert(arr->length == 1);
	assert(arr->capacity == 3);
	
	hxarray_pop(arr);
	assert(arr->length == 0);
	assert(arr->capacity == 3);

	hxarray_pop(arr);
	assert(arr->length == 0);
	assert(arr->capacity == 3);

	hxarray_destroy(arr);
}

void test_hxarray_pop_front()
{
	puts("\tTesting hxarray_popf()");

	HxArray *arr = hxarray_create_of(char);
	assert(arr != NULL);
	hxarray_resize(arr, 3);

	char *data = arr->data;
	data[0] = 'a';
	data[1] = 'b';
	data[2] = 'c';

	hxarray_pop_front(arr);
	assert(arr->length == 2);
	assert(arr->capacity == 3);
	assert(data[0] == 'b');

	hxarray_pop_front(arr);
	assert(arr->length == 1);
	assert(arr->capacity == 3);
	assert(data[0] == 'c');

	hxarray_pop_front(arr);
	assert(arr->length == 0);
	assert(arr->capacity == 3);

	hxarray_destroy(arr);
}

void test_hxarray_erase()
{
	puts("\tTesting hxarray_erase()");

	HxArray *arr = hxarray_create_of(char);
	assert(arr != NULL);
	hxarray_resize(arr, 3);

	char *data = arr->data;
	data[0] = 'a';
	data[1] = 'b';
	data[2] = 'c';

	hxarray_erase(arr, 1);
	assert(arr->length == 2);
	assert(arr->capacity == 3);
	assert(data[0] == 'a');
	assert(data[1] == 'c');

	hxarray_erase(arr, 0);
	assert(arr->length == 1);
	assert(arr->capacity == 3);
	assert(data[0] == 'c');
	
	hxarray_erase(arr, 0);
	assert(arr->length == 0);
	assert(arr->capacity == 3);

	hxarray_destroy(arr);
}

void test_hxarray_set()
{
	puts("\tTesting hxarray_set()");

	int a = 0;
	int b = 1;
	int c = 2;

	HxArray *arr = hxarray_create_of(int*);
	assert(arr != NULL);
	hxarray_resize(arr, 3);

	int **data = arr->data;
	data[0] = &a;
	data[1] = &b;
	data[2] = &c;

	int *ptr = &a;
	hxarray_set(arr, 1, &ptr);
	assert(data[1] == &a);
	assert(data[0] == data[1]);
	
	hxarray_set(arr, 2, &ptr);
	assert(data[2] == &a);
	assert(data[0] == data[1]);
	assert(data[1] == data[2]);

	ptr = &c;
	hxarray_set(arr, 0, &ptr);
	assert(data[0] == &c);

	assert(arr->length == 3);
	assert(arr->capacity == 3);

	hxarray_destroy(arr);
}

void test_hxarray_get()
{
	puts("\tTesting hxarray_get()");

	HxArray *arr = hxarray_create_of(unsigned long long);
	assert(arr != NULL);
	hxarray_resize(arr, 3);

	unsigned long long *data = arr->data;
	data[0] = 123;
	data[1] = 456;
	data[2] = 789;

	unsigned long long value;
	hxarray_get(arr, &value, 0);
	assert(value == 123);

	hxarray_get(arr, &value, 1);
	assert(value == 456);

	hxarray_get(arr, &value, 2);
	assert(value == 789);

	hxarray_destroy(arr);
}

void test_hxarray_at()
{
	puts("\tTesting hxarray_at()");

	HxArray *arr = hxarray_create_of(unsigned long long);
	hxarray_resize(arr, 3);

	unsigned long long *data = arr->data;
	data[0] = 123;
	data[1] = 456;
	data[2] = 789;

	unsigned long long *ptr;
	ptr = hxarray_at(arr, 0);
	assert(*ptr == 123);

	ptr = hxarray_at(arr, 1);
	assert(*ptr == 456);

	ptr = hxarray_at(arr, 2);
	assert(*ptr == 789);

	hxarray_destroy(arr);
}

void test_hxarray_front()
{
	puts("\tTesting hxarray_front()");

	HxArray *arr = hxarray_create_of(char);
	hxarray_resize(arr, 3);

	char *data = arr->data;
	data[0] = 'a';
	data[1] = 'b';
	data[2] = 'c';

	char *front = hxarray_front(arr);
	assert(*front == 'a');

	char tmp = 'h';
	hxarray_push_front(arr, &tmp);
	front = hxarray_front(arr);
	assert(*front == 'h');

	hxarray_pop_front(arr);
	front = hxarray_front(arr);
	assert(*front == 'a');

	hxarray_pop_front(arr);
	front = hxarray_front(arr);
	assert(*front == 'b');

	hxarray_pop_front(arr);
	front = hxarray_front(arr);
	assert(*front == 'c');

	hxarray_destroy(arr);
}

void test_hxarray_back()
{
	puts("\tTesting hxarray_back()");

	HxArray *arr = hxarray_create_of(char);
	hxarray_resize(arr, 3);

	char *data = arr->data;
	data[0] = 'a';
	data[1] = 'b';
	data[2] = 'c';

	char *back = hxarray_back(arr);
	assert(*back == 'c');

	char tmp = 'h';
	hxarray_push(arr, &tmp);
	back = hxarray_back(arr);
	assert(*back == 'h');

	hxarray_pop(arr);
	back = hxarray_back(arr);
	assert(*back == 'c');

	hxarray_pop(arr);
	back = hxarray_back(arr);
	assert(*back == 'b');

	hxarray_pop(arr);
	back = hxarray_back(arr);
	assert(*back == 'a');

	hxarray_destroy(arr);
}

void test_hxarray_swap()
{
	puts("\tTesting hxarray_swap()");

	HxArray *arr = hxarray_create_of(char);
	hxarray_resize(arr, 3);

	char *data = arr->data;
	data[0] = 'a';
	data[1] = 'b';
	data[2] = 'c';

	char tmp;
	hxarray_swap(arr, &tmp, 0, 1);
	assert(data[0] == 'b');
	assert(data[1] == 'a');
	assert(data[2] == 'c');
	
	hxarray_swap(arr, &tmp, 1, 2);
	assert(data[0] == 'b');
	assert(data[1] == 'c');
	assert(data[2] == 'a');

	hxarray_swap(arr, &tmp, 0, 2);
	assert(data[0] == 'a');
	assert(data[1] == 'c');
	assert(data[2] == 'b');

	hxarray_destroy(arr);
}

void test_hxarray_clear()
{
	puts("\tTesting clear()");

	HxArray *arr = hxarray_create_of(int);

	hxarray_resize(arr, 3);
	assert(arr->length == 3);
	assert(arr->capacity == 3);

	hxarray_clear(arr);
	assert(arr->length == 0);
	assert(arr->capacity == 3);

	hxarray_clear(arr);
	assert(arr->length == 0);
	assert(arr->capacity == 3);

	hxarray_destroy(arr);
}

void test_hxarray_is_empty()
{
	puts("\tTesting isEmpty()");

	HxArray *arr = hxarray_create_of(bool);
	hxarray_resize(arr, 3);

	assert(!hxarray_is_empty(arr));

	hxarray_pop(arr);
	assert(arr->length == 2);
	assert(!hxarray_is_empty(arr));
	hxarray_pop(arr);
	assert(arr->length == 1);
	assert(!hxarray_is_empty(arr));
	hxarray_pop(arr);
	assert(arr->length == 0);
	assert(hxarray_is_empty(arr));
	assert(hxarray_resize(arr, 1));
	assert(!hxarray_is_empty(arr));

	hxarray_destroy(arr);
}

int main(void)
{
	puts("Testing HxArray...");

	test_hxarray_create();
	test_hxarray_increment_size();
	test_hxarray_resize();
	test_hxarray_push();
	test_hxarray_push_front();
	test_hxarray_insert();
	test_hxarray_pop();
	test_hxarray_pop_front();
	test_hxarray_erase();
	test_hxarray_set();
	test_hxarray_get();
	test_hxarray_at();
	test_hxarray_front();
	test_hxarray_back();
	test_hxarray_swap();
	test_hxarray_clear();
	test_hxarray_is_empty();

	puts("All tests passed");

	return 0;
}
