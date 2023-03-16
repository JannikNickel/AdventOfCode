#ifndef INPUT_H
#define INPUT_H

#include "common/str.h"
#include "common/vector.h"

typedef struct
{
	const string raw;
	const vector lines;
} input;

input* input_from_file(const char* path);
void input_delete(input* input);

#endif
