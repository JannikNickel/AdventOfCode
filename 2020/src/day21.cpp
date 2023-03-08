#include "day21.h"
#include "common/stringutils.h"
#include <ranges>
#include <unordered_set>
#include <deque>
#include <algorithm>
#include <numeric>

ResultBase* Day21::SolveFirst() const
{
	std::vector<Food> foods = ParseInput();
	std::unordered_map<std::string, std::string> map = BuildAllergenMap(foods);
	int res = std::accumulate(foods.begin(), foods.end(), 0, [&](int acc, const Food& food)
	{
		return acc + std::ranges::count_if(food.ingredients, [&](const std::string& n)
		{
			return !std::ranges::any_of(map, [&](const auto& pair) { return pair.second == n; });
		});
	});
	return new Result(res);
}

ResultBase* Day21::SolveSecond() const
{
	std::unordered_map<std::string, std::string> map = BuildAllergenMap(ParseInput());
	std::vector<std::string> ingredients = {};
	std::ranges::transform(map, std::back_inserter(ingredients), [](const auto& pair) { return pair.second; });
	std::ranges::sort(ingredients, [&](const std::string& a, const std::string& b)
	{
		const std::string& aKey = std::ranges::find_if(map, [&](const auto& pair) { return pair.second == a; })->first;
		const std::string& bKey = std::ranges::find_if(map, [&](const auto& pair) { return pair.second == b; })->first;
		return aKey < bKey;
	});
	std::string res = std::accumulate(ingredients.begin(), ingredients.end(), std::string(), [&](const std::string& acc, const std::string& n)
	{
		return acc + (!acc.empty() ? "," : "") + n;
	});
	return new Result(res);
}

std::unordered_map<std::string, std::string> Day21::BuildAllergenMap(const std::vector<Food>& foods) const
{
	std::unordered_map<std::string, std::unordered_set<std::string>> possibleAllergens = {};
	for(const Food& food : foods)
	{
		for(const std::string allergen : food.allergens)
		{
			if(!possibleAllergens[allergen].empty())
			{
				std::unordered_set<std::string> intersection = {};
				for(const std::string& ing : possibleAllergens[allergen])
				{
					if(std::ranges::find(food.ingredients, ing) != food.ingredients.end())
					{
						intersection.insert(ing);
					}
				}
				possibleAllergens[allergen] = intersection;
			}
			else
			{
				possibleAllergens[allergen] = std::unordered_set<std::string>(food.ingredients.begin(), food.ingredients.end());
			}
		}
	}

	std::deque<std::string> open = { std::ranges::find_if(possibleAllergens, [](const auto& n) { return n.second.size() != 1; })->first };
	std::unordered_set<std::string> closed = {};
	while(!open.empty())
	{
		const std::string known = open.front();
		open.pop_front();
		const std::string& knownVal = *possibleAllergens[known].begin();

		for(const auto& [key, value] : possibleAllergens)
		{
			if(key != known)
			{
				std::unordered_set<std::string> next = {};
				std::ranges::copy_if(value, std::inserter(next, next.begin()), [&](const auto& n) { return n != knownVal; });
				possibleAllergens[key] = next;
				if(next.size() == 1 && !closed.contains(key))
				{
					closed.insert(key);
					open.push_back(key);
				}
			}
		}
	}

	std::unordered_map<std::string, std::string> mapping = {};
	for(const auto& [allergen, potAllergens] : possibleAllergens)
	{
		mapping[allergen] = *potAllergens.begin();
	}
	return mapping;
}

std::vector<Day21::Food> Day21::ParseInput() const
{
	std::vector<Food> foods = {};
	for(const std::string& line : input.lines)
	{
		std::vector<std::string> parts = common::SplitStr(line, " (contains ");
		parts[1].pop_back();
		foods.push_back(Food(common::SplitStr(parts[0], " "), common::SplitStr(parts[1], ", ")));
	}
	return foods;
}
