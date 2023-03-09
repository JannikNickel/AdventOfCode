#pragma once
#include "solutionbase.h"
#include <vector>
#include <unordered_map>

struct Day23 : SolutionBase
{
	Day23(const Input& input) : SolutionBase(input, 23, "Crab Cups") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:
	std::unordered_map<uint32_t, uint32_t> Simulate(std::vector<uint32_t>& numbers, uint32_t steps) const;
	std::vector<uint32_t> ParseInput() const;
};
