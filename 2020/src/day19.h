#pragma once
#include "solutionbase.h"
#include <vector>
#include <utility>
#include <unordered_map>

struct Day19 : SolutionBase
{
	Day19(const Input& input) : SolutionBase(input, 19, "Monster Messages") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:
	using Derivation = std::vector<int>;

	struct Rule
	{
		std::vector<Derivation> derivations;
		char terminal;

		Rule(const std::vector<Derivation>& derivations) : derivations(derivations), terminal(0) { }
		Rule(const char& terminal) : derivations({}), terminal(terminal) { }
		Rule() : derivations({}), terminal(0) { }
	};

	std::string BuildRegStr(const std::unordered_map<int, Rule>& rules, int rule = 0, int depth = 0) const;
	std::pair<std::unordered_map<int, Rule>, std::vector<std::string>> ParseInput() const;
};
