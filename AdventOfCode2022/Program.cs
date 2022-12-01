using System;
using System.Diagnostics;
using System.Linq;

namespace AdventOfCode2022
{
    internal class Program
    {
        private const ConsoleColor defaultColor = ConsoleColor.DarkGray;
        private const ConsoleColor resultColor = ConsoleColor.White;

        static void Main(string[] args)
        {
            Console.ForegroundColor = defaultColor;

            SolutionBase? solution = null;
            if(args.Length > 0)
            {
                if(int.TryParse(args[0], out int day))
                {
                    if(day >= 1 && day <= 25)
                    {
                        solution = SolutionBase.Solutions().FirstOrDefault(n => n.day == day);
                    }
                }
            }

            if(solution == null)
            {
                Console.WriteLine("Fallback to last solution!");
                solution = SolutionBase.Solutions().LastOrDefault();
                if(solution == null)
                {
                    Console.WriteLine("No solutions found!");
                    Console.ReadKey();
                    return;
                }
                Console.WriteLine();
            }

            Console.WriteLine($"Preparing Solution {solution.day:D2} - {solution.title}");
            solution.Prepare();

            Solve(solution, 1);
            Solve(solution, 2);

            Console.ReadLine();
        }

        private static void Solve(SolutionBase solution, int part)
        {
            Console.WriteLine($"Solving Part {part}...");
            Stopwatch sw = Stopwatch.StartNew();
            object? result = part == 1 ? solution.SolveFirst() : solution.SolveSecond();
            Console.WriteLine($"Solved Part {part} ({Math.Round(sw.Elapsed.TotalMilliseconds, 2)}ms)");
            Console.WriteLine("Result:");
            Console.ForegroundColor = resultColor;
            Console.WriteLine(result ?? "null");
            Console.ForegroundColor = defaultColor;
            Console.WriteLine();
        }
    }
}