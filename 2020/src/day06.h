#pragma once
#include "solutionbase.h"
#include <vector>

struct Day06 : SolutionBase
{
	Day06(const Input& input) : SolutionBase(input, 6, "Custom Customs") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:
	std::vector<std::string> ParseInput() const;
	int CalcAnswerSum(const std::string& answer, bool all = false) const;
};
