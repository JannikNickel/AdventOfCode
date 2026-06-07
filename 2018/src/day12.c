#include "solutions.h"
#include "common.h"

static const size_t SLOT_COUNT = 1000;

static int64_t solve(const input* input, uint64_t generations);
static void simulate_gen(char* slots, char* prev_gen, size_t slot_count, char rules[32]);
static int64_t count_plants(const char* slots, size_t slot_count, size_t zero_idx);
static uint8_t rule_id(const char* pattern);
static void parse_input(const input* input, char* slots, char rules[32], size_t zero_idx);

result day12_part1(const input* input)
{
	return result_int(solve(input, 20ULL));
}

result day12_part2(const input* input)
{
	return result_int(solve(input, 50000000000ULL));
}

int64_t solve(const input* input, uint64_t generations)
{
	char rules[32] = { 0 };
	memset(rules, '.', sizeof(char) * 32);

	size_t zero_idx = SLOT_COUNT / 2;
	char* slots = malloc(sizeof(char) * SLOT_COUNT);
	char* prev_gen = malloc(sizeof(char) * SLOT_COUNT);
	parse_input(input, slots, rules, zero_idx);

	int64_t result = 0;
	int stable_count = 0;
	int64_t prev_diff = 0;
	int64_t prev_sum = count_plants(slots, SLOT_COUNT, zero_idx);
	for(uint64_t i = 0;i < generations; i++)
	{
		simulate_gen(slots, prev_gen, SLOT_COUNT, rules);

		int64_t sum = count_plants(slots, SLOT_COUNT, zero_idx);
		int64_t diff = sum - prev_sum;
		prev_sum = sum;

		if(diff == prev_diff)
		{
			stable_count++;
		}
		else
		{
			stable_count = 0;
		}

		if(stable_count >= 5 || i == generations - 1)
		{
			result = sum + diff * (generations - i - 1);
			break;
		}
		prev_diff = diff;
	}

	free(slots);
	free(prev_gen);
	return result;
}

void simulate_gen(char* slots, char* prev_gen, size_t slot_count, char rules[32])
{
	memcpy(prev_gen, slots, sizeof(char) * slot_count);
	memset(slots, '.', sizeof(char) * slot_count);
	for(size_t i = 0; i < slot_count - 4; i++)
	{
		uint8_t pattern_id = rule_id(&prev_gen[i]);
		slots[i + 2] = rules[pattern_id];
	}
}

int64_t count_plants(const char* slots, size_t slot_count, size_t zero_idx)
{
	int64_t sum = 0;
	for(size_t i = 0; i < slot_count; i++)
	{
		sum += slots[i] == '#' ? ((int64_t)i - (int64_t)zero_idx) : 0;
	}
	return sum;
}

uint8_t rule_id(const char* pattern)
{
	uint8_t id = 0;
	for(uint8_t i = 0; i < 5; i++)
	{
		id <<= 1;
		id |= pattern[i] == '#';
	}
	return id;
}

void parse_input(const input* input, char* slots, char rules[32], size_t zero_idx)
{
	const char* initial_state = input->lines_c[0].data + strlen("initial state: ");
	size_t initial_slot_count = strlen(initial_state);
	memset(slots, '.', sizeof(char) * SLOT_COUNT);
	memcpy(slots + zero_idx, initial_state, initial_slot_count);

	for(size_t i = 2; i < input->line_count; i++)
	{
		const char* line = input->lines_c[i].data;
		rules[rule_id(line)] = line[9];
	}
}
