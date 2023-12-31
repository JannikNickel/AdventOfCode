using System.Linq;

namespace AdventOfCode2022.Day04
{
    public class Solution : SolutionBase
    {
        public Solution() : base(4, "Camp Cleanup") { }

        public override object? SolveFirst()
        {
            return Input.Lines
                .Select(n => ParseLine(n))
                .Where(n => n.largeFrom <= n.smallFrom && n.largeTo >= n.smallTo)
                .Count();
        }

        public override object? SolveSecond()
        {
            return Input.Lines
                .Select(n => ParseLine(n))
                .Where(n => n.largeFrom <= n.smallTo && n.largeTo >= n.smallFrom)
                .Count();
        }

        private static (int largeFrom, int largeTo, int smallFrom, int smallTo) ParseLine(string line)
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
