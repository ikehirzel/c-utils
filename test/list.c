#define HIRZEL_UTIL_LIST_T int
#define HIRZEL_UTIL_LIST_I
#include <hirzel/util/list.h>

#include <stdio.h>
#include <assert.h>

int main(void)
{
	printf("Struct name:  %s\n", STR(HXL_STRUCT));
	printf("Element Type: %s\n", STR(HXL_TYPE));
	hxlist_int_t *list = hxlist_int_create();

	hxlist_int_destroy(list);
	// IntList i;
	return 0;
}
