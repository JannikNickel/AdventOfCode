#include "day13.h"
#include "common/stringutils.h"
#include <ranges>
#include <algorithm>
#include <limits>

ResultBase* Day13::SolveFirst() const
{
	auto [t, schedule] = ParseInput();
	const auto& wait = schedule | std::views::transform([&](int n)
	{
		return (t / n + 1) * n - t;
	});
	auto it = std::ranges::min_element(wait);
	int index = std::distance(wait.begin(), it);
	return new Result(schedule[index] * (*it));
}

ResultBase* Day13::SolveSecond() const
{
	auto [_, schedule] = ParseInput();
	int64_t t = 0;
	int64_t inc = 1;
	for(int i = 0; i < schedule.size(); i++)
	{
		if(schedule[i] == std::numeric_limits<int>::max())
		{
			continue;
		}
		while((t + i) % schedule[i] != 0)
		{
			t += inc;
		}
		inc *= schedule[i];
	}
	return new Result(t);
}

Day13::Schedule Day13::ParseInput() const
{
	auto lines = common::SplitStr(input.lines[1], ",") | std::views::transform([](const std::string& n)
	{
		return n != "x" ? std::stoi(n) : std::numeric_limits<int>::max();
	});
	return Schedule(std::stoi(input.lines[0]), std::vector<int>(lines.begin(), lines.end()));
}
