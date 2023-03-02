#pragma once
#include <memory>
#include <type_traits>
#include "input.h"
#include "day01.h"
#include "day02.h"
#include "day03.h"
#include "day04.h"

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
		CreateInstance<Day04>
	};

public:
	static constexpr int COUNT = sizeof(DAYS) / sizeof(DAYS[0]);

	static std::unique_ptr<SolutionBase> GetSolution(int day, const Input& input)
	{
		return DAYS[day - 1](input);
	}
};
