#include <hirzel/array.h>

HXARRAY_DECLARE(int, IntArray)
HXARRAY_DEFINE(int, IntArray)

// standard library
#include <stdio.h>
#include <assert.h>

void test_IntArray_create()
{
	puts("\tTesting IntArray_create()");

	IntArray arr = IntArray_init();

	assert(arr.buffer == NULL);
	assert(arr.length == 0);
	assert(arr.capacity == 0);
}

void test_IntArray_reserve()
{
	puts("\tTesting IntArray_reserve()");
	IntArray arr = IntArray_init();

	assert(IntArray_reserve(&arr, 2));
	assert(arr.length == 0);
	assert(arr.buffer != NULL);
	assert(arr.capacity == 2);

	arr.buffer[0] = 7;
	arr.buffer[1] = 5;
	assert(arr.buffer[0] == 7);
	assert(arr.buffer[1] == 5);

	assert(IntArray_reserve(&arr, 4));
	assert(arr.length == 0);
	assert(arr.buffer != NULL);
	assert(arr.capacity == 4);

	arr.buffer[2] = 76;
	arr.buffer[3] = 87;
	assert(arr.buffer[0] == 7);
	assert(arr.buffer[1] == 5);
	assert(arr.buffer[2] == 76);
	assert(arr.buffer[3] == 87);

	IntArray_free(&arr);
}

void test_IntArray_resize()
{
	puts("\tTesting IntArray_resize()");
	IntArray arr = IntArray_init();
	
	assert(IntArray_resize(&arr, 2));
	assert(arr.length == 2);
	assert(arr.capacity == 2);
	
	arr.buffer[0] = 2;
	arr.buffer[1] = 600;
	assert(arr.buffer[0] == 2);
	assert(arr.buffer[1] == 600);

	assert(IntArray_resize(&arr, 4));
	assert(arr.length == 4);
	assert(arr.capacity == 4);

	arr.buffer[2] = 5;
	arr.buffer[3] = 984;
	assert(arr.buffer[0] == 2);
	assert(arr.buffer[1] == 600);
	assert(arr.buffer[2] == 5);
	assert(arr.buffer[3] == 984);

	assert(IntArray_resize(&arr, 0));
	assert(arr.length == 0);
	assert(arr.capacity == 4);

	IntArray_free(&arr);
}

void test_IntArray_increment_size()
{
	puts("\tTesting IntArray_increment_size()");

	IntArray arr = IntArray_init();
	
	for (int i = 0; i < 10; ++i)
	{
		assert(IntArray_increment_size(&arr));
		assert(arr.length == (size_t)(i + 1));
		assert(arr.capacity == (size_t)(i + 1));
		arr.buffer[arr.length - 1] = i * 3;
		assert(arr.buffer[arr.length - 1] == i * 3);
	}	

	IntArray_free(&arr);
}

void test_IntArray_push()
{
	puts("\tTesting IntArray_push()");

	IntArray arr = IntArray_init();

	int values[] = { 1, 8, 6, 6, 2 };
	int count = sizeof(values) / sizeof(*values);

	assert(IntArray_reserve(&arr, count));

	for (int i = 0; i < count; ++i)
	{
		IntArray_push(&arr, i);
	}

	for (int i = 0; i < count; ++i)
	{
		assert(arr.buffer[i] == values[i]);
	}

	IntArray_clear(&arr);

	for (int i = 0; i < count; ++i)
	{
		IntArray_push_ptr(&arr, &i);
	}

	for (int i = 0; i < count; ++i)
	{
		assert(arr.buffer[i] == values[i]);
	}

	IntArray_free(&arr);
}

void test_IntArray_pushf()
{
	puts("\tTesting IntArray_pushf()");

	IntArray arr = IntArray_init();
	
	IntArray_reserve(&arr, 1);

	assert(IntArray_pushf(&arr, 6));

	assert(arr.length == 1);
	assert(arr.capacity == 1);
	assert(arr.buffer[0] == 6);

	assert(IntArray_pushf(&arr, 2));
	assert(arr.length == 2);
	assert(arr.capacity == 2);
	assert(arr.buffer[0] == 2);
	assert(arr.buffer[1] == 6);

	assert(IntArray_pushf(&arr, 8));	
	assert(arr.length == 3);
	assert(arr.capacity == 3);
	assert(arr.buffer[0] == 8);
	assert(arr.buffer[1] == 2);
	assert(arr.buffer[2] == 6);

	IntArray_free(&arr);
}

void test_IntArray_insert()
{
	puts("\tTesting IntArray_insert()");

	IntArray arr = IntArray_init();
	
	int value = 3;
	IntArray_insert(&arr, 0, &value);	

	assert(arr.length == 1);
	assert(arr.capacity == 1);
	assert(arr.buffer[0] == 3);

	value = 5;
	IntArray_insert(&arr, 1, &value);	

	assert(arr.length == 2);
	assert(arr.capacity == 2);
	assert(arr.buffer[0] == 3.2);
	assert(arr.buffer[1] == 5.75);

	value = 2.345;
	IntArray_insert(&arr, 0, &value);

	assert(arr.length == 3);
	assert(arr.capacity == 3);
	assert(arr.buffer[0] == 2.345);
	assert(arr.buffer[1] == 3.2);
	assert(arr.buffer[2] == 5.75);

	value = 1.0;
	IntArray_insert(&arr, 1, &value);

	assert(arr.length == 4);
	assert(arr.capacity == 4);
	assert(arr.buffer[0] == 2.345);
	assert(arr.buffer[1] == 1.0);
	assert(arr.buffer[2] == 3.2);
	assert(arr.buffer[3] == 5.75);

	IntArray_free(&arr);
}

void test_IntArray_pop()
{
	puts("\tTesting IntArray_pop()");

	IntArray arr = IntArray_init();
	
	IntArray_resize(&arr, 3);

	IntArray_pop(&arr);
	assert(arr.length == 2);
	assert(arr.capacity == 3);
	
	IntArray_pop(&arr);
	assert(arr.length == 1);
	assert(arr.capacity == 3);
	
	IntArray_pop(&arr);
	assert(arr.length == 0);
	assert(arr.capacity == 3);

	IntArray_pop(&arr);
	assert(arr.length == 0);
	assert(arr.capacity == 3);

	IntArray_free(&arr);
}

void test_IntArray_popf()
{
	puts("\tTesting IntArray_popf()");

	IntArray arr = IntArray_init();
	
	IntArray_resize(&arr, 3);

	arr.buffer[0] = 'a';
	arr.buffer[1] = 'b';
	arr.buffer[2] = 'c';

	IntArray_popf(&arr);
	assert(arr.length == 2);
	assert(arr.capacity == 3);
	assert(arr.buffer[0] == 'b');

	IntArray_popf(&arr);
	assert(arr.length == 1);
	assert(arr.capacity == 3);
	assert(arr.buffer[0] == 'c');

	IntArray_popf(&arr);
	assert(arr.length == 0);
	assert(arr.capacity == 3);

	IntArray_free(&arr);
}

void test_IntArray_erase()
{
	puts("\tTesting IntArray_erase()");

	IntArray arr = IntArray_init();
	
	IntArray_resize(&arr, 3);

	arr.buffer[0] = 'a';
	arr.buffer[1] = 'b';
	arr.buffer[2] = 'c';

	IntArray_erase(&arr, 1);
	assert(arr.length == 2);
	assert(arr.capacity == 3);
	assert(arr.buffer[0] == 'a');
	assert(arr.buffer[1] == 'c');

	IntArray_erase(&arr, 0);
	assert(arr.length == 1);
	assert(arr.capacity == 3);
	assert(arr.buffer[0] == 'c');
	
	IntArray_erase(&arr, 0);
	assert(arr.length == 0);
	assert(arr.capacity == 3);

	IntArray_free(&arr);
}

void test_IntArray_set()
{
	puts("\tTesting IntArray_set()");

	IntArray arr = IntArray_init();
	
	assert(IntArray_resize(&arr, 3));

	arr.buffer[0] = 0;
	arr.buffer[1] = 1;
	arr.buffer[2] = 2;

	IntArray_set(&arr, 1, 0);
	assert(arr.buffer[1] == 0);
	assert(arr.buffer[0] == arr.buffer[1]);
	
	IntArray_set(&arr, 2, 0);
	assert(arr.buffer[2] == 0);
	assert(arr.buffer[0] == arr.buffer[1]);
	assert(arr.buffer[1] == arr.buffer[2]);

	IntArray_set(&arr, 0, 2);
	assert(arr.buffer[0] == 2);

	assert(arr.length == 3);
	assert(arr.capacity == 3);

	IntArray_free(&arr);
}

void test_IntArray_get()
{
	puts("\tTesting IntArray_get()");

	IntArray arr = IntArray_init();
	
	IntArray_resize(&arr, 3);

	arr.buffer[0] = 123;
	arr.buffer[1] = 456;
	arr.buffer[2] = 789;

	assert(IntArray_get(&arr, 0) == 123);
	assert(IntArray_get(&arr, 1) == 456);	
	assert(IntArray_get(&arr, 2) == 789);

	IntArray_free(&arr);
}

void test_IntArray_at()
{
	puts("\tTesting IntArray_at()");

	IntArray arr = IntArray_init();
	IntArray_resize(&arr, 3);

	arr.buffer[0] = 123;
	arr.buffer[1] = 456;
	arr.buffer[2] = 789;

	int *ptr = IntArray_get_ptr(&arr, 0);
	assert(*ptr == 123);

	ptr = IntArray_get_ptr(&arr, 1);
	assert(*ptr == 456);

	ptr = IntArray_get_ptr(&arr, 2);
	assert(*ptr == 789);

	IntArray_free(&arr);
}

void test_IntArray_front()
{
	puts("\tTesting IntArray_front()");

	IntArray arr = IntArray_init();
	IntArray_resize(&arr, 3);

	arr.buffer[0] = 'a';
	arr.buffer[1] = 'b';
	arr.buffer[2] = 'c';

	int front = IntArray_front(&arr);
	assert(front == 'a');

	IntArray_pushf(&arr, 'h');
	front = IntArray_front(&arr);
	assert(front == 'h');

	IntArray_popf(&arr);
	front = IntArray_front(&arr);
	assert(front == 'a');

	IntArray_popf(&arr);
	front = IntArray_front(&arr);
	assert(front == 'b');

	IntArray_popf(&arr);
	front = IntArray_front(&arr);
	assert(front == 'c');

	IntArray_free(&arr);
}

void test_IntArray_back()
{
	puts("\tTesting IntArray_back()");

	IntArray arr = IntArray_init();
	IntArray_resize(&arr, 3);

	arr.buffer[0] = 'a';
	arr.buffer[1] = 'b';
	arr.buffer[2] = 'c';

	int back = IntArray_back(&arr);
	assert(back == 'c');

	IntArray_push(&arr, 'h');
	back = IntArray_back(&arr);
	assert(back == 'h');

	IntArray_pop(&arr);
	back = IntArray_back(&arr);
	assert(back == 'c');

	IntArray_pop(&arr);
	back = IntArray_back(&arr);
	assert(back == 'b');

	IntArray_pop(&arr);
	back = IntArray_back(&arr);
	assert(back == 'a');

	IntArray_free(&arr);
}

void test_IntArray_swap()
{
	puts("\tTesting IntArray_swap()");

	IntArray arr = IntArray_init();
	IntArray_resize(&arr, 3);

	arr.buffer[0] = 'a';
	arr.buffer[1] = 'b';
	arr.buffer[2] = 'c';

	IntArray_swap(&arr, 0, 1);
	assert(arr.buffer[0] == 'b');
	assert(arr.buffer[1] == 'a');
	assert(arr.buffer[2] == 'c');
	
	IntArray_swap(&arr, 1, 2);
	assert(arr.buffer[0] == 'b');
	assert(arr.buffer[1] == 'c');
	assert(arr.buffer[2] == 'a');

	IntArray_swap(&arr, 0, 2);
	assert(arr.buffer[0] == 'a');
	assert(arr.buffer[1] == 'c');
	assert(arr.buffer[2] == 'b');

	IntArray_free(&arr);
}

void test_IntArray_clear()
{
	puts("\tTesting IntArray_clear()");

	IntArray arr = IntArray_init();

	IntArray_resize(&arr, 3);
	assert(arr.length == 3);
	assert(arr.capacity == 3);

	IntArray_clear(&arr);
	assert(arr.length == 0);
	assert(arr.capacity == 3);

	IntArray_clear(&arr);
	assert(arr.length == 0);
	assert(arr.capacity == 3);

	IntArray_free(&arr);
}

int main(void)
{
	puts("Testing IntArray...");

	test_IntArray_create();
	test_IntArray_increment_size();
	test_IntArray_resize();
	test_IntArray_push();
	test_IntArray_pushf();
	test_IntArray_insert();
	test_IntArray_pop();
	test_IntArray_popf();
	test_IntArray_erase();
	test_IntArray_set();
	test_IntArray_get();
	test_IntArray_at();
	test_IntArray_front();
	test_IntArray_back();
	test_IntArray_swap();
	test_IntArray_clear();

	puts("All tests passed");

	return 0;
}
