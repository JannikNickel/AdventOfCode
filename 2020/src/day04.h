#pragma once
#include "solutionbase.h"
#include <vector>
#include <string>

struct Day04 : SolutionBase
{
	Day04(const Input& input) : SolutionBase(input, 4, "Passport Processing") { }

protected:
	ResultBase* SolveFirst() const override;
	ResultBase* SolveSecond() const override;

private:
	int CountValid(bool withFields = false) const;
	bool FieldsValid(const std::string& passport) const;
	bool ValidateYear(const std::string& passport, const std::string& key, int min, int max) const;
	bool ValidateHeight(const std::string& passport, const std::string& key) const;
	bool ValidateRegex(const std::string& passport, const std::string& key, const std::string& regex) const;
	std::string ExtractValue(const std::string& passport, const std::string& key) const;
	std::vector<std::string> ParseInput() const;
};
