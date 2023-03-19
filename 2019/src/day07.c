#include "solutions.h"
#include "common.h"
#include "shared/intcode.h"

static int64_t solve_amp(string code_str, int64_t phase[5]);
static void swap(uint64_t* a, uint64_t* b);
static bool next_perm(uint64_t* a, uint64_t n);

result day07_part1(const input* input)
{
	int64_t res = solve_amp(input->raw, &(int64_t[]) { 0, 1, 2, 3, 4 });
	return result_int(res);
}

result day07_part2(const input* input)
{
	int64_t res = solve_amp(input->raw, &(int64_t[]) { 5, 6, 7, 8, 9 });
	return result_int(res);
}

int64_t solve_amp(string code_str, int64_t phase[5])
{
	vector code = intcode_parse(code_str);
	vector in = vector_create(sizeof(int64_t));
	vector output = vector_create(sizeof(int64_t));
	int64_t max_thrust = 0;
	do
	{
		vector amp_codes[5] = { vector_clone(&code, NULL), vector_clone(&code, NULL), vector_clone(&code, NULL), vector_clone(&code, NULL), vector_clone(&code, NULL) };
		size_t amp_inst_ptrs[5] = { 0 };

		int64_t prev = 0;
		size_t count = 0;
		while(true)
		{
			for(int64_t i = 0; i < 5; i++)
			{
				vector_clear(&in, NULL);
				if(count == 0)
				{
					vector_push(&in, &phase[i]);
				}
				vector_push(&in, &prev);
				vector_clear(&output, NULL);

				intcode_run_result res = intcode_continue(&amp_codes[i], &in, &output, amp_inst_ptrs[i], false, false);
				amp_inst_ptrs[i] = res.inst_ptr;
				prev = *(int64_t*)vector_last(&output);
				if(i == 4 && res.type == IC_END)
				{
					goto END;
				}
			}
			count++;
		}
		END:
		max_thrust = max(max_thrust, prev);

		for(size_t i = 0; i < 5; i++)
		{
			vector_clear(&amp_codes[i], NULL);
		}
	}
	while(next_perm(phase, 5));

	vector_delete(&in, NULL);
	vector_delete(&output, NULL);
	vector_delete(&code, NULL);
	return max_thrust;
}

void swap(uint64_t* a, uint64_t* b)
{
	uint64_t temp = *a;
	*a = *b;
	*b = temp;
}

bool next_perm(uint64_t* a, uint64_t n)
{
	uint32_t k = -1;
	for(uint32_t i = 0; i < n - 1; i++)
	{
		if(a[i] < a[i + 1])
		{
			k = i;
		}
	}
	if(k == -1)
	{
		return false;
	}
	uint32_t l = n - 1;
	while(a[l] < a[k])
	{
		l--;
	}
	swap(&a[k], &a[l]);
	for(uint32_t i = k + 1, j = n - 1; i < j; i++, j--)
	{
		swap(&a[i], &a[j]);
	}
	return true;
}
