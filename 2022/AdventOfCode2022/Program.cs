using System;
using System.Collections;
using System.Collections.Generic;
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

            IEnumerable<SolutionBase>? solutions = null;
            if(args.Length > 0)
            {
                if(args[0] == "--day" && int.TryParse(args[1], out int day))
                {
                    if(day >= 1 && day <= 25)
                    {
                        SolutionBase? solution = SolutionBase.Solutions().FirstOrDefault(n => n.day == day);
                        solutions = solution != null ? new SolutionBase[] { solution } : null;
                    }
                }
                else if(args[0] == "--all")
                {
                    solutions = SolutionBase.Solutions();
                }
            }

            if(solutions == null)
            {
                Console.WriteLine("Fallback to last solution!");
                SolutionBase? solution = SolutionBase.Solutions().LastOrDefault();
                solutions = solution != null ? new SolutionBase[] { solution } : null;
                if(solutions == null)
                {
                    Console.WriteLine("No solutions found!");
                    Console.ReadKey();
                    return;
                }
                Console.WriteLine();
            }

            foreach(SolutionBase solution in solutions)
            {
                Console.WriteLine($"Preparing Solution {solution.day:D2} - {solution.title}");
                solution.Prepare();

                Solve(solution, 1);
                Solve(solution, 2);
            }
        }

        private static void Solve(SolutionBase solution, int part)
        {
            Console.WriteLine($"Solving Part {part}...");
            Stopwatch sw = Stopwatch.StartNew();
            object? result = part == 1 ? solution.SolveFirst() : solution.SolveSecond();
            sw.Stop();
            Console.WriteLine($"Solved Part {part} ({Math.Round(sw.Elapsed.TotalMilliseconds, 2)}ms)");
            Console.WriteLine("Result:");
            Console.ForegroundColor = resultColor;
            Console.WriteLine(result ?? "null");
            Console.ForegroundColor = defaultColor;
            Console.WriteLine();
        }
    }
}