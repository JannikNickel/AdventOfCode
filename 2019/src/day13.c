#include "solutions.h"
#include "common.h"
#include "shared/intcode.h"

result day13_part1(const input* input)
{
	vector out = intcode_run_simple(input->raw, NULL, 0);
	uint64_t c = 0;
	for(size_t i = 0; i < out.size; i += 3)
	{
		c += *(int64_t*)vector_at(&out, i + 2) == 2;
	}
	vector_delete(&out, NULL);
	return result_uint(c);
}

result day13_part2(const input* input)
{
	vector code = intcode_parse(input->raw);
	vector in = vector_create(sizeof(int64_t));
	vector out = vector_create(sizeof(int64_t));
	*(int64_t*)vector_first(&code) = 2;

	int64_t score = 0;
	int64_t ball = 0;
	int64_t paddle = 0;
	size_t inst_ptr = 0;
	size_t rel_base = 0;
	intcode_run_result res = (intcode_run_result) { .type = IC_NONE };
	while(res.type != IC_END)
	{
		res = intcode_continue(&code, &in, &out, inst_ptr, rel_base, false, false);
		inst_ptr = res.inst_ptr;
		rel_base = res.rel_base;

		for(size_t i = 0; i < out.size; i += 3)
		{
			vec2 p = vec2_new(*(int64_t*)vector_at(&out, i), *(int64_t*)vector_at(&out, i + 1));
			int64_t t = *(int64_t*)vector_at(&out, i + 2);
			if(p.x == -1 && p.y == 0)
			{
				score = t;
			}
			else if(t == 3)
			{
				paddle = p.x;
			}
			else if(t == 4)
			{
				ball = p.x;
			}
		}

		int64_t joystick = ball < paddle ? -1 : (ball > paddle ? 1 : 0);
		vector_push(&in, &joystick);
		vector_clear(&out, NULL);
	}

	vector_delete(&code, NULL);
	vector_delete(&in, NULL);
	vector_delete(&out, NULL);
	return result_int(score);
}
