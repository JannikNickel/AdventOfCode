#pragma once
#include "solutionbase.h"

struct Day00 : SolutionBase
{
	Day00(const Input& input) : SolutionBase(input, 0, "") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:
	void ParseInput() const;
};
