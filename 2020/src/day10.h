#pragma once
#include "solutionbase.h"
#include <vector>
#include <unordered_set>
#include <unordered_map>

struct Day10 : SolutionBase
{
	Day10(const Input& input) : SolutionBase(input, 10, "Adapter Array") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:
	int CountJoltageDiff(const std::vector<int>& adapters) const;
	uint64_t CountValidConfigs(const std::unordered_set<int>& adapters, std::unordered_map<int, uint64_t>& cache, int adapter, int target) const;
	std::vector<int> ParseInput() const;
};
