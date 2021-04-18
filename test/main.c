// windows deprecated warnings
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#define HIRZEL_UTIL_FILE_I
#include <hirzel/util/file.h>

void test(int *p)
{
	*p = 0;
}

int main()
{
	puts("Reading lines");
	char** lines = hzl_file_read_lines("./file.txt");
	puts("Done reading");

	int i = 0;
	for (const char **pos = (const char**)lines; *pos != NULL; ++pos)
	{
		printf("%d: %s\n", i++, *pos);
	}

	hzl_file_free_lines(lines);
	printf("Val: %p\n", lines);
	return 0;
}
