#include "intcode.h"
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

typedef enum
{
	ADDRESS = 0,
	IMMEDIATE = 1,
	RELATIVE = 2
} param_mode;

typedef enum
{
	OP_ADD = 1,
	OP_MUL = 2,
	OP_INP = 3,
	OP_OUT = 4,
	OP_JIT = 5,
	OP_JIF = 6,
	OP_LTN = 7,
	OP_EQL = 8,
	OP_REL = 9,
	OP_END = 99
} op;

typedef struct
{
	op op;
	param_mode p0_mode;
	param_mode p1_mode;
	param_mode p2_mode;
} instruction;

static int inst_seg(int64_t code, int index)
{
	int size = (int)log10(code) + 1;
	if(index >= size)
	{
		return 0;
	}
	return (code / (uint32_t)pow(10, index)) % 10;
}

static instruction parse_inst(int64_t code)
{
	return (instruction)
	{
		.op = (op)(inst_seg(code, 0) + 10 * inst_seg(code, 1)),
			.p0_mode = inst_seg(code, 2),
			.p1_mode = inst_seg(code, 3),
			.p2_mode = inst_seg(code, 4)
	};
}

static void ensure_mem_size(vector* code, size_t size)
{
	while(size >= code->size)
	{
		vector_push(code, &(int64_t) { 0 });
	}
}

static int64_t mem_access(vector* code, size_t addr)
{
	ensure_mem_size(code, addr);
	return *(int64_t*)vector_at(code, addr);
}

static int64_t param(vector* code, size_t code_addr, size_t relative_base, param_mode mode)
{
	int64_t p = mem_access(code, code_addr);
	if(mode == RELATIVE)
	{
		p += relative_base;
	}
	if(mode == ADDRESS || mode == RELATIVE)
	{
		p = mem_access(code, p);
	}
	return p;
}

static int64_t addr(vector* code, size_t code_addr, size_t relative_base, param_mode mode)
{
	int64_t p = mem_access(code, code_addr);
	if(mode == RELATIVE)
	{
		p += relative_base;
	}
	ensure_mem_size(code, p);
	return p;
}

intcode_run_result intcode_run(vector* code, vector* input, vector* output)
{
	return intcode_continue(code, input, output, 0, false, false);
}

intcode_run_result intcode_continue(vector* code, vector* input, vector* output, size_t inst_ptr, bool break_inp, bool break_out)
{
	size_t relative_base = 0;
	instruction inst;
	while((inst = parse_inst(*(int64_t*)vector_at(code, inst_ptr))).op != OP_END)
	{
		switch(inst.op)
		{
			case OP_ADD:
			case OP_MUL:
			case OP_LTN:
			case OP_EQL:
			{
				int64_t lhs = param(code, inst_ptr + 1, relative_base, inst.p0_mode);
				int64_t rhs = param(code, inst_ptr + 2, relative_base, inst.p1_mode);
				int64_t res_addr = addr(code, inst_ptr + 3, relative_base, inst.p2_mode);
				int64_t res = 0;
				switch(inst.op)
				{
					case OP_ADD: res = lhs + rhs; break;
					case OP_MUL: res = lhs * rhs; break;
					case OP_LTN: res = lhs < rhs; break;
					case OP_EQL: res = lhs == rhs; break;
				}
				vector_set(code, res_addr, &res, NULL);

				inst_ptr += 4;
				break;
			}
			case OP_INP:
			{
				int64_t res_addr = addr(code, inst_ptr + 1, relative_base, inst.p0_mode);
				int64_t res = 0;
				if(break_inp)
				{
					return (intcode_run_result) { .type = IC_INP_BREAK, .inst_ptr = inst_ptr };
				}
				if(input != NULL)
				{
					if(input->size != 0)
					{
						res = *(int64_t*)vector_at(input, 0);
						vector_remove_at(input, 0, NULL);
					}
					else
					{
						return (intcode_run_result) { .type = IC_INP_MISSING, .inst_ptr = inst_ptr };
					}
				}
				vector_set(code, res_addr, &res, NULL);

				inst_ptr += 2;
				break;
			}
			case OP_OUT:
			{
				if(output != NULL)
				{
					int64_t res = param(code, inst_ptr + 1, relative_base, inst.p0_mode);
					vector_push(output, &res);
				}

				inst_ptr += 2;
				if(break_out)
				{
					return (intcode_run_result) { .type = IC_OUT_BREAK, .inst_ptr = inst_ptr };
				}
				break;
			}
			case OP_JIT:
			case OP_JIF:
			{
				int64_t cond = param(code, inst_ptr + 1, relative_base, inst.p0_mode);
				int64_t addr = param(code, inst_ptr + 2, relative_base, inst.p1_mode);
				if(inst.op == OP_JIT ? cond : !cond)
				{
					inst_ptr = addr;
					continue;
				}

				inst_ptr += 3;
				break;
			}
			case OP_REL:
			{
				int64_t mod = param(code, inst_ptr + 1, relative_base, inst.p0_mode);
				relative_base += mod;

				inst_ptr += 2;
				break;
			}
			default:
				assert(0);
				break;
		}
	}
	return (intcode_run_result) { .type = IC_END, .inst_ptr = inst_ptr };
}

vector intcode_run_simple(string code_str, int64_t* input, size_t input_len)
{
	vector code = intcode_parse(code_str);
	vector in = vector_from(sizeof(int64_t), input, input_len);
	vector output = vector_create(sizeof(int64_t));
	intcode_run(&code, &in, &output);
	vector_delete(&code, NULL);
	vector_delete(&in, NULL);
	return output;
}

int64_t intcode_run_simple_out(string code_str, int64_t* input, size_t input_len)
{
	vector out = intcode_run_simple(code_str, input, input_len);
	int64_t res = out.size > 0 ? *(int64_t*)vector_last(&out) : 0;
	vector_delete(&out, NULL);
	return res;
}

vector intcode_parse(string source)
{
	vector v = vector_create(sizeof(int64_t));
	vector numbers = string_split_all_cstr(source, ",");
	vector_iter it = vector_iterator(&numbers);
	string* s;
	while((s = vector_iter_next(&it)) != NULL)
	{
		int64_t value = strtoll(s->data, NULL, 10);
		vector_push(&v, &value);
	}
	vector_delete(&numbers, string_dealloc);
	return v;
}
