#pragma once
#include "solutionbase.h"
#include "common/grid.h"
#include "common/vec2.h"

struct Day03 : SolutionBase
{
	Day03(const Input& input) : SolutionBase(input, 3, "Toboggan Trajectory") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:
	int CountTrees(const common::Grid<int>& map, common::Vec2<int> start, common::Vec2<int> step) const;
	common::Grid<int> ParseInput() const;
};
