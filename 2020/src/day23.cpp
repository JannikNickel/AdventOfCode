#include "day23.h"
#include <ranges>
#include <algorithm>
#include <utility>

ResultBase* Day23::SolveFirst() const
{
	std::vector<uint32_t> numbers = ParseInput();
	std::unordered_map<uint32_t, uint32_t> circle = Simulate(numbers, 100);
	std::string res = "";
	uint32_t n = circle[1];
	for(size_t i = 0; i < numbers.size() - 1; i++)
	{
		res += std::to_string(n);
		n = circle[n];
	}
	return new Result(res);
}

ResultBase* Day23::SolveSecond() const
{
	std::vector<uint32_t> numbers = ParseInput();
	std::unordered_map<uint32_t, uint32_t> circle = Simulate(numbers, 10000000);
	return new Result(static_cast<uint64_t>(circle[1]) * static_cast<uint64_t>(circle[circle[1]]));
}

std::unordered_map<uint32_t, uint32_t> Day23::Simulate(std::vector<uint32_t>& numbers, uint32_t steps) const
{	
	if(steps == 10000000)
	{
		uint32_t start = std::ranges::max(numbers);
		size_t toAdd = 1000000 - numbers.size();
		numbers.reserve(numbers.size() + toAdd);
		for(size_t i = 1; i <= toAdd; i++)
		{
			numbers.push_back(start + i);
		}
	}

	std::unordered_map<uint32_t, uint32_t> next = {};
	for(size_t i = 0; i < numbers.size(); i++)
	{
		next[numbers[i]] = numbers[(i + 1) % numbers.size()];
	}

	uint32_t curr = numbers[0];
	uint32_t max = std::ranges::max(numbers);
	for(size_t i = 0; i < steps; i++)
	{
		uint32_t n0 = next[curr];
		uint32_t n1 = next[n0];
		uint32_t n2 = next[n1];
		next[curr] = next[n2];

		uint32_t target = curr;
		do
		{
			target--;
			if(target <= 0)
			{
				target = max;
			}
		}
		while(target == n0 || target == n1 || target == n2);

		next[n2] = next[target];
		next[target] = n0;
		curr = next[curr];
	}

	return next;
}

std::vector<uint32_t> Day23::ParseInput() const
{
	auto numbers = std::ranges::views::transform(input.raw, [](char n) { return static_cast<uint32_t>(n - '0'); });
	return std::vector<uint32_t>(numbers.begin(), numbers.end());
}
