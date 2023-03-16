#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

input* input_from_file(const char* path)
{
	FILE* file = fopen(path, "r");
	assert(file);

	fseek(file, 0, SEEK_END);
	size_t size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* content = malloc(size + 1);
	size = fread(content, sizeof(char), size, file);
	content[size] = '\0';
	fclose(file);

	string raw = string_from(content);
	string l = string_replace(raw, "\r\n", "\n");

	input* in = malloc(sizeof(input));
	*(string*)&in->raw = raw;
	*(vector*)&in->lines = string_split_all_cstr(l, "\n");

	free(content);
	string_delete(&l);

	return in;
}

void input_delete(input* input)
{
	string_delete(&input->raw);
	vector_delete(&input->lines, string_dealloc);
}
