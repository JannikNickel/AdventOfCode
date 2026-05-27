#include "solutions.h"
#include "common.h"

typedef struct
{
	uint8_t month;
	uint8_t day;
	uint8_t minute;
} time;

typedef struct
{
	uint16_t guard_id;
	time start;
	uint16_t duration;
} sleep;

static vector parse_input(const input* input, uint16_t* max_guard_id);
static string* sort_input(const input* input);
static int datecmp(const void* a, const void* b);
static uint16_t calc_duration(time start, time end);
static void fill_guard_sleep_times(const vector* sleeps, uint16_t guard_id, uint32_t times[60]);
static uint32_t sum_times(uint32_t times[60]);
static size_t max_times(uint32_t times[60]);

result day04_part1(const input* input)
{
	uint16_t max_guard_id = 0;
	vector sleeps = parse_input(input, &max_guard_id);

	uint16_t worst_guard_id = 0;
	uint32_t worst_guard_time = 0;
	uint32_t sleep_times[60];
	for(uint16_t i = 0; i < max_guard_id + 1; i++)
	{
		fill_guard_sleep_times(&sleeps, i, sleep_times);
		uint32_t total_time = sum_times(sleep_times);
		if(total_time > worst_guard_time)
		{
			worst_guard_time = total_time;
			worst_guard_id = i;
		}
	}

	fill_guard_sleep_times(&sleeps, worst_guard_id, sleep_times);
	size_t max_minute = max_times(sleep_times);

	vector_delete(&sleeps, NULL);
	return result_int(worst_guard_id * max_minute);
}

result day04_part2(const input* input)
{
	uint16_t max_guard_id = 0;
	vector sleeps = parse_input(input, &max_guard_id);

	size_t global_max_minute = 0;
	uint16_t global_max_minute_sleeps = 0;
	uint16_t global_max_guard_id = 0;
	uint32_t sleep_times[60];
	for(uint16_t i = 0; i < max_guard_id + 1; i++)
	{
		fill_guard_sleep_times(&sleeps, i, sleep_times);
		size_t max_minute = max_times(sleep_times);
		if(sleep_times[max_minute] > global_max_minute_sleeps)
		{
			global_max_minute_sleeps = sleep_times[max_minute];
			global_max_minute = max_minute;
			global_max_guard_id = i;
		}
	}

	vector_delete(&sleeps, NULL);
	return result_int(global_max_guard_id * global_max_minute);
}

vector parse_input(const input* input, uint16_t* max_guard_id)
{
	string* sorted = sort_input(input);
	vector sleeps = vector_create(sizeof(sleep));
	*max_guard_id = 0;

	sleep current_sleep = { 0 };
	bool is_asleep = false;
	for(size_t i = 0; i < input->line_count; i++)
	{
		const string* line = &sorted[i];

		uint8_t month = 10 * (line->data[6] - '0') + (line->data[7] - '0');
		uint8_t day = 10 * (line->data[9] - '0') + (line->data[10] - '0');
		uint8_t minute = 10 * (line->data[15] - '0') + (line->data[16] - '0');
		time curr_time = { .month = month, .day = day, .minute = minute };

		const char* instruction = line->data + 19;
		if(strncmp(instruction, "Guard #", 7) == 0)
		{
			if(is_asleep)
			{
				current_sleep.duration = calc_duration(current_sleep.start, curr_time);
				vector_push(&sleeps, &current_sleep);
			}

			uint16_t id = atoi(instruction + 7);
			*max_guard_id = max(*max_guard_id, id);
			current_sleep = (sleep) { .guard_id = id, .start = { .month = 0, .day = 0, .minute = 0 } };
			is_asleep = false;
		}
		else
		{
			bool falls_asleep = strcmp(instruction, "falls asleep") == 0;
			if(falls_asleep)
			{
				is_asleep = true;
				current_sleep.start = curr_time;
			}
			else if(is_asleep)
			{
				is_asleep = false;
				current_sleep.duration = calc_duration(current_sleep.start, curr_time);
				vector_push(&sleeps, &current_sleep);
			}
		}
	}
	free(sorted);
	return sleeps;
}

string* sort_input(const input* input)
{
	string* sorted = malloc(sizeof(string) * input->line_count);
	memcpy(sorted, input->lines_c, sizeof(string) * input->line_count);
	qsort(sorted, input->line_count, sizeof(string), datecmp);
	return sorted;
}

int datecmp(const void* a, const void* b)
{
	const string* str_a = (const string*)a;
	const string* str_b = (const string*)b;
	return strncmp(str_a->data, str_b->data, 18);
}

uint16_t calc_duration(time start, time end)
{
	uint16_t day_diff = (start.day != end.day || start.month != end.month);
	return day_diff * 60 + (end.minute - start.minute);
}

void fill_guard_sleep_times(const vector* sleeps, uint16_t guard_id, uint32_t times[60])
{
	memset(times, 0, sizeof(uint32_t) * 60);
	for(size_t i = 0; i < sleeps->size; i++)
	{
		const sleep* s = vector_at_c(sleeps, i);
		if(s->guard_id == guard_id)
		{
			for(size_t k = 0; k < s->duration; k++)
			{
				times[(s->start.minute + k) % 60]++;
			}
		}
	}
}

uint32_t sum_times(uint32_t times[60])
{
	uint32_t sum = 0;
	for(size_t i = 0; i < 60; i++)
	{
		sum += times[i];
	}
	return sum;
}

size_t max_times(uint32_t times[60])
{
	size_t max = 0;
	for(size_t i = 0; i < 60; i++)
	{
		if(times[i] > times[max])
		{
			max = i;
		}
	}
	return max;
}
