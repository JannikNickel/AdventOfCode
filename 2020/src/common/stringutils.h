#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <initializer_list>
#include <utility>

namespace common
{
	static std::vector<std::string> SplitStr(const std::string& source, const std::string& delimiter)
	{
		size_t start = 0;
		size_t end;
		std::vector<std::string> res = {};
		while((end = source.find(delimiter, start)) != std::string::npos)
		{
			res.push_back(source.substr(start, end - start));
			start = end + delimiter.length();
		}
		res.push_back(source.substr(start));
		return res;
	}

	static std::string ReplaceStr(const std::string& source, const std::string& search, const std::string& replacement)
	{
		std::string res = source;
		size_t pos = 0;
		while((pos = source.find(search, pos)) != std::string::npos)
		{
			res.replace(pos, search.length(), replacement);
			pos += replacement.length();
		}
		return res;
	}

	static std::string TrimStr(const std::string& source)
	{
		auto start = std::find_if_not(source.begin(), source.end(), std::isspace);
		auto end = std::find_if_not(source.rbegin(), source.rend(), std::isspace).base();
		return start < end ? std::string(start, end) : source;
	}

	static size_t FindInStr(const std::string& source, std::initializer_list<std::string> searches, size_t start)
	{
		size_t result = std::string::npos;
		for(const std::string& search : searches)
		{
			size_t index = source.find(search, start);
			result = std::min(result, index);
		}
		return result;
	}
}
