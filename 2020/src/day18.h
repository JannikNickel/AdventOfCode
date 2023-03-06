#pragma once
#include "solutionbase.h"
#include <vector>
#include <utility>

struct Day18 : SolutionBase
{
	Day18(const Input& input) : SolutionBase(input, 18, "Operation Order") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:
	void InsertParen(std::string& s, int64_t start, int64_t dir) const;
	std::pair<int64_t, int64_t> SolveParenthesis(const std::string& expression, int64_t start) const;
	int64_t Solve(const std::string& expression, int64_t start) const;
	std::vector<std::string> ParseInput() const;
};
