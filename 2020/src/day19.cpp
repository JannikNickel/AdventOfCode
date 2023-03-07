#include "day19.h"
#include "common/stringutils.h"
#include <ranges>
#include <cctype>
#include <regex>

ResultBase* Day19::SolveFirst() const
{
	auto [rules, messages] = ParseInput();
	std::regex reg = std::regex(BuildRegStr(rules, 0));
	int valid = std::ranges::count_if(messages, [&](const std::string& n) { return std::regex_match(n, reg); });
	return new Result(valid);
}

ResultBase* Day19::SolveSecond() const
{
	auto [rules, messages] = ParseInput();
	rules[8] = Rule({ Derivation({ 42 }), Derivation({ 42, 8 }) });
	rules[11] = Rule({ Derivation({ 42, 31 }), Derivation({ 42, 11, 31 }) });
	std::regex reg = std::regex(BuildRegStr(rules, 0));
	int valid = std::ranges::count_if(messages, [&](const std::string& n) { return std::regex_match(n, reg); });
	return new Result(valid);
}

std::string Day19::BuildRegStr(const std::unordered_map<int, Day19::Rule>& rules, int rule, int depth) const
{
	if(depth > 50)
	{
		return "";
	}
	const Rule& r = rules.at(rule);
	if(r.terminal != 0)
	{
		return std::string(1, r.terminal);
	}
	bool many = r.derivations.size() > 1;
	std::string sub = many ? "(" : "";
	for(const Derivation& d : r.derivations)
	{
		for(int next : d)
		{
			sub += BuildRegStr(rules, next, depth + 1);
		}
		if(&d != &r.derivations.back())
		{
			sub += "|";
		}
	}
	sub += many ? ")" : "";
	return sub;
}

std::pair<std::unordered_map<int, Day19::Rule>, std::vector<std::string>> Day19::ParseInput() const
{
	std::unordered_map<int, Rule> rules = {};
	std::vector<std::string> messages = {};

	for(const std::string& line : input.lines | std::views::filter([](const std::string& n) { return !n.empty(); }))
	{
		if(std::isdigit(line[0]))
		{
			int start = line.find(':');
			int id = std::stoi(line.substr(0, start));
			std::string remaining = line.substr(start + 2);
			if(remaining[0] == '"')
			{
				rules[id] = Rule(remaining[1]);
			}
			else
			{
				auto derivations = common::SplitStr(remaining, " | ") | std::views::transform([](const std::string& option)
				{
					auto numbers = common::SplitStr(option, " ") | std::views::transform([](const std::string& n) { return std::stoi(n); });
					return Derivation(numbers.begin(), numbers.end());
				});
				rules[id] = Rule(std::vector<Derivation>(derivations.begin(), derivations.end()));
			}
			continue;
		}
		messages.push_back(line);
	}

	return std::make_pair(rules, messages);
}
