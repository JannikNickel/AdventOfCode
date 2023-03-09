#include "day25.h"

ResultBase* Day25::SolveFirst() const
{
	auto [cardKey, doorKey] = ParseInput();
	uint64_t cardLoop = CalcLoopSize(cardKey);
	return new Result(CalcEncryptionKey(doorKey, cardLoop));
}

ResultBase* Day25::SolveSecond() const
{
	//Day 25 only has 1 puzzle
	return nullptr;
}

uint64_t Day25::CalcLoopSize(uint64_t key) const
{
	uint64_t v = 1;
	for(uint64_t i = 0;; i++)
	{
		v = v * 7 % 20201227;
		if(v == key)
		{
			return i + 1;
		}
	}
	return 0;
}

uint64_t Day25::CalcEncryptionKey(uint64_t subjectNumber, uint64_t loopSize) const
{
	uint64_t v = 1;
	for(uint64_t i = 0; i < loopSize; i++)
	{
		v = v * subjectNumber % 20201227;
	}
	return v;
}

std::pair<uint64_t, uint64_t> Day25::ParseInput() const
{
	return std::make_pair(std::stoull(input.lines[0]), std::stoull(input.lines[1]));
}
