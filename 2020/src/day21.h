#pragma once
#include "solutionbase.h"
#include <vector>
#include <unordered_map>

struct Day21 : SolutionBase
{
	Day21(const Input& input) : SolutionBase(input, 21, "Allergen Assessment") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:
	struct Food
	{
		std::vector<std::string> ingredients;
		std::vector<std::string> allergens;

		Food(std::vector<std::string> ingredients, std::vector<std::string> allergens) : ingredients(ingredients), allergens(allergens) { }
	};

	std::unordered_map<std::string, std::string> BuildAllergenMap(const std::vector<Food>& foods) const;
	std::vector<Food> ParseInput() const;
};
