#include "day02.h"
#include "common/stringutils.h"
#include <ranges>

ResultBase* Day02::SolveFirst() const
{
	auto amount = CountValidPasswords([](const Policy& policy, const std::string& pw)
	{
		int c = std::ranges::count(pw, policy.c);
		return c >= policy.min && c <= policy.max;
	});
	return new Result(amount);
}

ResultBase* Day02::SolveSecond() const
{
	auto amount = CountValidPasswords([](const Policy& policy, const std::string& pw)
	{
		return (pw[policy.min - 1] == policy.c) != (pw[policy.max - 1] == policy.c);
	});
	return new Result(amount);
}

size_t Day02::CountValidPasswords(const std::function<bool(const Policy&, const std::string&)>& validation) const
{
	std::vector<std::pair<Day02::Policy, std::string>> passwords = ParseInput();
	return static_cast<size_t>(std::ranges::count_if(passwords, [&](const std::pair<Day02::Policy, std::string>& pair)
	{
		const auto& [policy, pw] = pair;
		return validation(policy, pw);
	}));
}

std::vector<std::pair<Day02::Policy, std::string>> Day02::ParseInput() const
{	
	std::vector<std::pair<Day02::Policy, std::string>> res = {};
	for(const std::string& line : input.lines)
	{
		std::vector<std::string> parts = common::SplitStr(line, ":");
		char character = parts[0].back();
		parts[0].erase(parts[0].length() - 2);
		std::vector<std::string> range = common::SplitStr(parts[0], "-");
		res.push_back(std::make_pair(Policy(std::stoi(range[0]), std::stoi(range[1]), character), common::TrimStr(parts[1])));
	}
	return res;
}
