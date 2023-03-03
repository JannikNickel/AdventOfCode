#pragma once
#include "solutionbase.h"
#include <vector>

struct Day09 : SolutionBase
{
	Day09(const Input& input) : SolutionBase(input, 9, "Encoding Error") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:
	uint64_t FindError(const std::vector<uint64_t>& numbers, int preambleLen) const;
	uint64_t FindContiguous(const std::vector<uint64_t>& numbers, uint64_t target) const;
	std::vector<uint64_t> ParseInput() const;
};
