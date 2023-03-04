#pragma once
#include "solutionbase.h"
#include <vector>

struct Day12 : SolutionBase
{
	Day12(const Input& input) : SolutionBase(input, 12, "Rain Risk") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:
	struct Action
	{
		char dir;
		int value;

		Action(char dir, int value) : dir(dir), value(value) { }
	};

	int Simulate(const std::vector<Action>& actions, bool useWaypoint = false) const;
	std::vector<Action> ParseInput() const;
};
