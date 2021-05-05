#define HIRZEL_UTIL_LIST_T int
#define HIRZEL_UTIL_LIST_I
#include <hirzel/util/list.h>
typedef HXL_STRUCT_NAME(int) IntList;

#include <stdio.h>


int main(void)
{
	printf("Struct name:  %s\n", STR(HXL_STRUCT));
	printf("Element Type: %s\n", STR(HXL_TYPE));
	// IntList i;
	return 0;
}
