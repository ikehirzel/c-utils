#define HIRZEL_UTIL_LIST_T int
#define HIRZEL_UTIL_LIST_I
#include <hirzel/util/list.h>

// preprocessor assertations
#if defined(HXL_STRUCT) || defined(HXL_TYPEDEF) || defined(HXL_TYPE)\
|| defined(HXL_BASE) || defined(HIRZEL_UTIL_LIST_T) || defined(HIRZEL_UTIL_LIST_I)
#error Utility macros need to have their definitions removed
#endif

#include <stdio.h>
#include <assert.h>
#include <string.h>

int main(void)
{
	// testing list creation
	hxlist_int_t *list = hxlist_int_create();
	assert(list != NULL);
	assert(list->data != NULL);
	assert(list->len == 0);

	// testing list pushing
	hxlist_int_push(list, 3);
	assert(list->len == 1);
	assert(hxlist_get(list, 0) == 3);

	hxlist_int_push(list, 4);
	assert(list->len == 2);
	assert(hxlist_get(list, 1) == 4);

	hxlist_int_push(list, 5);
	assert(list->len == 3);
	assert(hxlist_get(list, 2) == 5);

	// testing list setting
	hxlist_set(list, 2, 7);
	assert(hxlist_get(list, 2) == 7);

	printf("Data[2]: %d\n", hxlist_get(list, 2));

	hxlist_int_destroy(list);
	// IntList i;
	return 0;
}
