// standard library
#include <stdio.h>
#include <stdlib.h>

int runTest(const char *name)
{
	int return_value = system(name);

	if (return_value == -1)
		printf("Failed to run test: %s\n", name);

	return return_value;
}

int main()
{
	const char *tests[] =
	{
		"./test_array",
		"./test_table"
	};
	
	size_t test_count = sizeof(tests) / sizeof(tests[0]);
	int fail_count = 0;

	for (size_t i = 0; i < test_count; ++i)
	{		
		if (runTest(tests[i]))
			fail_count += 1;
		putchar('\n');
	}

	
	if (!fail_count)
		puts("All tests succeeded");
	else
		printf("\033[31m%d tests failed\033[0m\n", fail_count);

	return fail_count;
}