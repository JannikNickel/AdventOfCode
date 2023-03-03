#include "day06.h"
#include "common/stringutils.h"
#include <ranges>

ResultBase* Day06::SolveFirst() const
{
	int sum = 0;
	std::ranges::for_each(std::views::transform(ParseInput(), [&](const std::string& n) { return CalcAnswerSum(n); }), [&](int n)
	{
		sum += n;
	});
	return new Result(sum);
}

ResultBase* Day06::SolveSecond() const
{
	int sum = 0;
	std::ranges::for_each(std::views::transform(ParseInput(), [&](const std::string& n) { return CalcAnswerSum(n, true); }), [&](int n)
	{
		sum += n;
	});
	return new Result(sum);
}

std::vector<std::string> Day06::ParseInput() const
{
	std::string input = common::ReplaceStr(this->input.raw, "\r\n", "\n");
	return common::SplitStr(input, "\n\n");
}

int Day06::CalcAnswerSum(const std::string& answer, bool all) const
{
	int questions[26] = {};
	for(char c : answer)
	{
		if(c >= 'a' && c <= 'z')
		{
			questions['z' - c]++;
		}
	}
	int groupSize = std::ranges::count(answer, '\n') + 1;
	return std::ranges::count_if(questions, [&](int n)
	{
		return all ? n == groupSize : n > 0;
	});
}
