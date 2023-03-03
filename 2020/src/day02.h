#pragma once
#include "solutionbase.h"
#include <vector>
#include <utility>
#include <functional>

struct Day02 : SolutionBase
{
	Day02(const Input& input) : SolutionBase(input, 2, "Password Philosophy") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:
	struct Policy
	{
		int min;
		int max;
		char c;

		Policy(int min, int max, char c) : min(min), max(max), c(c) { }
	};

	size_t CountValidPasswords(const std::function<bool (const Policy&, const std::string&)>& validation) const;
	std::vector<std::pair<Policy, std::string>> ParseInput() const;
};
