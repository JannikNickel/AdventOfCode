#pragma once
#include "solutionbase.h"
#include <unordered_map>
#include <utility>

struct Day07 : SolutionBase
{
	Day07(const Input& input) : SolutionBase(input, 7, "Handy Haversacks") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:
	using BagMap = std::unordered_map<std::string, std::vector<std::pair<std::string, int>>>;

	bool CanContain(const BagMap& bags, const std::string& root, const std::string& target) const;
	int CountContaining(const BagMap& bags, const std::string& root) const;
	BagMap ParseInput() const;
};
