#pragma once
#include "solutionbase.h"
#include "common/vec4.h"
#include <unordered_set>

struct Day17 : SolutionBase
{
	Day17(const Input& input) : SolutionBase(input, 17, "Conway Cubes") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:
	using Point = common::Vec4<int>;

	int Simulate(int steps, int dimensions) const;
	std::unordered_set<Point> Simulate(const std::unordered_set<Point>& points, int dimensions) const;
	std::unordered_set<Point> ParseInput() const;
};
