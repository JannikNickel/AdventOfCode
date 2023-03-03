#include "day07.h"
#include <ranges>
#include <regex>
#include <deque>

ResultBase* Day07::SolveFirst() const
{
	const BagMap bags = ParseInput();
	int result = std::ranges::count_if(bags, [&](const auto& n) { return CanContain(bags, n.first, "shiny gold"); });
	return new Result(result);
}

ResultBase* Day07::SolveSecond() const
{
	return new Result(CountContaining(ParseInput(), "shiny gold") - 1);
}

bool Day07::CanContain(const BagMap& bags, const std::string& root, const std::string& target) const
{
	bool contains = false;
	for(const auto& [name, amount] : bags.at(root))
	{
		if(name == target)
		{
			return true;
		}
		contains |= CanContain(bags, name, target);
	}
	return contains;
}

int Day07::CountContaining(const BagMap& bags, const std::string& root) const
{
	int sum = 1;
	for(const auto& [name, amount] : bags.at(root))
	{
		sum += amount * CountContaining(bags, name);
	}
	return sum;
}

Day07::BagMap Day07::ParseInput() const
{
	BagMap bags = {};
	std::regex reg = std::regex("\\d+ [a-z]* [a-z]*");
	for(const std::string& line : input.lines)
	{
		std::string name = line.substr(0, line.find(" bags"));
		std::vector<std::pair<std::string, int>> content = {};
		for(std::sregex_iterator i = std::sregex_iterator(line.begin(), line.end(), reg); i != std::sregex_iterator(); i++)
		{
			std::string s = i->str();
			size_t start = s.find(' ');
			int amount = std::stoi(s.substr(0, start));
			content.push_back(std::make_pair(s.erase(0, start + 1), amount));
		}
		bags.insert(std::make_pair(name, content));
	}
	return bags;
}
