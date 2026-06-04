#ifndef RESULT_H
#define RESULT_H

#include <stdint.h>

typedef struct
{
	char* data;
} result;

result result_none();
void result_delete(result res);

result result_string(const char* s);
result result_heap_string(char* s);
result result_int(int64_t v);
result result_uint(uint64_t v);
result result_float(double v);
result result_fmt(const char* fmt, ...);

void result_print(result res);

#endif
