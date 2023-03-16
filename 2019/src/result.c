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
	int len = strlen(s) + 1;
	char* data = malloc(sizeof(char) * len);
	memcpy(data, s, len);
	return (result) { .data = data };
}

result result_int(int64_t v)
{
	char* data = malloc(sizeof(char) * (snprintf(NULL, 0, "%" PRId64, v) + 1));
	sprintf(data, "%" PRId64, v);
	return (result) { .data = data };
}

result result_uint(uint64_t v)
{
	char* data = malloc(sizeof(char) * (snprintf(NULL, 0, "%" PRIu64, v) + 1));
	sprintf(data, "%" PRIu64, v);
	return (result) { .data = data };
}

result result_float(double v)
{
	char* data = malloc(sizeof(char) * (snprintf(NULL, 0, "%f", v) + 1));
	sprintf(data, "%f", v);
	return (result) { .data = data };
}

void result_print(result res)
{
	printf("%s", res.data != NULL ? res.data : "-");
}
