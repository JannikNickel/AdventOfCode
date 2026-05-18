#include "result.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

result result_none()
{
	return (result) { .data = NULL };
}

void result_delete(result res)
{
	free(res.data);
}

result result_string(const char* s)
{
	size_t len = strlen(s) + 1;
	char* data = malloc(sizeof(char) * len);
	memcpy(data, s, len);
	return (result) { .data = data };
}

result result_heap_string(char* s)
{
	return (result) { .data = s };
}

result result_int(int64_t v)
{
	int size = snprintf(NULL, 0, "%" PRId64, v) + 1;
	char* data = malloc(size);
	snprintf(data, size, "%" PRId64, v);
	return (result) { .data = data };
}

result result_uint(uint64_t v)
{
	int size = snprintf(NULL, 0, "%" PRIu64, v) + 1;
	char* data = malloc(size);
	snprintf(data, size, "%" PRIu64, v);
	return (result) { .data = data };
}

result result_float(double v)
{
	int size = snprintf(NULL, 0, "%f", v) + 1;
	char* data = malloc(size);
	snprintf(data, size, "%f", v);
	return (result) { .data = data };
}

void result_print(result res)
{
	printf("%s", res.data != NULL ? res.data : "-");
}
