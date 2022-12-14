using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AdventOfCode2022.Day14
{
    public class Solution : SolutionBase
    {
        private Vec2[] fallOffsets = new Vec2[3]
        {
            new Vec2(0, 1),
            new Vec2(-1, 1),
            new Vec2(1, 1)
        };

        public Solution() : base(14, "Regolith Reservoir")
        {
            
        }

        public override object? SolveFirst()
        {
            byte[,] cave = ParseCave(Input.Lines, out _);
            int amount = 0;
            while(SimulateSand(cave, new Vec2(500, 0)))
            {
                amount++;
            }
            return amount;
        }

        public override object? SolveSecond()
        {
            byte[,] cave = ParseCave(Input.Lines, out int maxY);
            RenderLine(cave, new Vec2(0, maxY + 2), new Vec2(cave.GetLength(0) - 1, maxY + 2));

            int amount = 0;
            while(SimulateSand(cave, new Vec2(500, 0)) && cave[500, 0] == 0)
            {
                amount++;
            }
            return amount + 1;
        }

        private bool SimulateSand(byte[,] cave, Vec2 origin)
        {
            Vec2 pos = origin;
            foreach(Vec2 offset in fallOffsets)
            {
                Vec2 p = new Vec2(pos.x + offset.x, pos.y + offset.y);
                if(p.x < 0 || p.y < 0 || p.x >= cave.GetLength(0) || p.y >= cave.GetLength(1))
                {
                    return false;
                }
                if(cave[p.x, p.y] == 0)
                {
                    pos = p;
                    return SimulateSand(cave, pos);
                }
            }
            cave[pos.x, pos.y] = 2;
            return true;
        }

        private byte[,] ParseCave(string[] lines, out int maxY)
        {
            maxY = 0;
            byte[,] cave = new byte[1000, 1000];
            for(int i = 0;i < lines.Length;i++)
            {
                Vec2[] points = lines[i].Split("->", StringSplitOptions.TrimEntries).Select(n => n.Split(',')).Select(n => new Vec2(int.Parse(n[0]), int.Parse(n[1]))).ToArray();
                for(int k = 1;k < points.Length;k++)
                {
                    RenderLine(cave, points[k - 1], points[k]);
                }
                maxY = Math.Max(maxY, points.Max(n => n.y));
            }
            return cave;
        }

        private void RenderLine(byte[,] data, Vec2 from, Vec2 to)
        {
            Vec2 dir = new Vec2(Clamp(to.x - from.x, -1, 1), Clamp(to.y - from.y, -1, 1));
            data[from.x, from.y] = 1;
            while(from != to)
            {
                from.x += dir.x;
                from.y += dir.y;
                data[from.x, from.y] = 1;
            }
        }

        private int Clamp(int value, int min, int max)
        {
            return value > max ? max : (value < min) ? min : value;
        }

        private record struct Vec2(int x, int y);
    }
}
