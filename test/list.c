#define HIRZEL_UTIL_LIST_T int
#define HIRZEL_UTIL_LIST_I
#include <hirzel/util/list.h>

#include <stdio.h>


int main(void)
{
	HXLIST_TNAME t;
	t.len = 0;
	printf("%zu\n", t.len);
	struct HXLIST_SNAME s;
	s.len = 3;
	printf("%zu\n", s.len);
	return 0;
}
