#pragma once
#include "solutionbase.h"
#include <vector>

struct Day15 : SolutionBase
{
	Day15(const Input& input) : SolutionBase(input, 15, "Rambunctious Recitation") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:
	uint64_t FindNumber(const std::vector<uint64_t>& numbers, uint64_t steps) const;
	std::vector<uint64_t> ParseInput() const;
};
