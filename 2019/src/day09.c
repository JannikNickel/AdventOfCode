#include "solutions.h"
#include "common.h"
#include "shared/intcode.h"

result day09_part1(const input* input)
{
	return result_int(intcode_run_simple_out(input->raw, &(int64_t) { 1 }, 1));
}

result day09_part2(const input* input)
{
	return result_int(intcode_run_simple_out(input->raw, &(int64_t) { 2 }, 1));
}
