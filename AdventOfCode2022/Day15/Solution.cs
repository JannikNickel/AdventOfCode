using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace AdventOfCode2022.Day15
{
    public class Solution : SolutionBase
    {
        private Vec2[] diamondDirs = new Vec2[] { new Vec2(1, -1), new Vec2(1, 1), new Vec2(-1, 1), new Vec2(-1, -1) };

        public Solution() : base(15, "Beacon Exclusion Zone")
        {
            
        }

        public override object? SolveFirst()
        {
            return CountedBlockedInRow(ParseSensorData(Input.Lines), 2000000);
        }

        public override object? SolveSecond()
        {
            Vec2 pos = FindSignalSource(ParseSensorData(Input.Lines), 4000000);
            return (ulong)pos.x * 4000000UL + (ulong)pos.y;
        }

        private int CountedBlockedInRow((Vec2 sensor, Vec2 beacon)[] sensors, int row)
        {
            HashSet<int> blockedRowPositions = new HashSet<int>(sensors.Select(n => n.sensor).Where(n => n.y == row).Select(n => n.x).Concat(sensors.Select(n => n.beacon).Where(n => n.y == row).Select(n => n.x)));
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
                        if(blockedRowPositions.Contains(sensor.x + i))
                        {
                            continue;
                        }
                        blockedCells.Add(sensor.x + i);
                    }
                }
            }
            return blockedCells.Count();
        }

        private Vec2 FindSignalSource((Vec2 sensor, Vec2 beacon)[] sensors, int area)
        {
            foreach((Vec2 sensor, Vec2 beacon) in sensors)
            {
                foreach(Vec2 p in IterateSensor(sensor, ManhattenDst(sensor, beacon) + 1, area))
                {
                    if(sensors.Any(n => ManhattenDst(n.sensor, p) <= ManhattenDst(n.sensor, n.beacon)) == false)
                    {
                        return p;
                    }
                }
            }
            return default;
        }

        private IEnumerable<Vec2> IterateSensor(Vec2 sensor, int radius, int area)
        {
            Vec2 pos = sensor;
            pos.x -= radius;
            foreach(Vec2 dir in diamondDirs)
            {
                for(int i = 0;i < radius;i++)
                {
                    if(pos.x < 0 || pos.y < 0 || pos.x > area || pos.y > area)
                    {
                        continue;
                    }
                    yield return pos;
                    pos.x += dir.x;
                    pos.y += dir.y;
                }
            }
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
