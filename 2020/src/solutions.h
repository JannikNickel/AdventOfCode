#pragma once
#include <memory>
#include <type_traits>
#include "input.h"
#include "day01.h"

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
	};

public:
	static constexpr int COUNT = sizeof(DAYS) / sizeof(DAYS[0]);

	static std::unique_ptr<SolutionBase> GetSolution(int day, const Input& input)
	{
		return DAYS[day - 1](input);
	}
};
