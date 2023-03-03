#pragma once
#include "solutionbase.h"
#include <utility>

struct Day05 : SolutionBase
{
	Day05(const Input& input) : SolutionBase(input, 5, "Passport Processing") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:
	static int CalcSeatId(const std::string& seat);
};
