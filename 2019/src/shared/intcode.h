#ifndef INTCODE_H
#define INTCODE_H

#include "../common/vector.h"
#include "../common/str.h"

//Use intcode_parse to parse the input. Both input and output are optional, but should be of type int64_t if not NULL. The input vector will be consumed
void intcode_run(vector* code, vector* input, vector* output);
//Simplified version to run intcode with less setup work required. Returns a vector of type int64_t which contains the output
vector intcode_run_simple(string code_str, int64_t* input, size_t input_len);
//Parse a comma separated string as intcode
vector intcode_parse(string source);

#endif
