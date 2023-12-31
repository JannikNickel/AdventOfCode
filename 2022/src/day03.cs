using System.Linq;

namespace AdventOfCode2022.Day03
{
    public class Solution : SolutionBase
    {
        public Solution() : base(3, "Rucksack Reorganization") { }

        public override object? SolveFirst()
        {
            int sum = 0;
            foreach(string line in Input.Lines)
            {
                string l = line[..(line.Length / 2)];
                string r = line[(line.Length / 2)..];
                sum += l.Where(n => r.Contains(n)).Distinct().Sum(n => GetItemPriority(n));
            }
            return sum;
        }

        public override object? SolveSecond()
        {
            int sum = 0;
            for(int i = 0;i < Input.Lines.Length;i += 3)
            {
                char item = Input.Lines[i].Where(n => Input.Lines[i + 1].Contains(n) && Input.Lines[i + 2].Contains(n)).FirstOrDefault();
                sum += GetItemPriority(item);
            }
            return sum;
        }

        private static int GetItemPriority(char item)
        {
            if(item >= 'a' && item <= 'z')
            {
                return item - 'a' + 1;
            }
            return item - 'A' + 27;
        }
    }
}
