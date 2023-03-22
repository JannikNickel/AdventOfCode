#ifndef INTCODE_H
#define INTCODE_H

#include "../common/vector.h"
#include "../common/str.h"

typedef enum
{
	IC_END,
	IC_INP_MISSING,
	IC_INP_BREAK,
	IC_OUT_BREAK
} intcode_res_type;

typedef struct
{
	intcode_res_type type;
	size_t inst_ptr;
	size_t rel_base;
} intcode_run_result;

//Use intcode_parse to parse the input. Both input and output are optional, but should be of type int64_t if not NULL. The input vector will be consumed
intcode_run_result intcode_run(vector* code, vector* input, vector* output);
//Continue or start to run intcode. Both input and output are optional, but should be of type int64_t if not NULL. The input vector will be consumed
//'inst_ptr' can be used to continue previous intcode, 'break_inp' flag will stop execution before an input instruction, 'break_out' flag will stop execution after an output instruction
intcode_run_result intcode_continue(vector* code, vector* input, vector* output, size_t inst_ptr, size_t rel_base, bool break_inp, bool break_out);
//Simplified version to run intcode with less setup work required. Returns a vector of type int64_t which contains the output
vector intcode_run_simple(string code_str, int64_t* input, size_t input_len);
//Simplified version to run intcode with less setup work required. Returns the last value of the output or 0 if none
int64_t intcode_run_simple_out(string code_str, int64_t* input, size_t input_len);
//Parse a comma separated string as intcode
vector intcode_parse(string source);

#endif