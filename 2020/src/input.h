#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

struct Input
{
	const std::string raw;
	const std::vector<std::string> lines;

	static Input FromFile(std::string path)
	{
		std::ifstream file = std::ifstream(path);
		std::stringstream stream;
		stream << file.rdbuf();
		std::vector<std::string> lines = {};
		std::string line;
		while(std::getline(stream, line))
		{
			lines.push_back(line);
		}
		return Input { .raw = stream.str(), .lines = lines };
	}
};
