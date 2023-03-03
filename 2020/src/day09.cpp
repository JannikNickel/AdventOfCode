#include "day09.h"
#include <ranges>
#include <utility>

const int PREAMBLE_LEN = 25;

ResultBase* Day09::SolveFirst() const
{
	return new Result(FindError(ParseInput(), PREAMBLE_LEN));
}

ResultBase* Day09::SolveSecond() const
{
	std::vector<uint64_t> numbers = ParseInput();
	int error = FindError(numbers, PREAMBLE_LEN);
	return new Result(FindContiguous(numbers, error));
}

uint64_t Day09::FindError(const std::vector<uint64_t>& numbers, int preambleLen) const
{
	for(size_t i = preambleLen; i < numbers.size(); i++)
	{
		int target = numbers[i];
		auto from = numbers.begin() + i - preambleLen;
		auto to = from + preambleLen;
		for(auto i = from; i != to; i++)
		{
			for(auto k = from; k != to; k++)
			{
				if(i != k && *i + *k == target)
				{
					goto END;
				}
			}
		}
		return target;

		END:;
	}
	return 0;
}

uint64_t Day09::FindContiguous(const std::vector<uint64_t>& numbers, uint64_t target) const
{
	for(size_t i = 0; i < numbers.size(); i++)
	{
		uint64_t sum = numbers[i];
		uint64_t min = sum;
		uint64_t max = sum;
		for(size_t k = i + 1; k < numbers.size(); k++)
		{
			sum += numbers[k];
			min = std::min(min, numbers[k]);
			max = std::max(max, numbers[k]);
			if(sum >= target)
			{
				if(sum == target)
				{
					return min + max;
				}
				break;
			}
		}
	}
	return 0;
}

std::vector<uint64_t> Day09::ParseInput() const
{
	auto numbers = input.lines | std::views::transform([](const std::string& n)
	{
		return std::stoull(n);
	});
	return std::vector<uint64_t>(numbers.begin(), numbers.end());
}
