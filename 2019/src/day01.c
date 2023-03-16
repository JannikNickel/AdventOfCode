#include "solutions.h"
#include "common.h"

result day01_part1(const input* input)
{
	int64_t sum = 0;
	vector_iter it = vector_iterator(&input->lines);
	string* s;
	while((s = vector_iter_next(&it)) != NULL)
	{
		sum += atoll(s->data) / 3 - 2;
	}
	return result_int(sum);
}

result day01_part2(const input* input)
{
	int64_t sum = 0;
	vector_iter it = vector_iterator(&input->lines);
	string* s;
	while((s = vector_iter_next(&it)) != NULL)
	{
		int64_t mod = atoll(s->data) / 3 - 2;
		do
		{
			sum += mod;
			mod = mod / 3 - 2;
		}
		while(mod > 0);
	}
	return result_int(sum);
}
