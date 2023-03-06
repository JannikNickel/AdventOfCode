#pragma once
#include <memory>
#include <type_traits>
#include "input.h"
#include "day01.h"
#include "day02.h"
#include "day03.h"
#include "day04.h"
#include "day05.h"
#include "day06.h"
#include "day07.h"
#include "day08.h"
#include "day09.h"
#include "day10.h"
#include "day11.h"
#include "day12.h"
#include "day13.h"
#include "day14.h"
#include "day15.h"
#include "day16.h"
#include "day17.h"

class Solutions
{
	template<typename T> requires std::is_base_of<SolutionBase, T>::value
	static std::unique_ptr<SolutionBase> CreateInstance(const Input& input)
	{
		return std::make_unique<T>(input);
	}

	static constexpr std::unique_ptr<SolutionBase>(*DAYS[])(const Input&) =
	{
		CreateInstance<Day01>,
		CreateInstance<Day02>,
		CreateInstance<Day03>,
		CreateInstance<Day04>,
		CreateInstance<Day05>,
		CreateInstance<Day06>,
		CreateInstance<Day07>,
		CreateInstance<Day08>,
		CreateInstance<Day09>,
		CreateInstance<Day10>,
		CreateInstance<Day11>,
		CreateInstance<Day12>,
		CreateInstance<Day13>,
		CreateInstance<Day14>,
		CreateInstance<Day15>,
		CreateInstance<Day16>,
		CreateInstance<Day17>
	};

public:
	static constexpr int COUNT = sizeof(DAYS) / sizeof(DAYS[0]);

	static std::unique_ptr<SolutionBase> GetSolution(int day, const Input& input)
	{
		return DAYS[day - 1](input);
	}
};
