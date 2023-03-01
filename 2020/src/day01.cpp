#include "day01.h"
#include <ranges>

ResultBase* Day01::SolveFirst() const
{
	std::vector<int> numbers = ParseInput();
	for(int i = 0; i < numbers.size(); i++)
	{
		for(int k = i + 1; k < numbers.size(); k++)
		{
			if(numbers[i] + numbers[k] == 2020)
			{
				return new Result(numbers[i] * numbers[k]);
			}
		}
	}
	return nullptr;
}

ResultBase* Day01::SolveSecond() const
{
	std::vector<int> numbers = ParseInput();
	for(int i = 0; i < numbers.size(); i++)
	{
		for(int k = i + 1; k < numbers.size(); k++)
		{
			for(int l = k + 1; l < numbers.size(); l++)
			{
				if(numbers[i] + numbers[k] + numbers[l] == 2020)
				{
					return new Result(numbers[i] * numbers[k] * numbers[l]);
				}
			}
		}
	}
	return nullptr;
}

std::vector<int> Day01::ParseInput() const
{
	auto res = input.lines | std::views::transform([](const std::string& str)
	{
		return std::stoi(str);
	});
	return std::vector<int>(res.begin(), res.end());
}
