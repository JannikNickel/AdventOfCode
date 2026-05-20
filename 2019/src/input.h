#ifndef INPUT_H
#define INPUT_H

#include "common/str.h"
#include "common/vector.h"

typedef struct
{
	string raw;
	vector lines;
	const string* lines_c;
	size_t line_count;
} input;

input* input_from_file(const char* path);
void input_delete(input* input);

#endif
