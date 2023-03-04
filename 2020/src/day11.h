#pragma once
#include "solutionbase.h"
#include "common/grid.h"

struct Day11 : SolutionBase
{
	Day11(const Input& input) : SolutionBase(input, 11, "Seating System") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:
	template<typename T>
	using Grid = common::Grid<T>;

	int Simulate(Grid<char>& map, int leaveThreshold, bool diagonalSearch = false) const;
	int DiagonalSearch(const Grid<char>& map, size_t x, size_t y) const;
	Grid<char> ParseInput() const;
};
