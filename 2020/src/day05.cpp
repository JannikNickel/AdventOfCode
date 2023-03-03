#include "day05.h"
#include <ranges>
#include <algorithm>
#include <unordered_set>
#include <utility>

struct Range
{
	int min;
	int max;

	Range(int min, int max) : min(min), max(max) { }

	std::pair<Range, Range> Split()
	{
		return std::make_pair(Range(min, min + (max - min) / 2), Range(min + (max - min) / 2 + 1, max));
	}
};

ResultBase* Day05::SolveFirst() const
{
	int res = std::ranges::max(std::views::transform(input.lines, [](const std::string& n) { return CalcSeatId(n); }));
	return new Result(res);
}

ResultBase* Day05::SolveSecond() const
{
	auto ids = input.lines | std::views::transform([](const std::string& n) { return CalcSeatId(n); });
	std::unordered_set<int> set = std::unordered_set<int>(ids.begin(), ids.end());
	for(int i = 1; i < 127 * 8; i++)
	{
		if(!set.contains(i) && set.contains(i - 1) && set.contains(i + 1))
		{
			return new Result(i);
		}
	}
	return nullptr;
}

int Day05::CalcSeatId(const std::string& seat)
{
	Range row = Range(0, 127);
	for(int i = 0; i < 7; i++)
	{
		row = seat[i] == 'F' ? row.Split().first : row.Split().second;
	}
	Range column = Range(0, 7);
	for(int i = 7; i < 7 + 3; i++)
	{
		column = seat[i] == 'L' ? column.Split().first : column.Split().second;
	}
	return row.min * 8 + column.min;
}
