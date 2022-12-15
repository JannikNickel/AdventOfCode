using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace AdventOfCode2022.Day15
{
    public class Solution : SolutionBase
    {
        public Solution() : base(15, "Beacon Exclusion Zone")
        {
            
        }

        public override object? SolveFirst()
        {
            (Vec2 sensor, Vec2 beacon)[] sensors = ParseSensorData(Input.Lines);
            return CountFreeInRow(sensors, 2000000);
        }

        public override object? SolveSecond()
        {
            return null;
        }

        private int CountFreeInRow((Vec2 sensor, Vec2 beacon)[] sensors, int row)
        {
            HashSet<int> blockRowPositions = new HashSet<int>(
                sensors.Select(n => n.sensor).Where(n => n.y == row).Select(n => n.x)
                .Concat(sensors.Select(n => n.beacon).Where(n => n.y == row).Select(n => n.x)));
            Console.WriteLine(blockRowPositions.Count);
            HashSet<int> blockedCells = new HashSet<int>();

            foreach((Vec2 sensor, Vec2 beacon) in sensors)
            {
                int r = ManhattenDst(sensor, beacon);
                int rowDst = Math.Abs(row - sensor.y);
                if(rowDst <= r)
                {
                    int w = r - rowDst;
                    for(int i = -w;i < w + 1;i++)
                    {
                        if(blockRowPositions.Contains(sensor.x + i))
                        {
                            continue;
                        }
                        blockedCells.Add(sensor.x + i);
                    }
                }
            }
            return blockedCells.Count();
        }

        private int ManhattenDst(Vec2 from, Vec2 to)
        {
            return Math.Abs(to.x - from.x) + Math.Abs(to.y - from.y);
        }

        private (Vec2 sensor, Vec2 beacon)[] ParseSensorData(string[] lines)
        {
            return lines
                .Select(n => Regex.Matches(n, @"-?\d+"))
                .Select(n => (new Vec2(int.Parse(n[0].Value), int.Parse(n[1].Value)), new Vec2(int.Parse(n[2].Value), int.Parse(n[3].Value)))).ToArray();
        }

        private record struct Vec2(int x, int y);
    }
}
