#include "solutions.h"
#include "common.h"

static uint32_t solve(const input* input, bool part2);
static uint32_t sum_meta(const uint32_t** data, bool part2);
static vector parse_input(const input* input);

result day08_part1(const input* input)
{
	return result_int(solve(input, false));
}

result day08_part2(const input* input)
{
	return result_int(solve(input, true));
}

uint32_t solve(const input* input, bool part2)
{
	vector nums = parse_input(input);
	uint32_t* data = (uint32_t*)nums.data;
	uint32_t sum = sum_meta(&data, part2);
	vector_delete(&nums, NULL);
	return sum;
}

uint32_t sum_meta(const uint32_t** data, bool part2)
{
	uint32_t child_count = *(*data + 0);
	uint32_t meta_count = *(*data + 1);
	*data += 2;

	uint32_t sum = 0;
	uint32_t* child_sums = part2 ? malloc(sizeof(uint32_t) * child_count) : NULL;
	for(size_t i = 0; i < child_count; i++)
	{
		uint32_t child_sum = sum_meta(data, part2);
		*(part2 ? &child_sums[i] : &sum) = child_sum + (!part2 ? sum : 0);
	}

	for(size_t i = 0; i < meta_count; i++)
	{
		uint32_t meta = *(*data + i);
		if(part2 && child_count > 0)
		{
			meta = meta > 0 && meta <= child_count ? child_sums[meta - 1] : 0;
		}
		sum += meta;
	}
	*data += meta_count;

	free(child_sums);
	return sum;
}

vector parse_input(const input* input)
{
	vector nums = vector_create(sizeof(uint32_t));
	char* ptr = input->raw.data;
	while(*ptr)
	{
		uint32_t num = strtoul(ptr, &ptr, 10);
		vector_push(&nums, &num);
		while(*ptr == ' ')
		{
			ptr++;
		}
	}
	return nums;
}
