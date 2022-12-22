using System;
using System.Collections.Generic;
using System.Linq;

namespace AdventOfCode2022.Day22
{
    public class Solution : SolutionBase
    {
        private static (int xfrom, int xTo, int yFrom, int yTo, Vec2 dir)[] zones = new (int xfrom, int xTo, int yFrom, int yTo, Vec2 dir)[]
        {
            (xfrom: 0, xTo: 49, yFrom: 200, yTo: 200, new Vec2(0, 1)),
            (xfrom: -1, xTo: -1, yFrom: 150, yTo: 199, new Vec2(-1, 0)),
            (xfrom: 50, xTo: 50, yFrom: 150, yTo: 199, new Vec2(1, 0)),

            (xfrom: -1, xTo: -1, yFrom: 100, yTo: 149, new Vec2(-1, 0)),
            (xfrom: 0, xTo: 49, yFrom: 99, yTo: 99, new Vec2(0, -1)),

            (xfrom: 50, xTo: 99, yFrom: 150, yTo: 150, new Vec2(0, 1)),
            (xfrom: 100, xTo: 100, yFrom: 100, yTo: 149, new Vec2(1, 0)),

            (xfrom: 49, xTo: 49, yFrom: 50, yTo: 99, new Vec2(-1, 0)),
            (xfrom: 100, xTo: 100, yFrom: 50, yTo: 99, new Vec2(1, 0)),

            (xfrom: 49, xTo: 49, yFrom: 0, yTo: 49, new Vec2(-1, 0)),
            (xfrom: 50, xTo: 99, yFrom: -1, yTo: -1, new Vec2(0, -1)),

            (xfrom: 100, xTo: 149, yFrom: -1, yTo: -1, new Vec2(0, -1)),
            (xfrom: 150, xTo: 150, yFrom: 0, yTo: 49, new Vec2(1, 0)),
            (xfrom: 100, xTo: 194, yFrom: 50, yTo: 50, new Vec2(0, 1)),
        };

        private static (int from, int to, Func<Vec2, Vec2> transform)[] transforms = new (int from, int to, Func<Vec2, Vec2> transform)[]
        {
            (1, 12, (Vec2 v) => new Vec2(v.x + 100, -200)),
            (2, 11, (Vec2 v) => new Vec2(v.y - 100, 0)),
            (3, 6, (Vec2 v) => v + new Vec2(v.y - 100, 149)),
            (4, 10, (Vec2 v) => v + new Vec2(50, 49 - (v.y - 100))),
            (5, 8, (Vec2 v) => v + new Vec2(50, v.x + 50)),
            (7, 13, (Vec2 v) => v + new Vec2(149, 49 - (100 - v.y))),
            (9, 14, (Vec2 v) => v + new Vec2(v.y - 50 + 100, 49)),
        };

        public Solution() : base(22, "Monkey Map")
        {

        }

        public override object? SolveFirst()
        {
            char[,] map = ParseMap(Input.Raw, out List<Step> path, out Vec2 startPos);
            Vec2 playerPos = startPos;
            Vec2 dir = new Vec2(1, 0);
            int stepIndex = 0;
            do
            {
                Step step = path[stepIndex];
                if(step.rotation != (char)0)
                {
                    dir = step.rotation == 'L' ? new Vec2(dir.y, -dir.x) : new Vec2(-dir.y, dir.x);
                }
                else
                {
                    for(int i = 0;i < step.forward;i++)
                    {
                        Vec2 np = new Vec2(playerPos.x + dir.x, playerPos.y + dir.y);
                        NormalizePos(ref np, map);
                        if(map[np.y, np.x] == ' ')
                        {
                            np = WrapAround(map, np, dir);
                        }
                        if(map[np.y, np.x] == '#')
                        {
                            break;
                        }
                        playerPos = np;
                        DrawMap(map, playerPos);
                    }
                }
                stepIndex++;
            } while(stepIndex < path.Count);

            int row = playerPos.y + 1;
            int column = playerPos.x + 1;
            int facing = dir == new Vec2(1, 0) ? 0 : (dir == new Vec2(0, 1) ? 1 : (dir == new Vec2(0, -1) ? 2 : 3));
            return 1000 * row + 4 * column + facing;
        }

        private void NormalizePos(ref Vec2 pos, char[,] map)
        {
            if(pos.x < 0)
            {
                pos.x = map.GetLength(1) - 1;
            }
            if(pos.y < 0)
            {
                pos.y = map.GetLength(0) - 1;
            }
            pos.x %= map.GetLength(1);
            pos.y %= map.GetLength(0);
        }

        private Vec2 WrapAround(char[,] map, Vec2 np, Vec2 dir)
        {
            while(map[np.y, np.x] == ' ')
            {
                np += dir;
                NormalizePos(ref np, map);
            }
            return np;
        }

        public override object? SolveSecond()
        {
            return null;
        }

        private void DrawMap(char[,] map, Vec2 player)
        {
            return;
            for(int i = 0;i < map.GetLength(0);i++)
            {
                for(int k = 0;k < map.GetLength(1);k++)
                {
                    Console.Write(i == player.y && k == player.x ? 'X' : map[i, k]);
                }
                Console.WriteLine();
            }
            Console.WriteLine();
            System.Threading.Thread.Sleep(1000);
        }

        private char[,] ParseMap(string input, out List<Step> path, out Vec2 startPos)
        {
            string[] parts = input.Split(new string[] { "\r\n\r\n", "\n\n" }, StringSplitOptions.RemoveEmptyEntries);
            string[] mapInput = parts[0].Split(new string[] { "\r\n", "\n" }, StringSplitOptions.RemoveEmptyEntries);
            string pathInput = parts[1];
            char[,] map = new char[mapInput.Length, mapInput.Max(n => n.Length)];
            startPos = new Vec2(mapInput[0].IndexOf('.'), 0);
            for(int i = 0;i < map.GetLength(0);i++)
            {
                for(int k = 0;k < map.GetLength(1);k++)
                {
                    map[i, k] = k < mapInput[i].Length ? mapInput[i][k] : ' ';
                }
            }

            path = new List<Step>();
            int index = 0;
            while(index < pathInput.Length)
            {
                if(char.IsDigit(pathInput[index]))
                {
                    int from = index;
                    while(++index < pathInput.Length && char.IsDigit(pathInput[index]))
                        ;
                    path.Add(new Step(int.Parse(pathInput.Substring(from, index - from))));
                    continue;
                }
                path.Add(new Step(0, pathInput[index++]));
            }

            return map;
        }

        private readonly record struct Step(int forward, char rotation = (char)0);
        private record struct Vec2(int x, int y)
        {
            public static Vec2 operator +(Vec2 lhs, Vec2 rhs) => new Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
        }

        private class CubeMap
        {
            private char[,] map;

            public CubeMap(char[,] map)
            {
                this.map = map;
            }

        }
    }
}
