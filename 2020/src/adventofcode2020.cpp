#include <iostream>
#include <format>
#include <chrono>
#include "input.h"
#include "solutions.h"

void Solve(int day);
void Solve(std::unique_ptr<SolutionBase>& solution, int part);
template<Printable T>
void PrintInfoLine(T t);
template<Printable T>
void PrintErrorLine(T t);

int main(int argc, char* argv[])
{
	std::vector<int> solutionsToRun = {};
	if(argc > 1)
	{
		std::string arg1 = std::string(argv[1]);
		if(argc >= 2 && arg1 == "--day")
		{
			try
			{
				int day = std::stoi(argv[2]);
				if(day <= 0 || day > Solutions::COUNT)
				{
					throw std::out_of_range("");
				}
			}
			catch(const std::exception&)
			{
				PrintErrorLine(std::format("Expected number in range [1, {}] after --day argument!", Solutions::COUNT));
				return 1;
			}
		}
		else if(arg1 == "--all")
		{
			for(int i = 1; i <= Solutions::COUNT; i++)
			{
				solutionsToRun.push_back(i);
			}
		}
		else
		{
			PrintErrorLine("Unknown cmd args, running last solution!");
		}
	}

	if(solutionsToRun.size() == 0)
	{
		solutionsToRun.push_back(Solutions::COUNT);
	}

	for(int day : solutionsToRun)
	{
		Solve(day);
	}
}

void Solve(int day)
{
	Input input = Input::FromFile(std::format("/input/{:02}.txt", day));
	std::unique_ptr<SolutionBase> solution = Solutions::GetSolution(day, input);
	Solve(solution, 1);
	Solve(solution, 2);
}

void Solve(std::unique_ptr<SolutionBase>& solution, int part)
{
	PrintInfoLine(std::format("Solving Part {}...", part));
	auto t0 = std::chrono::high_resolution_clock::now();
	std::unique_ptr<ResultBase> result = solution->Solve(part);
	auto t1 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count() * 1e-6;
	PrintInfoLine(std::format("Solved Part {} ({:.5f}ms)", part, duration));
	if(result != nullptr)
	{
		(*result).ToStream(std::cout);
	}
	else
	{
		std::cout << "-";
	}
	std::cout << std::endl << std::endl;
}

template<Printable T>
void PrintInfoLine(T t)
{
	std::cout << "\x1B[90m" << t << "\033[0m" << std::endl;
}

template<Printable T>
void PrintErrorLine(T t)
{
	std::cout << "\x1B[31m" << t << "\033[0m" << std::endl;
}
