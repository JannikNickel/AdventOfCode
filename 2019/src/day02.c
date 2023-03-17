#include "solutions.h"
#include "common.h"
#include "shared/intcode.h"

static int64_t run_intcode(vector* code);

result day02_part1(const input* input)
{
	vector code = intcode_parse(input->raw);
	vector_set(&code, 1, &(int64_t){12}, NULL);
	vector_set(&code, 2, &(int64_t){2}, NULL);
	int64_t res = run_intcode(&code);
	vector_delete(&code, NULL);
	return result_int(res);
}

result day02_part2(const input* input)
{
	vector code = intcode_parse(input->raw);
	for(int64_t i = 0; i < 100; i++)
	{
		for(int64_t k = 0; k < 100; k++)
		{
			vector code_instance = vector_clone(&code, NULL);
			vector_set(&code_instance, 1, &i, NULL);
			vector_set(&code_instance, 2, &k, NULL);
			int64_t res = run_intcode(&code_instance);
			vector_delete(&code_instance, NULL);
			if(res == 19690720)
			{
				vector_delete(&code, NULL);
				return result_int(100 * i + k);
			}
		}
	}
	vector_delete(&code, NULL);
	return result_none();
}

int64_t run_intcode(vector* code)
{
	intcode_run(code, NULL, NULL);
	return *(int64_t*)vector_at(code, 0);
}
