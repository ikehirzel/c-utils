#ifndef HIRZEL_UTIL_FILE_H
#define HIRZEL_UTIL_FILE_H
#include <stdio.h>

extern char* hzl_file_read(const char* file, const char* options);
inline char* hzl_file_read_raw(const char* file)
{
	return hzl_file_read(file, "rb");
}
inline char* hzl_file_read_text(const char* file)
{
	return hzl_file_read(file, "r");
}

/**
 * @brief Reads line from file stream till newline
 * 
 * The memory for the stream is dynamically allocated and must be freed
 * 
 * @param	stream	file stream
 * @return	pointer to malloc'd string
 */
extern char* hzl_file_read_line(FILE* stream);
extern char** hzl_file_read_lines(const char* file);
extern void hzl_file_free_lines(char** lines);
#endif

#define HIRZEL_UTIL_FILE_I

#ifdef HIRZEL_UTIL_FILE_I
#undef HIRZEL_UTIL_FILE_I

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* hzl_file_read(const char* file, const char* options)
{
	// open file stream
	FILE* stream = fopen(file, options);
	// going to end of stream
	fseek(stream, 0, SEEK_END);
	// getting size of stream
	size_t size = ftell(stream);
	// rewinding stream
	fseek(stream, 0, SEEK_SET);
	// allocating string
	char* str = malloc(size + 1);
	// reading file
	size_t readc = fread(str, sizeof(char), size, stream);
	// null terminating string
	str[readc] = 0;
	// closing file
	fclose(stream);
	return str;
}

char **hzl_file_read_lines(const char *file)
{
	// open file stream
	FILE *stream = fopen(file, "r");
	// safeguard file read failure
	if (!stream) return NULL;
	// setting eof flag if empty
	char c = getc(stream);
	rewind(stream);
	// tmp buffer to store lines
	char *buf[32];
	int bufc = 0;
	// null terminated lines array
	char **lines = NULL;
	// pointing iterator to base of stack
	char* cur = buf[0];
	// initialized to 1 for null termination
	size_t linec = 1;

	do
	{
		cur = (buf[bufc++] = hzl_file_read_line(stream));
		printf("cur: %s\n", cur);
		// buf is full or line is empty, store buffer
		if (bufc >= 32 || !cur)
		{
			// allocate more memory (previous line count + lines in stack - 1(if line was invalid))
			char **tmp = malloc((linec + bufc - !cur) * sizeof(char*));
			printf("Allocating %zu\n", linec + bufc - !cur);
			// null terminate buffer
			tmp[linec + bufc - 1] = NULL;
			// if there is data in output buffer
			if (lines)
			{	
				// copy it
				memcpy(tmp, lines, (linec - 1) * sizeof(char*));
				// free old buffer
				free(lines);
			}
			// point to new buffer
			lines = tmp;
			// copy stack into output buffer
			memcpy(lines + linec - 1, buf, bufc * sizeof(char*));
			// increase output buffer by size of stack
			linec += bufc;
			// rebase iterator
			bufc = 0;
		}
	}
	while(cur);

	// closing file
	fclose(stream);

	// return buffer
	return lines;
}

char *hzl_file_read_line(FILE *stream)
{
	// checking if eof
	char c = getc(stream);
	// returning null on fail
	if (c == EOF || !stream) return NULL;
	// putting char back in stream
	ungetc(c, stream);
	// temporary buffer
	char buf[128];
	// size of output buffer
	size_t osize = 0;
	// output buffer pointer
	char *line = NULL;

	do
	{
		// reset flag byte
		buf[126] = 0;
		// read line into buf
		fgets(buf, 128, stream);
		// get len of read info
		size_t len = strlen(buf);
		// expand size of output buffer
		osize += (len + 1) * sizeof(char);
		// allocate space for output buffer
		char* tmp = malloc(osize);
		// if we have data in ouput buffer
		if (line)
		{
			// copy to over to new, bigger buffer
			strcpy(tmp, line);
			free(line);
		}
		else
		{
			// prepare new buffer for concatenation
			tmp[0] = 0;
		}
		// store output buffer in line
		line = tmp;
		// copy temp buffer into output buffer
		strcat(line, buf);
	}
	// continue if data was not a complete line
	while (buf[126] != 0 && buf[126] != '\n');

	// return output buffer
	return line;
}

void hzl_file_free_lines(char **lines)
{
	for (char **pos = lines; *pos != NULL; ++pos)
	{
		free(*pos);
	}
	free(lines);
	char *linesp = &lines;
	*linesp = NULL;
}

#endif
