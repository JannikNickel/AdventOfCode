#include "solutions.h"
#include "common.h"

result day01_part1(const input* input)
{
	int32_t freq = 0;
	for(size_t i = 0; i < input->line_count; i++)
	{
		freq += atoi(input->lines_c[i].data);
	}
	return result_int(freq);
}

result day01_part2(const input* input)
{
	int32_t freq = 0;
	set known_freqs = set_create(sizeof(int32_t), input->line_count * input->line_count, 1.0f, NULL, NULL);
	set_insert(&known_freqs, &freq);

	size_t i = 0;
	while(true)
	{
		freq += atoi(input->lines_c[i].data);
		i = (i + 1) % input->line_count;

		if(!set_insert(&known_freqs, &freq))
		{
			break;
		}
	}

	set_delete(&known_freqs, NULL);
	return result_int(freq);
}
