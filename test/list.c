#define HIRZEL_UTIL_LIST_T int
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
	assert(list->data == NULL);
	assert(list->len == 0);
	assert(hxlist_is_empty(list));

	// testing list pushing

	assert(hxlist_int_push(list, 3));
	assert(list->len == 1);
	assert(hxlist_get(list, 0) == 3);
	assert(hxlist_back(list) == 3);
	assert(hxlist_front(list) == 3);

	assert(hxlist_int_push(list, 4));
	assert(list->len == 2);
	assert(hxlist_get(list, 1) == 4);
	assert(hxlist_back(list) == 4);
	assert(hxlist_front(list) == 3);

	hxlist_int_pushf(list, 9);
	assert(list->len == 3);
	printf("front: %d\n", hxlist_front(list));
	assert(hxlist_front(list) == 9);


	// testing list inserting

	// testing list setting
	hxlist_set(list, 2, 7);
	assert(hxlist_get(list, 2) == 7);
	assert(hxlist_back(list) == 7);

	// testing list get

	printf("%zu\n", list->len);
	
	//_hxlist_push((char**)list, &i, sizeof(int));
	hxlist_int_push(list, 3);
	printf("Len: %zu\n", list->len);
	printf("back: %d\n", hxlist_back(list));

	// freeing data
	hxlist_destroy(list);
	
	return 0;
}
