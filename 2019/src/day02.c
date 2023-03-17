#include "solutions.h"
#include "common.h"

uint64_t run_intcode(vector* code);
vector parse_input(const input* input);

result day02_part1(const input* input)
{
	vector code = parse_input(input);
	uint64_t first = 12, second = 2;
	vector_set(&code, 1, &first, NULL);
	vector_set(&code, 2, &second, NULL);
	uint64_t res = run_intcode(&code);
	return result_int(res);
}

result day02_part2(const input* input)
{
	vector code = parse_input(input);
	for(uint64_t i = 0; i < 100; i++)
	{
		for(uint64_t k = 0; k < 100; k++)
		{
			vector code_instance = vector_clone(&code, NULL);
			vector_set(&code_instance, 1, &i, NULL);
			vector_set(&code_instance, 2, &k, NULL);
			uint64_t res = run_intcode(&code_instance);
			vector_delete(&code_instance, NULL);
			if(res == 19690720)
			{
				return result_int(100 * i + k);
			}
		}
	}
	return result_none();
}

uint64_t run_intcode(vector* code)
{
	size_t pos = 0;
	uint64_t* op;
	while(*(op = vector_at(code, pos)) != 99)
	{
		if(*op == 1 || *op == 2)
		{
			uint64_t lhs = *(uint64_t*)vector_at(code, *(uint64_t*)vector_at(code, pos + 1));
			uint64_t rhs = *(uint64_t*)vector_at(code, *(uint64_t*)vector_at(code, pos + 2));
			uint64_t ti = *(uint64_t*)vector_at(code, pos + 3);
			uint64_t res = *op == 1 ? lhs + rhs : lhs * rhs;
			vector_set(code, ti, &res, NULL);
			pos += 4;
		}
		else
		{
			return UINT64_MAX;
		}
	}
	return *(uint64_t*)vector_at(code, 0);
}

vector parse_input(const input* input)
{
	vector v = vector_create(sizeof(uint64_t));
	vector numbers = string_split_all_cstr(input->raw, ",");
	vector_iter it = vector_iterator(&numbers);
	string* s;
	while((s = vector_iter_next(&it)) != NULL)
	{
		uint64_t value = strtoull(s->data, NULL, 10);
		vector_push(&v, &value);
	}
	vector_delete(&numbers, string_dealloc);
	return v;
}
