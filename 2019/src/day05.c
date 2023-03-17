#include "solutions.h"
#include "common.h"
#include "shared/intcode.h"

static int64_t run(const input* input, int64_t in);

result day05_part1(const input* input)
{
	return result_int(run(input, 1));
}

result day05_part2(const input* input)
{
	return result_int(run(input, 5));
}

int64_t run(const input* input, int64_t in)
{
	vector output = intcode_run_simple(input->raw, &in, 1);
	int64_t res = *(int64_t*)vector_last(&output);
	vector_delete(&output, NULL);
	return res;
}
