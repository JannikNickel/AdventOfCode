#include "day15.h"
#include "common/stringutils.h"
#include <ranges>
#include <unordered_map>

ResultBase* Day15::SolveFirst() const
{
	std::vector<uint64_t> numbers = ParseInput();
	return new Result(FindNumber(numbers, 2020));
}

ResultBase* Day15::SolveSecond() const
{
	std::vector<uint64_t> numbers = ParseInput();
	return new Result(FindNumber(numbers, 30000000));
}

uint64_t Day15::FindNumber(const std::vector<uint64_t>& numbers, uint64_t steps) const
{
	std::unordered_map<uint64_t, uint64_t> lastIndex = {};
	uint64_t last = -1;
	for(uint64_t i = 0; i < steps; i++)
	{
		uint64_t prevIndex = -1;
		if(i != 0)
		{
			prevIndex = lastIndex.contains(last) ? lastIndex[last] : -1;
			lastIndex[last] = i - 1;
		}
		last = i < numbers.size() ? numbers[i] : (prevIndex != -1 ? (i - 1) - prevIndex : 0);
	}
	return last;
}

std::vector<uint64_t> Day15::ParseInput() const
{
	auto numbers = common::SplitStr(input.raw, ",") | std::views::transform([](const std::string& n) { return std::stoull(n); });
	return std::vector<uint64_t>(numbers.begin(), numbers.end());
}
