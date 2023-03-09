#pragma once
#include "solutionbase.h"
#include <deque>
#include <utility>

struct Day22 : SolutionBase
{
	Day22(const Input& input) : SolutionBase(input, 22, "Crab Combat") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:
	uint64_t Play(bool recursive) const;
	int Game(std::deque<int> p0, std::deque<int> p1, bool recursive = false, std::deque<int>* winningDeck = nullptr) const;
	std::pair<std::deque<int>, std::deque<int>> ParseInput() const;
};
