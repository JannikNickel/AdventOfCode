#pragma once
#include "solutionbase.h"
#include <vector>
#include <utility>

struct Day13 : SolutionBase
{
	Day13(const Input& input) : SolutionBase(input, 13, "Shuttle Search") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:
	using Schedule = std::pair<int, std::vector<int>>;

	Schedule ParseInput() const;
};
