// standard library
#include <stdio.h>
#include <stdlib.h>

int runTest(const char *name)
{
	int return_value = system(name);

	if (return_value == -1)
	{
		printf("failed to run test: %s\n", name);
	}
	else
	{
		if (return_value)
		{
			printf("test failed: %s\n", name);
		}
		else
		{
			printf("test succeeded: %s\n", name);
		}
	}

	return return_value;
}

int main()
{
	const char *tests[] =
	{
		"test_list",
		"test_table"
	};
	
	size_t test_count = sizeof(tests) / sizeof(tests[0]);
	int fail_count = 0;

	for (size_t i = 0; i < test_count; ++i)
	{
		if (runTest(tests[i]))
			fail_count += 1;
	}

	if (!fail_count)
		puts("All tests succeeded");

	return fail_count;
}