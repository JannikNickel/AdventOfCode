#include "solutions.h"
#include "common.h"
#include "shared/intcode.h"

result day19_part1(const input* input)
{
	int amount = 0;
	for(int i = 0; i < 50; i++)
	{
		for(int k = 0; k < 50; k++)
		{
			amount += intcode_run_simple_out(input->raw, &(int64_t[]) { i, k }, 2);
		}
	}
	return result_int(amount);
}

result day19_part2(const input* input)
{
	const int square = 100;
	uint32_t left = 0;
	uint32_t y = square;
	while(true)
	{
		while(intcode_run_simple_out(input->raw, &(int64_t[]) { left, y }, 2) == 0)
		{
			left += 1;
		}
		if(intcode_run_simple_out(input->raw, &(int64_t[]) { left + (square - 1), y - (square - 1) }, 2))
		{
			return result_int(left * 10000 + y - (square - 1));
		}
		y++;
	}
	return result_none();
}
