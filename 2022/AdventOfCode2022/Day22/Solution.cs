using System;
using System.Collections.Generic;
using System.Linq;

namespace AdventOfCode2022.Day22
{
    public class Solution : SolutionBase
    {
        public Solution() : base(22, "Monkey Map")
        {
            
        }

        public override object? SolveFirst()
        {
            Map map = new Map(ParseMap(Input.Raw, out List<Step> path, out Vec2 pos));
            Vec2 dir = new Vec2(1, 0);
            for(int i = 0;i < path.Count;i++)
            {
                map.HandleStep(path[i], ref pos, ref dir);
            }
            return CalcScore(pos, dir);
        }

        public override object? SolveSecond()
        {
            CubeMap map = new CubeMap(ParseMap(Input.Raw, out List<Step> path, out Vec2 pos));
            Vec2 dir = new Vec2(1, 0);
            for(int i = 0;i < path.Count;i++)
            {
                map.HandleStep(path[i], ref pos, ref dir);
            }
            return CalcScore(pos, dir);
        }

        private int CalcScore(Vec2 player, Vec2 dir)
        {
            int facing = dir == new Vec2(1, 0) ? 0 : (dir == new Vec2(0, -1) ? 1 : (dir == new Vec2(-1, 0) ? 2 : 3));
            return 1000 * (player.y + 1) + 4 * (player.x + 1) + facing;
        }

        private char[,] ParseMap(string input, out List<Step> path, out Vec2 startPos)
        {
            string[] parts = input.Split(new string[] { "\r\n\r\n", "\n\n" }, StringSplitOptions.RemoveEmptyEntries);
            string[] mapInput = parts[0].Split(new string[] { "\r\n", "\n" }, StringSplitOptions.RemoveEmptyEntries);
            char[,] map = new char[mapInput.Length, mapInput.Max(n => n.Length)];
            startPos = new Vec2(mapInput[0].IndexOf('.'), 0);
            for(int i = 0;i < map.GetLength(0);i++)
            {
                for(int k = 0;k < map.GetLength(1);k++)
                {
                    map[i, k] = k < mapInput[i].Length ? mapInput[i][k] : ' ';
                }
            }

            string pathInput = parts[1];
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
            public static Vec2 Right => new Vec2(1, 0);
            public static Vec2 Down => new Vec2(0, -1);
            public static Vec2 Left => new Vec2(-1, 0);
            public static Vec2 Up => new Vec2(0, 1);

            public static Vec2 operator +(Vec2 lhs, Vec2 rhs) => new Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
            public static Vec2 operator -(Vec2 value) => new Vec2(-value.x, -value.y);
        }

        private class Map
        {
            protected char[,] map;

            public Map(char[,] map)
            {
                this.map = map;
            }

            public void HandleStep(Step step, ref Vec2 pos, ref Vec2 dir)
            {
                if(step.rotation != (char)0)
                {
                    dir = step.rotation == 'L' ? new Vec2(dir.y, -dir.x) : new Vec2(-dir.y, dir.x);
                    return;
                }

                for(int i = 0;i < step.forward;i++)
                {
                    Move(ref pos, ref dir);
                }
            }

            protected virtual bool Move(ref Vec2 pos, ref Vec2 dir)
            {
                Vec2 np = pos + dir;
                NormalizePos(ref np);
                if(map[np.y, np.x] == ' ')
                {
                    np = WrapAround(map, np, dir);
                }
                if(map[np.y, np.x] == '#')
                {
                    return false;
                }
                pos = np;
                return true;
            }

            private void NormalizePos(ref Vec2 pos)
            {
                pos.x = (pos.x + map.GetLength(1)) % map.GetLength(1);
                pos.y = (pos.y + map.GetLength(0)) % map.GetLength(0);
            }

            private Vec2 WrapAround(char[,] map, Vec2 np, Vec2 dir)
            {
                while(map[np.y, np.x] == ' ')
                {
                    np += dir;
                    NormalizePos(ref np);
                }
                return np;
            }
        }

        private class CubeMap : Map
        {
            private Dictionary<(Vec2 from, Vec2 dir), (Vec2 target, Vec2 dir)> jumps;

            public CubeMap(char[,] map) : base(map)
            {
                this.map = map;
                this.jumps = new Dictionary<(Vec2 from, Vec2 dir), (Vec2 target, Vec2 dir)>();

                Enumerable.Range(0, 50).ForEach(n => AddJump(new Vec2(50, 150 + n), new Vec2(50 + n, 150), Vec2.Left, Vec2.Down));
                Enumerable.Range(0, 50).ForEach(n => AddJump(new Vec2(-1, 150 + n), new Vec2(50 + n, -1), Vec2.Right, Vec2.Up));
                Enumerable.Range(0, 50).ForEach(n => AddJump(new Vec2(n, 200), new Vec2(n + 100, -1), Vec2.Down, Vec2.Up));
                Enumerable.Range(0, 50).ForEach(n => AddJump(new Vec2(n, 99), new Vec2(49, 50 + n), Vec2.Up, Vec2.Right));
                Enumerable.Range(0, 50).ForEach(n => AddJump(new Vec2(-1, 100 + n), new Vec2(49, 49 - n), Vec2.Right, Vec2.Right));
                Enumerable.Range(0, 50).ForEach(n => AddJump(new Vec2(100, 100 + n), new Vec2(150, 49 - n), Vec2.Left, Vec2.Left));
                Enumerable.Range(0, 50).ForEach(n => AddJump(new Vec2(100, 50 + n), new Vec2(100 + n, 50), Vec2.Left, Vec2.Down));
            }

            private void AddJump(Vec2 a, Vec2 b, Vec2 dirA, Vec2 dirB)
            {
                jumps.Add((a, -dirA), (b, dirB));
                jumps.Add((b, -dirB), (a, dirA));
            }

            protected override bool Move(ref Vec2 pos, ref Vec2 dir)
            {
                Vec2 np = pos + dir;
                Vec2 nd = dir;
                if(jumps.TryGetValue((np, nd), out (Vec2 target, Vec2 targetDir) result))
                {
                    np = result.target;
                    nd = result.targetDir;
                    np += nd;
                }
                if(map[np.y, np.x] == '#')
                {
                    return false;
                }
                pos = np;
                dir = nd;
                return true;
            }
        }
    }

    internal static class EnumerableExtensions
    {
        internal static void ForEach<T>(this IEnumerable<T> source, Action<T> action)
        {
            foreach(T item in source)
            {
                action?.Invoke(item);
            }
        }
    }
}
