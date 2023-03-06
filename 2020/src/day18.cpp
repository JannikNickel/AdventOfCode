#include "day18.h"
#include "common/stringutils.h"
#include <ranges>
#include <numeric>
#include <cctype>

ResultBase* Day18::SolveFirst() const
{
	std::vector<std::string> expressions = ParseInput();
	int64_t sum = std::accumulate(expressions.begin(), expressions.end(), static_cast<int64_t>(0), [&](int64_t acc, const std::string& n) { return acc + Solve(n, n.size() - 1); });
	return new Result(sum);
}

ResultBase* Day18::SolveSecond() const
{
	std::vector<std::string> expressions = ParseInput();
	std::vector<std::string> precedenceAdjusted = {};
	for(const std::string& expression : expressions)
	{
		std::string n = expression;
		for(size_t i = 0; i < n.size(); i++)
		{
			if(n[i] == '+')
			{
				InsertParen(n, i++, -1);
				InsertParen(n, i, 1);
			}
		}
		precedenceAdjusted.push_back(n);
	}
	int64_t sum = std::accumulate(precedenceAdjusted.begin(), precedenceAdjusted.end(), static_cast<int64_t>(0), [&](int64_t acc, const std::string& n) { return acc + Solve(n, n.size() - 1); });
	return new Result(sum);
}

void Day18::InsertParen(std::string& s, int64_t start, int64_t dir) const
{
	char open = dir > 0 ? '(' : ')';
	char close = dir > 0 ? ')' : '(';
	int stack = 0;
	do
	{
		start += dir;
		char c = s[start];
		stack += (c == open ? 1 : (c == close ? -1 : 0));
	}
	while(stack != 0);
	start += (std::isdigit(s[start]) && dir > 0);
	s.insert(start, 1, close);
}

std::pair<int64_t, int64_t> Day18::SolveParenthesis(const std::string& expression, int64_t start) const
{
	int stack = 1;
	int64_t from = start--;
	while(stack != 0)
	{
		char c = expression[start--];
		stack += (c == ')' ? 1 : (c == '(' ? -1 : 0));
	}
	int64_t to = start + 2;
	std::string next = expression.substr(to, from - to);
	return std::make_pair(Solve(next, next.size() - 1), from - start - 1);
}

int64_t Day18::Solve(const std::string& expression, int64_t start) const
{
	int64_t v = expression[start] - '0';
	if(expression[start] == ')')
	{
		auto [value, off] = SolveParenthesis(expression, start);
		v = value;
		start -= off;
	}
	start--;
	if(start > 0)
	{
		int64_t lhs = Solve(expression, start - 1);
		if(expression[start] == '+')
		{
			v += lhs;
		}
		else if(expression[start] == '*')
		{
			v *= lhs;
		}
	}
	return v;
}

std::vector<std::string> Day18::ParseInput() const
{
	auto t = input.lines | std::views::transform([](const std::string& n) { return common::ReplaceStr(n, " ", ""); });
	return std::vector<std::string>(t.begin(), t.end());
}
