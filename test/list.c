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
	hxlist_int_t *list = hxlist_int_create();

	hxlist_int_destroy(list);
	// IntList i;
	return 0;
}
