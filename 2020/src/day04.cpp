#include "day04.h"
#include "common/stringutils.h"
#include <ranges>
#include <cctype>
#include <regex>

const std::string FIELDS[] =
{
	"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"
};

ResultBase* Day04::SolveFirst() const
{
	return new Result(CountValid(false));
}

ResultBase* Day04::SolveSecond() const
{
	return new Result(CountValid(true));
}

int Day04::CountValid(bool withFields) const
{
	std::vector<std::string> passports = ParseInput();
	return std::ranges::count_if(passports, [&](const std::string& n)
	{
		return std::ranges::all_of(FIELDS, [&](const std::string& field) { return n.find(field) != std::string::npos; }) && (!withFields || FieldsValid(n));
	});
}

bool Day04::FieldsValid(const std::string& passport) const
{
	return ValidateYear(passport, FIELDS[0], 1920, 2002)
		&& ValidateYear(passport, FIELDS[1], 2010, 2020)
		&& ValidateYear(passport, FIELDS[2], 2020, 2030)
		&& ValidateHeight(passport, FIELDS[3])
		&& ValidateRegex(passport, FIELDS[4], "#[0-9a-f]{6}")
		&& ValidateRegex(passport, FIELDS[5], "amb|blu|brn|gry|grn|hzl|oth")
		&& ValidateRegex(passport, FIELDS[6], "^[0-9]{9}$");
}

bool Day04::ValidateYear(const std::string& passport, const std::string& key, int min, int max) const
{
	std::string value = ExtractValue(passport, key);
	return value.length() == 4
		&& std::ranges::all_of(value, [](char c){ return std::isdigit(c); })
		&& std::stoi(value) >= min && std::stoi(value) <= max;
}

bool Day04::ValidateHeight(const std::string& passport, const std::string& key) const
{
	std::string value = ExtractValue(passport, key);
	std::string unit = value.substr(value.length() - 2);
	value.erase(value.length() - 2, 2);
	return (unit == "cm" || unit == "in")
		&& std::ranges::all_of(value, [](char c){ return std::isdigit(c); })
		&& (unit == "cm" ? (std::stoi(value) >= 150 && std::stoi(value) <= 193) : (std::stoi(value) >= 59 && std::stoi(value) <= 76));
}

bool Day04::ValidateRegex(const std::string& passport, const std::string& key, const std::string& regex) const
{
	return std::regex_match(ExtractValue(passport, key), std::regex(regex));
}

std::string Day04::ExtractValue(const std::string& passport, const std::string& key) const
{
	size_t start = passport.find(key) + key.length() + 1;
	size_t end = common::FindInStr(passport, { " ", "\n" }, start);
	return passport.substr(start, end - start);
}

std::vector<std::string> Day04::ParseInput() const
{
	std::string input = common::ReplaceStr(this->input.raw, "\r\n", "\n");
	return common::SplitStr(input, "\n\n");
}
