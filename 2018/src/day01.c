#include "solutions.h"
#include "common.h"

result day01_part1(const input* input)
{
	int32_t freq = 0;
	for(size_t i = 0; i < input->lines.size; i++)
	{
		string line = *(string*)vector_at(&input->lines, i);
		freq += atoi(line.data);
	}
	return result_int(freq);
}

result day01_part2(const input* input)
{
	int32_t freq = 0;
	set known_freqs = set_create(sizeof(int32_t), input->lines.size * input->lines.size, 1.0f, NULL, NULL);
	set_insert(&known_freqs, &freq);

	size_t i = 0;
	while(true)
	{
		string line = *(string*)vector_at(&input->lines, i);
		i = (i + 1) % input->lines.size;

		freq += atoi(line.data);
		if(!set_insert(&known_freqs, &freq))
		{
			break;
		}
	}

	set_delete(&known_freqs, NULL);
	return result_int(freq);
}
