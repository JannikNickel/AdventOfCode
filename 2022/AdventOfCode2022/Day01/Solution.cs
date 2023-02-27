using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AdventOfCode2022.Day01
{
    public class Solution : SolutionBase
    {
        public Solution() : base(1, "Calorie Counting")
        {

        }

        public override object? SolveFirst()
        {
            int maxCalories = 0;
            int calories = 0;
            foreach(string line in Input.Lines)
            {
                if(string.IsNullOrEmpty(line.Trim()))
                {
                    maxCalories = Math.Max(calories, maxCalories);
                    calories = 0;
                    continue;
                }

                calories += int.Parse(line);
            }
            maxCalories = Math.Max(calories, maxCalories);
            return maxCalories;
        }

        public override object? SolveSecond()
        {
            List<int> elves = new List<int>();
            int calories = 0;
            foreach(string line in Input.Lines)
            {
                if(string.IsNullOrEmpty(line.Trim()))
                {
                    elves.Add(calories);
                    calories = 0;
                    continue;
                }

                calories += int.Parse(line);
            }
            elves.Add(calories);
            return elves.OrderByDescending(n => n).Take(3).Sum();
        }
    }
}
