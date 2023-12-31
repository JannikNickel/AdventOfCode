using System;
using System.Collections.Generic;

namespace AdventOfCode2022.Day09
{
    public class Solution : SolutionBase
    {
        private static readonly (int x, int y)[] dirVec = new (int x, int y)[4]
        {
            (-1, 0),
            (0, 1),
            (1, 0),
            (0, -1)
        };

        public Solution() : base(9, "Rope Bridge") { }

        public override object? SolveFirst()
        {
            return CalcTrailPositionCount(2);
        }

        public override object? SolveSecond()
        {
            return CalcTrailPositionCount(10);
        }

        private int CalcTrailPositionCount(int length)
        {
            (int x, int y)[] ropePositions = new (int x, int y)[length];
            HashSet<(int, int)> tPositions = new HashSet<(int, int)>();
            foreach(string line in Input.Lines)
            {
                (Dir dir, int steps) = ParseLine(line);
                (int xOff, int yOff) = dirVec[(int)dir];
                for(int i = 0;i < steps;i++)
                {
                    ropePositions[0].x += xOff;
                    ropePositions[0].y += yOff;

                    for(int k = 1;k < length;k++)
                    {
                        int xDiff = ropePositions[k - 1].x - ropePositions[k].x;
                        int yDiff = ropePositions[k - 1].y - ropePositions[k].y;
                        bool xStep = (Math.Abs(xDiff) > 1 && Math.Abs(yDiff) > 1) || (Math.Abs(xDiff) > 1 ^ Math.Abs(yDiff) > 1 && xDiff != 0);
                        bool yStep = (Math.Abs(xDiff) > 1 && Math.Abs(yDiff) > 1) || (Math.Abs(xDiff) > 1 ^ Math.Abs(yDiff) > 1 && yDiff != 0);
                        ropePositions[k].x += CopySign(xStep ? 1 : 0, xDiff);
                        ropePositions[k].y += CopySign(yStep ? 1 : 0, yDiff);
                    }

                    tPositions.Add(ropePositions[length - 1]);
                }
            }
            return tPositions.Count;
        }

        private static int CopySign(int magnitude, int sign)
        {
            return sign < 0 ? -magnitude : magnitude;
        }

        private static (Dir dir, int steps) ParseLine(string line)
        {
            string[] parts = line.Split(' ');
            return (Enum.Parse<Dir>(parts[0]), int.Parse(parts[1]));
        }

        private enum Dir
        {
            L, U, R, D
        }
    }
}
