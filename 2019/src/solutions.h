#ifndef SOLUTIONS_H
#define SOLUTIONS_H

#include "input.h"
#include "result.h"

result day01_part1(const input* input);
result day01_part2(const input* input);
result day02_part1(const input* input);
result day02_part2(const input* input);

typedef result (*solution)(const input* input);
static const solution SOLUTIONS[] =
{
	day01_part1,
	day01_part2,
	day02_part1,
	day02_part2
};

#define SOLUTION_COUNT sizeof(SOLUTIONS) / sizeof(SOLUTIONS[0]) / 2
#define SOLUTION_INDEX(day, part) ((day) - 1) * 2 + ((part) - 1)

static const char* PUZZLE_NAMES[] =
{
	"The Tyranny of the Rocket Equation",
	"1202 Program Alarm",
	"Crossed Wires",
	"Secure Container",
	"Sunny with a Chance of Asteroids",
	"Universal Orbit Map",
	"Amplification Circuit",
	"Space Image Format",
	"Sensor Boost",
	"Monitoring Station",
	"Space Police",
	"The N-Body Problem",
	"Care Package",
	"Space Stoichiometry",
	"Oxygen System",
	"Flawed Frequency Transmission",
	"Set and Forge",
	"Many-Worlds Interpretation",
	"Tractor Beam",
	"Donut Maze",
	"Springdroid Adventure",
	"Slam Shuffle",
	"Category Six",
	"Planet of Discord",
	"Cryostasis"
};
#define PUZZLE_NAME(day) PUZZLE_NAMES[(day)]

#endif