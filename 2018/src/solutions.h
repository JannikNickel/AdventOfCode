#ifndef SOLUTIONS_H
#define SOLUTIONS_H

#include "common.h"

result day01_part1(const input* input);
result day01_part2(const input* input);
result day02_part1(const input* input);
result day02_part2(const input* input);
result day03_part1(const input* input);
result day03_part2(const input* input);
result day04_part1(const input* input);
result day04_part2(const input* input);

typedef result (*solution)(const input* input);
static const solution SOLUTIONS[] =
{
	day01_part1,
	day01_part2,
	day02_part1,
	day02_part2,
	day03_part1,
	day03_part2,
	day04_part1,
	day04_part2,
};

#define SOLUTION_COUNT sizeof(SOLUTIONS) / sizeof(SOLUTIONS[0]) / 2
#define SOLUTION_INDEX(day, part) ((day) - 1) * 2 + ((part) - 1)

static const char* PUZZLE_NAMES[] =
{
	"Chronal Calibration",
	"Inventory Management System",
	"No Matter How You Slice It",
	"Repose Record",
};

#endif
