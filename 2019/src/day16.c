#include "solutions.h"
#include "common.h"

static uint64_t dig_to_num(const vector* digits, size_t from, size_t amount);
static vector parse_input(const input* input);

result day16_part1(const input* input)
{
	int32_t pattern[4] = { 0, 1, 0, -1 };
	vector v = parse_input(input);

	for(size_t i = 0; i < 100; i++)
	{
		vector next = vector_create(sizeof(int32_t));
		vector_set_capacity(&next, v.size);
		for(size_t i = 1; i <= v.size; i++)
		{
			size_t off = i - 1;
			int pattern_index = 0;
			int pattern_sub_index = off + 1;
			int32_t res = 0;
			for(size_t k = off; k < v.size; k++)
			{
				if(pattern_sub_index % i == 0)
				{
					pattern_index = (pattern_index + 1) % 4;
				}
				pattern_sub_index++;
				res += *(int32_t*)vector_at(&v, k) * pattern[pattern_index];
			}
			int32_t c = abs(res % 10);
			vector_push(&next, &c);
		}
		vector_delete(&v, NULL);
		v = next;
	}

	uint64_t res = dig_to_num(&v, 0, 8);
	vector_delete(&v, NULL);
	return result_uint(res);
}


result day16_part2(const input* input)
{
	vector v = parse_input(input);
	size_t in_size = v.size;
	size_t size = in_size * 10000;
	vector_set_capacity(&v, size);
	for(size_t i = 0; i < 10000 - 1; i++)
	{
		for(size_t k = 0; k < in_size; k++)
		{
			vector_push(&v, vector_at(&v, k));
		}
	}

	uint64_t off = dig_to_num(&v, 0, 7);
	for(size_t i = 0; i < 100; i++)
	{
		int64_t p_sum = 0;
		for(size_t k = off; k < size; k++)
		{
			p_sum += *(int32_t*)vector_at(&v, k);
		}
		for(size_t k = off; k < size; k++)
		{
			int32_t curr = *(int32_t*)vector_at(&v, k);
			int32_t next = p_sum % 10;
			vector_set(&v, k, &next, NULL);
			p_sum -= curr;
		}
	}

	uint64_t res = dig_to_num(&v, off, 8);
	vector_delete(&v, NULL);
	return result_uint(res);
}

uint64_t dig_to_num(const vector* digits, size_t from, size_t amount)
{
	uint64_t res = 0;
	for(size_t i = from; i < from + amount; i++)
	{
		res = res * 10 + *(int32_t*)vector_at(digits, i);
	}
	return res;
}

vector parse_input(const input* input)
{
	vector v = vector_create(sizeof(int32_t));
	for(size_t i = 0; i < input->raw.length; i++)
	{
		int32_t c = string_at(input->raw, i) - '0';
		vector_push(&v, &c);
	}
	return v;
}
