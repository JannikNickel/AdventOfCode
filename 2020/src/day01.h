#pragma once
#include "solutionbase.h"
#include <vector>

struct Day01 : SolutionBase
{
	Day01(const Input& input) : SolutionBase(input, 1, "Report Repair") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:
	std::vector<int> ParseInput() const;
};
