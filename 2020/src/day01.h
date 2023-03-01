#pragma once
#include "solutionbase.h"

struct Day01 : SolutionBase
{
	Day01(const Input& input) : SolutionBase(input, 1, "") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:

};
