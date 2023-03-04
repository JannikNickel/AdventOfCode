#include "day10.h"
#include <ranges>
#include <algorithm>
#include <utility>

using State = std::pair<int, int>;

ResultBase* Day10::SolveFirst() const
{
	std::vector<int> adapters = ParseInput();
	std::ranges::sort(adapters, std::less());
	return new Result(CountJoltageDiff(adapters));
}

ResultBase* Day10::SolveSecond() const
{
	std::vector<int> adapters = ParseInput();
	int device = *std::ranges::max_element(adapters) + 3;
	adapters.push_back(device);
	std::unordered_set<int> set = std::unordered_set<int>(adapters.begin(), adapters.end());
	std::unordered_map<int, uint64_t> cache = {};
	return new Result(CountValidConfigs(set, cache, 0, device));
}

int Day10::CountJoltageDiff(const std::vector<int>& adapters) const
{
	int diffs[4] = { 0 };
	diffs[adapters[0]]++;
	for(size_t i = 0; i < adapters.size() - 1; i++)
	{
		diffs[adapters[i + 1] - adapters[i]]++;
	}
	diffs[3]++;
	return diffs[1] * diffs[3];
}

uint64_t Day10::CountValidConfigs(const std::unordered_set<int>& adapters, std::unordered_map<int, uint64_t>& cache, int adapter, int target) const
{
	if(adapter == target)
	{
		return 1;
	}
	if(cache.contains(adapter))
	{
		return cache[adapter];
	}
	uint64_t res = 0;
	for(int i = 1; i <= 3; i++)
	{
		if(adapters.contains(adapter + i))
		{
			res += CountValidConfigs(adapters, cache, adapter + i, target);
		}
	}
	cache[adapter] = res;
	return res;
}

std::vector<int> Day10::ParseInput() const
{
	auto numbers = input.lines | std::views::transform([](const std::string& n)
	{
		return std::stoull(n);
	});
	return std::vector<int>(numbers.begin(), numbers.end());
}
