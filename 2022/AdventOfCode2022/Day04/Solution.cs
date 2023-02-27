using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AdventOfCode2022.Day04
{
    public class Solution : SolutionBase
    {
        public Solution() : base(4, "Camp Cleanup")
        {
            
        }

        public override object? SolveFirst()
        {
            int amount = 0;
            foreach(string line in Input.Lines)
            {
                (int largeFrom, int largeTo, int smallFrom, int smallTo) = ParseLine(line);
                if(largeFrom <= smallFrom && largeTo >= smallTo)
                {
                    amount++;
                }
            }
            return amount;
        }

        public override object? SolveSecond()
        {
            int amount = 0;
            foreach(string line in Input.Lines)
            {
                (int largeFrom, int largeTo, int smallFrom, int smallTo) = ParseLine(line);
                if(largeFrom <= smallTo && largeTo >= smallFrom)
                {
                    amount++;
                }
            }
            return amount;
        }

        private (int largeFrom, int largeTo, int smallFrom, int smallTo) ParseLine(string line)
        {
            string[] parts = line.Split(new char[] { '-', ',' });
            int from1 = int.Parse(parts[0]);
            int to1 = int.Parse(parts[1]);
            int from2 = int.Parse(parts[2]);
            int to2 = int.Parse(parts[3]);
            if(to1 - from1 > to2 - from2)
            {
                return (from1, to1, from2, to2);
            }
            return (from2, to2, from1, to1);
        }
    }
}
