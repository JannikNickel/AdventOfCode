#include "solutions.h"
#include "common.h"

typedef struct
{
	char* gen;
	char* prev_gen;
	size_t size;
	size_t zero_idx;
} slots_t;

static int64_t solve(const input* input, uint64_t generations);
static void simulate_gen(slots_t* slots, char rules[32]);
static void add_padding(slots_t* slots);
static int64_t count_plants(const slots_t* slots);
static uint8_t rule_id(const char* pattern);
static slots_t parse_input(const input* input, char rules[32]);

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
	slots_t slots = parse_input(input, rules);

	int64_t result = 0;
	int stable_count = 0;
	int64_t prev_diff = 0;
	int64_t prev_sum = count_plants(&slots);
	for(uint64_t i = 0;i < generations; i++)
	{
		add_padding(&slots);
		simulate_gen(&slots, rules);

		int64_t sum = count_plants(&slots);
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

	free(slots.gen);
	free(slots.prev_gen);
	return result;
}

void simulate_gen(slots_t* slots, char rules[32])
{
	memcpy(slots->prev_gen, slots->gen, sizeof(char) * slots->size);
	memset(slots->gen, '.', sizeof(char) * slots->size);
	for(size_t i = 0; i < slots->size - 4; i++)
	{
		uint8_t pattern_id = rule_id(&slots->prev_gen[i]);
		slots->gen[i + 2] = rules[pattern_id];
	}
}

void add_padding(slots_t* slots)
{
	size_t left_idx = SIZE_MAX, right_idx = 0;
	for(size_t i = 0; i < slots->size; i++)
	{
		if(slots->gen[i] == '#')
		{
			if(left_idx == SIZE_MAX)
			{
				left_idx = i;
			}
			right_idx = i;
		}
	}

	if(left_idx != SIZE_MAX)
	{
		const size_t min_padding = 5 * 10;
		size_t right_inc = right_idx + min_padding >= slots->size ? right_idx + min_padding - slots->size + 1 : 0;
		size_t left_inc = left_idx < min_padding ? min_padding - left_idx : 0;
		size_t new_size = slots->size + left_inc + right_inc;
		if(new_size > slots->size)
		{
			char* new_gen = malloc(sizeof(char) * new_size);
			char* new_prev_gen = malloc(sizeof(char) * new_size);
			memset(new_gen, '.', sizeof(char) * new_size);
			memcpy(new_gen + left_inc, slots->gen, sizeof(char) * slots->size);
			free(slots->gen);
			free(slots->prev_gen);
			slots->gen = new_gen;
			slots->prev_gen = new_prev_gen;
			slots->size = new_size;
			slots->zero_idx += left_inc;
		}
	}
}

int64_t count_plants(const slots_t* slots)
{
	int64_t sum = 0;
	for(size_t i = 0; i < slots->size; i++)
	{
		sum += slots->gen[i] == '#' ? ((int64_t)i - (int64_t)slots->zero_idx) : 0;
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

slots_t parse_input(const input* input, char rules[32])
{
	const char* initial_state = input->lines_c[0].data + strlen("initial state: ");
	size_t initial_slot_count = strlen(initial_state);

	slots_t slots = { 0 };
	slots.size = initial_slot_count;
	slots.zero_idx = 0;
	slots.gen = malloc(sizeof(char) * initial_slot_count);
	slots.prev_gen = malloc(sizeof(char) * initial_slot_count);

	memset(slots.gen, '.', sizeof(char) * initial_slot_count);
	memcpy(slots.gen + slots.zero_idx, initial_state, initial_slot_count);

	for(size_t i = 2; i < input->line_count; i++)
	{
		const char* line = input->lines_c[i].data;
		rules[rule_id(line)] = line[9];
	}

	return slots;
}
