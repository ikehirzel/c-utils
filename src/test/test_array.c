#define HIRZEL_IMPLEMENT
#include <hirzel/array.h>

// standard library
#include <stdio.h>
#include <assert.h>

void testCreate()
{
	puts("\tTesting createHxArray()");

	HxArray array = createHxArray(sizeof(int));
	assert(array != NULL);
	assert(array->data == NULL);
	assert(array->element_size == sizeof(int));
	assert(array->length == 0);
	assert(array->capacity == 0);
	destroyHxArray(array);

	array = createHxArray(sizeof(double));
	assert(array != NULL);
	assert(array->data == NULL);
	assert(array->element_size == sizeof(double));
	assert(array->length == 0);
	assert(array->capacity == 0);
	destroyHxArray(array);
}

void testReserve()
{
	puts("\tTesting reserveHxArray()");
	HxArray arr = createHxArray(sizeof(int));
	assert(arr != NULL);

	assert(reserveHxArray(arr, 2));
	assert(arr->length == 0);
	assert(arr->data != NULL);
	assert(arr->capacity == 2);
	assert(arr->element_size == sizeof(int));

	int *data = arr->data;
	data[0] = 7;
	data[1] = 5;
	assert(data[0] == 7);
	assert(data[1] == 5);

	assert(reserveHxArray(arr, 4));
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

	destroyHxArray(arr);
}

void testResize()
{
	puts("\tTesting resizeHxArray()");
	HxArray arr = createHxArray(sizeof(int));
	assert(arr != NULL);
	
	assert(resizeHxArray(arr, 2));
	assert(arr->length == 2);
	assert(arr->capacity == 2);

	int *data = arr->data;
	data[0] = 2;
	data[1] = 600;
	assert(data[0] == 2);
	assert(data[1] == 600);

	assert(resizeHxArray(arr, 4));
	assert(arr->length == 4);
	assert(arr->capacity == 4);

	data = arr->data;
	data[2] = 5;
	data[3] = 984;
	assert(data[0] == 2);
	assert(data[1] == 600);
	assert(data[2] == 5);
	assert(data[3] == 984);

	assert(resizeHxArray(arr, 0));
	assert(arr->length == 0);
	assert(arr->capacity == 4);

	destroyHxArray(arr);
}

void testIncrement()
{
	puts("\tTesting incrementHxArray()");

	HxArray arr = createHxArray(sizeof(float));
	assert(arr != NULL);

	for (size_t i = 0; i < 10; ++i)
	{
		assert(incrementHxArray(arr));
		assert(arr->length == i + 1);
		assert(arr->capacity == i + 1);
		float *data = arr->data;
		data[arr->length - 1] = (float)(i * 3);
		assert(data[arr->length - 1] == (float)(i * 3));
	}	

	destroyHxArray(arr);
}

void testPush()
{
	puts("\tTesting pushHxArray()");

	HxArray arr = createHxArray(sizeof(int));
	assert(arr != NULL);	

	int value = 3;
	assert(pushHxArray(arr, &value));

	int *data = arr->data;

	assert(arr->length == 1);
	assert(arr->capacity == 1);
	assert(data[0] == 3);

	value = 4;
	assert(pushHxArray(arr, &value));
	data = arr->data;

	assert(arr->length == 2);
	assert(arr->capacity == 2);
	assert(data[0] == 3 && data[1] == 4);

	destroyHxArray(arr);
}

void testPushFront()
{
	puts("\tTesting pushFrontHxArray()");

	HxArray arr = createHxArray(sizeof(float));
	assert(arr != NULL);
	reserveHxArray(arr, 1);

	float value = 6.8f;
	assert(pushFrontHxArray(arr, &value));
	float *data = arr->data;

	assert(arr->length == 1);
	assert(arr->capacity == 1);
	assert(data[0] == 6.8f);

	value = 2.7f;
	assert(pushFrontHxArray(arr, &value));
	data = arr->data;
	assert(arr->length == 2);
	assert(arr->capacity == 2);
	assert(data[0] == 2.7f);
	assert(data[1] == 6.8f);

	value = 8.0f;
	assert(pushFrontHxArray(arr, &value));
	data = arr->data;
	assert(arr->length == 3);
	assert(arr->capacity == 3);
	assert(data[0] == 8.0f);
	assert(data[1] == 2.7f);
	assert(data[2] == 6.8f);

	destroyHxArray(arr);
}

void testInsert()
{
	puts("\tTesting insertHxArray()");

	HxArray arr = createHxArray(sizeof(double));
	assert(arr != NULL);

	double value = 3.2;
	insertHxArray(arr, 0, &value);
	double *data = arr->data;

	assert(arr->length == 1);
	assert(arr->capacity == 1);
	assert(data[0] == 3.2);

	value = 5.75;
	insertHxArray(arr, 1, &value);
	data = arr->data;

	assert(arr->length == 2);
	assert(arr->capacity == 2);
	assert(data[0] == 3.2);
	assert(data[1] == 5.75);

	value = 2.345;
	insertHxArray(arr, 0, &value);
	data = arr->data;

	assert(arr->length == 3);
	assert(arr->capacity == 3);
	assert(data[0] == 2.345);
	assert(data[1] == 3.2);
	assert(data[2] == 5.75);

	value = 1.0;
	insertHxArray(arr, 1, &value);
	data = arr->data;

	assert(arr->length == 4);
	assert(arr->capacity == 4);
	assert(data[0] == 2.345);
	assert(data[1] == 1.0);
	assert(data[2] == 3.2);
	assert(data[3] == 5.75);

	destroyHxArray(arr);
}

void testPop()
{
	puts("\tTesting popHxArray()");

	HxArray arr = createHxArray(sizeof(char));
	assert(arr != NULL);
	resizeHxArray(arr, 3);

	popHxArray(arr);
	assert(arr->length == 2);
	assert(arr->capacity == 3);
	
	popHxArray(arr);
	assert(arr->length == 1);
	assert(arr->capacity == 3);
	
	popHxArray(arr);
	assert(arr->length == 0);
	assert(arr->capacity == 3);

	popHxArray(arr);
	assert(arr->length == 0);
	assert(arr->capacity == 3);

	destroyHxArray(arr);
}

void testPopFront()
{
	puts("\tTesting popFrontHxArray()");

	HxArray arr = createHxArray(sizeof(char));
	assert(arr != NULL);
	resizeHxArray(arr, 3);

	char *data = arr->data;
	data[0] = 'a';
	data[1] = 'b';
	data[2] = 'c';

	popFrontHxArray(arr);
	assert(arr->length == 2);
	assert(arr->capacity == 3);
	assert(data[0] == 'b');

	popFrontHxArray(arr);
	assert(arr->length == 1);
	assert(arr->capacity == 3);
	assert(data[0] == 'c');

	popFrontHxArray(arr);
	assert(arr->length == 0);
	assert(arr->capacity == 3);

	destroyHxArray(arr);
}

void testErase()
{
	puts("\tTesting eraseHxArray()");

	HxArray arr = createHxArray(sizeof(char));
	assert(arr != NULL);
	resizeHxArray(arr, 3);

	char *data = arr->data;
	data[0] = 'a';
	data[1] = 'b';
	data[2] = 'c';

	eraseHxArray(arr, 1);
	assert(arr->length == 2);
	assert(arr->capacity == 3);
	assert(data[0] == 'a');
	assert(data[1] == 'c');

	eraseHxArray(arr, 0);
	assert(arr->length == 1);
	assert(arr->capacity == 3);
	assert(data[0] == 'c');
	
	eraseHxArray(arr, 0);
	assert(arr->length == 0);
	assert(arr->capacity == 3);

	destroyHxArray(arr);
}

void testSet()
{
	puts("\tTesting setHxArray()");

	int a = 0;
	int b = 1;
	int c = 2;

	HxArray arr = createHxArray(sizeof(int*));
	assert(arr != NULL);
	resizeHxArray(arr, 3);

	int **data = arr->data;
	data[0] = &a;
	data[1] = &b;
	data[2] = &c;

	int *ptr = &a;
	setHxArray(arr, 1, &ptr);
	assert(data[1] == &a);
	assert(data[0] == data[1]);
	
	setHxArray(arr, 2, &ptr);
	assert(data[2] == &a);
	assert(data[0] == data[1]);
	assert(data[1] == data[2]);

	ptr = &c;
	setHxArray(arr, 0, &ptr);
	assert(data[0] == &c);

	assert(arr->length == 3);
	assert(arr->capacity == 3);

	destroyHxArray(arr);
}

void testGet()
{
	puts("\tTesting getHxArray()");

	HxArray arr = createHxArray(sizeof(unsigned long long));
	assert(arr != NULL);
	resizeHxArray(arr, 3);

	unsigned long long *data = arr->data;
	data[0] = 123;
	data[1] = 456;
	data[2] = 789;

	unsigned long long value;
	getHxArray(arr, &value, 0);
	assert(value == 123);

	getHxArray(arr, &value, 1);
	assert(value == 456);

	getHxArray(arr, &value, 2);
	assert(value == 789);

	destroyHxArray(arr);
}

void testAt()
{
	puts("\tTesting atHxArray()");

	HxArray arr = createHxArray(sizeof(unsigned long long));
	resizeHxArray(arr, 3);

	unsigned long long *data = arr->data;
	data[0] = 123;
	data[1] = 456;
	data[2] = 789;

	unsigned long long *ptr;
	ptr = atHxArray(arr, 0);
	assert(*ptr == 123);

	ptr = atHxArray(arr, 1);
	assert(*ptr == 456);

	ptr = atHxArray(arr, 2);
	assert(*ptr == 789);

	destroyHxArray(arr);
}

void testFront()
{
	puts("\tTesting frontHxArray()");

	HxArray arr = createHxArray(sizeof(char));
	resizeHxArray(arr, 3);

	char *data = arr->data;
	data[0] = 'a';
	data[1] = 'b';
	data[2] = 'c';

	char *front = frontHxArray(arr);
	assert(*front == 'a');

	char tmp = 'h';
	pushFrontHxArray(arr, &tmp);
	front = frontHxArray(arr);
	assert(*front == 'h');

	popFrontHxArray(arr);
	front = frontHxArray(arr);
	assert(*front == 'a');

	popFrontHxArray(arr);
	front = frontHxArray(arr);
	assert(*front == 'b');

	popFrontHxArray(arr);
	front = frontHxArray(arr);
	assert(*front == 'c');

	destroyHxArray(arr);
}

void testBack()
{
	puts("\tTesting backHxArray()");

	HxArray arr = createHxArray(sizeof(char));
	resizeHxArray(arr, 3);

	char *data = arr->data;
	data[0] = 'a';
	data[1] = 'b';
	data[2] = 'c';

	char *back = backHxArray(arr);
	assert(*back == 'c');

	char tmp = 'h';
	pushHxArray(arr, &tmp);
	back = backHxArray(arr);
	assert(*back == 'h');

	popHxArray(arr);
	back = backHxArray(arr);
	assert(*back == 'c');

	popHxArray(arr);
	back = backHxArray(arr);
	assert(*back == 'b');

	popHxArray(arr);
	back = backHxArray(arr);
	assert(*back == 'a');

	destroyHxArray(arr);
}

void testSwap()
{
	puts("\tTesting swapHxArray()");

	HxArray arr = createHxArray(sizeof(char));
	resizeHxArray(arr, 3);

	char *data = arr->data;
	data[0] = 'a';
	data[1] = 'b';
	data[2] = 'c';

	char tmp;
	swapHxArray(arr, &tmp, 0, 1);
	assert(data[0] == 'b');
	assert(data[1] == 'a');
	assert(data[2] == 'c');
	
	swapHxArray(arr, &tmp, 1, 2);
	assert(data[0] == 'b');
	assert(data[1] == 'c');
	assert(data[2] == 'a');

	swapHxArray(arr, &tmp, 0, 2);
	assert(data[0] == 'a');
	assert(data[1] == 'c');
	assert(data[2] == 'b');

	destroyHxArray(arr);
}

void testClear()
{
	puts("\tTesting clearHxArray()");

	HxArray arr = createHxArray(sizeof(int));

	resizeHxArray(arr, 3);
	assert(arr->length == 3);
	assert(arr->capacity == 3);

	clearHxArray(arr);
	assert(arr->length == 0);
	assert(arr->capacity == 3);

	clearHxArray(arr);
	assert(arr->length == 0);
	assert(arr->capacity == 3);

	destroyHxArray(arr);
}

void testIsEmpty()
{
	puts("\tTesting isEmptyHxArray()");

	HxArray arr = createHxArray(sizeof(bool));
	resizeHxArray(arr, 3);

	assert(!isEmptyHxArray(arr));

	popHxArray(arr);
	assert(arr->length == 2);
	assert(!isEmptyHxArray(arr));
	popHxArray(arr);
	assert(arr->length == 1);
	assert(!isEmptyHxArray(arr));
	popHxArray(arr);
	assert(arr->length == 0);
	assert(isEmptyHxArray(arr));
	assert(resizeHxArray(arr, 1));
	assert(!isEmptyHxArray(arr));

	destroyHxArray(arr);
}

int main(void)
{
	puts("Testing HxArray...");

	testCreate();
	testIncrement();
	testResize();
	testPush();
	testPushFront();
	testInsert();
	testPop();
	testPopFront();
	testErase();
	testSet();
	testGet();
	testAt();
	testFront();
	testBack();
	testSwap();
	testClear();
	testIsEmpty();

	puts("All tests passed");

	return 0;
}
