#include "solutions.h"
#include "common.h"

typedef struct
{
	uint32_t from;
	uint32_t to;
} range;

static uint64_t count_variants(uint32_t from, uint32_t to, bool groups);
static bool valid_password(uint32_t pw, bool groups);
static int get_digit(uint32_t pw, int index);
static range parse_input(const input* input);

result day04_part1(const input* input)
{
	range r = parse_input(input);
	uint64_t res = count_variants(r.from, r.to, false);
	return result_uint(res);
}

result day04_part2(const input* input)
{
	range r = parse_input(input);
	uint64_t res = count_variants(r.from, r.to, true);
	return result_uint(res);
}

uint64_t count_variants(uint32_t from, uint32_t to, bool groups)
{
	uint64_t variants = 0;
	for(uint32_t i = from; i <= to; i++)
	{
		variants += valid_password(i, groups);
	}
	return variants;
}

bool valid_password(uint32_t pw, bool groups)
{
	bool duplicate = false;
	int curr_count = 1;
	for(uint32_t i = 1; i < 6; i++)
	{
		uint32_t prev = get_digit(pw, i - 1);
		uint32_t digit = get_digit(pw, i);
		if(digit < prev)
		{
			return false;
		}

		if(digit == prev)
		{
			duplicate |= !groups;
			curr_count++;
		}
		else
		{
			duplicate |= curr_count == 2;
			curr_count = 1;
		}
	}
	return duplicate || curr_count == 2;
}

int get_digit(uint32_t pw, int index)
{
	return (pw / (uint32_t)pow(10, 5 - index)) % 10;
}

range parse_input(const input* input)
{
	string_pair pair = string_split_char(input->raw, '-');
	range r = (range) { .from = strtoul(pair.a.data, NULL, 10), .to = strtoul(pair.b.data, NULL, 10) };
	string_pair_delete(&pair);
	return r;
}
