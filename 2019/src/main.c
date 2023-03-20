#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "input.h"
#include "result.h"
#include "solutions.h"

#define USE_PERF_COUNTER 1
#if _WIN32 && USE_PERF_COUNTER
#include <windows.h>
#endif

const char* ANSI_ESC_ERROR = "\x1B[31m";
const char* ANSI_ESC_INFO = "\x1B[90m";
const char* ANSI_ESC_CLOSE = "\033[0m";

void solve(int day);
void solve_part(const input* input, int day, int part);
uint64_t nanoseconds();
double time_diff(uint64_t start, uint64_t end);

int main(int argc, char* argv[])
{
	vector solutions_to_run = vector_create(sizeof(int));
	if(argc > 1)
	{
		if(argc >= 2 && strcmp(argv[1], "--day") == 0)
		{
			int day = atoi(argv[2]);
			if(day <= 0 || day > SOLUTION_COUNT)
			{
				printf("%sExpected number in range [1, %d] after day argument!%s", ANSI_ESC_ERROR, SOLUTION_COUNT, ANSI_ESC_CLOSE);
				return 1;
			}
			vector_push(&solutions_to_run, &day);
		}
		else if(strcmp(argv[1], "--all") == 0)
		{
			for(int i = 1; i <= SOLUTION_COUNT; i++)
			{
				vector_push(&solutions_to_run, &i);
			}
		}
		else
		{
			printf("%sUnknown cmd args, running last solution!%s", ANSI_ESC_ERROR, SOLUTION_COUNT, ANSI_ESC_CLOSE);
		}
	}

	if(solutions_to_run.size == 0)
	{
		int last = SOLUTION_COUNT;
		vector_push(&solutions_to_run, &last);
	}

	for(int i = 0; i < solutions_to_run.size; i++)
	{
		solve(*(int*)vector_at(&solutions_to_run, i));
	}

	vector_delete(&solutions_to_run, NULL);
}

void solve(int day)
{
	char path[sizeof("input/__.txt")];
	snprintf(path, sizeof(path), "input/%02d.txt", day);
	input* input = input_from_file(path);

	printf("%sPreparing Solution %d - %s%s\n", ANSI_ESC_INFO, day, PUZZLE_NAMES[day - 1], ANSI_ESC_CLOSE);
	solve_part(input, day, 1);
	solve_part(input, day, 2);

	input_delete(input);
	free(input);
}

void solve_part(const input* input, int day, int part)
{
	printf("%sSolving Part %d...%s\n", ANSI_ESC_INFO, part, ANSI_ESC_CLOSE);
	int index = SOLUTION_INDEX(day, part);
	solution solution = SOLUTIONS[index];

	uint64_t start = nanoseconds();
	result res = solution(input);
	uint64_t end = nanoseconds();
	printf("%sSolved Part %d (%.5lfms)\nResult:\n%s", ANSI_ESC_INFO, part, time_diff(start, end), ANSI_ESC_CLOSE);
	result_print(res);
	result_delete(res);
	printf("\n\n");
}

uint64_t nanoseconds()
{
	#ifdef _WIN32 && USE_PERF_COUNTER
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	return (uint64_t)t.QuadPart;
	#else
	struct timespec t;
	timespec_get(&t, TIME_UTC);
	return (uint64_t)t.tv_sec * 1000000000 + (uint64_t)t.tv_nsec;
	#endif
}

double time_diff(uint64_t start, uint64_t end)
{
	#ifdef _WIN32 && USE_PERF_COUNTER
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	return (end - start) / (freq.QuadPart / 1000.0);
	#else
	return (end - start) * 1e-6;
	#endif
}
