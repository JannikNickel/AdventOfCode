using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AdventOfCode2022.Day24
{
    public class Solution : SolutionBase
    {
        private static Vec2[] nDirs = new Vec2[] { new Vec2(-1, 0), new Vec2(1, 0), new Vec2(0, -1), new Vec2(0, 1), new Vec2(0, 0) };

        private static (Cell, Vec2)[] dirVecs = new (Cell, Vec2)[]
        {
            (Cell.Left, nDirs[0]),
            (Cell.Right, nDirs[1]),
            (Cell.Up, nDirs[2]),
            (Cell.Down, nDirs[3])
        };

        public Solution() : base(24, "Blizzard Basin")
        {
            
        }

        public override object? SolveFirst()
        {
            Map map = ParseMap(Input.Lines, out Vec2 start, out Vec2 end);
            return FindPath(map, start, end, 0);
        }

        public override object? SolveSecond()
        {
            Map map = ParseMap(Input.Lines, out Vec2 start, out Vec2 end);
            int t0 = FindPath(map, start, end, 0);
            int t1 = FindPath(map, end, start, t0);
            return FindPath(map, start, end, t1);
        }

        private int FindPath(Map map, Vec2 from, Vec2 to, int startTime)
        {
            Queue<State> states = new Queue<State>();
            HashSet<State> seen = new HashSet<State>();
            states.Enqueue(new State(from, startTime));
            int best = int.MaxValue;
            while(states.Count > 0)
            {
                State state = states.Dequeue();

                if(state.minute > best)
                {
                    continue;
                }
                if(state.pos == to)
                {
                    best = Math.Min(best, state.minute);
                    continue;
                }

                State stateHash = new State(state.pos, state.minute % map.Duration);
                if(seen.Contains(stateHash))
                {
                    continue;
                }
                seen.Add(stateHash);

                int m = state.minute + 1;
                foreach(Vec2 dir in nDirs)
                {
                    if(map[m, state.pos + dir] == Cell.Empty)
                    {
                        states.Enqueue(new State(state.pos + dir, m));
                    }
                }
            }
            return best;
        }

        private Map ParseMap(string[] lines, out Vec2 start, out Vec2 end)
        {
            Cell[,] map = new Cell[lines.Length + 2, lines[0].Length + 2];
            for(int i = 0;i < map.GetLength(0);i++)
            {
                for(int k = 0;k < map.GetLength(1);k++)
                {
                    if(i == 0 || k == 0 || i == map.GetLength(0) - 1 || k == map.GetLength(1) - 1)
                    {
                        map[i, k] = Cell.Wall;
                    }
                    else
                    {
                        map[i, k] = lines[i - 1][k - 1] switch
                        {
                            '#' => Cell.Wall,
                            '>' => Cell.Right,
                            '<' => Cell.Left,
                            '^' => Cell.Up,
                            'v' => Cell.Down,
                            _ => Cell.Empty
                        };
                    }
                }
            }
            start = new Vec2(2, 1);
            end = new Vec2(map.GetLength(1) - 3, map.GetLength(0) - 2);
            return new Map(map);
        }

        private readonly record struct State(Vec2 pos, int minute);

        private record struct Vec2(int x, int y)
        {
            public static Vec2 operator +(Vec2 lhs, Vec2 rhs) => new Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
        }

        private class Map
        {
            private Cell[][,] maps;

            public int Width { get; init; }
            public int Height { get; init; }
            public int Duration { get; init; }

            public Cell this[int t, Vec2 pos]
            {
                get => maps[t % maps.Length][pos.y, pos.x];
            }

            public Map(Cell[,] map)
            {
                Width = map.GetLength(1);
                Height = map.GetLength(0);
                Duration = Lcm(Width - 4, Height - 4);
                maps = new Cell[Duration][,];
                for(int i = 0;i < maps.Length;i++)
                {
                    maps[i] = map;
                    map = Simulate(map);
                }
            }

            private Cell[,] Simulate(Cell[,] source)
            {
                Cell[,] map = (Cell[,])source.Clone();
                for(int i = 0;i < map.GetLength(0);i++)
                {
                    for(int k = 0;k < map.GetLength(1);k++)
                    {
                        if((byte)map[i, k] > (byte)Cell.Wall)
                        {
                            map[i, k] = Cell.Empty;
                        }
                    }
                }
                for(int y = 0;y < map.GetLength(0);y++)
                {
                    for(int x = 0;x < map.GetLength(1);x++)
                    {
                        if((byte)source[y, x] > (byte)Cell.Wall)
                        {
                            Cell sourceCell = source[y, x];
                            foreach((Cell cell, Vec2 dir) item in dirVecs)
                            {
                                if(sourceCell.HasFlag(item.cell))
                                {
                                    Vec2 dir = item.dir;
                                    Vec2 pos = new Vec2(x, y) + dir;
                                    while(map[pos.y, pos.x] == Cell.Wall)
                                    {
                                        pos += dir;
                                        pos.x = (pos.x + map.GetLength(1)) % map.GetLength(1);
                                        pos.y = (pos.y + map.GetLength(0)) % map.GetLength(0);
                                    }
                                    if((byte)map[pos.y, pos.x] <= (byte)Cell.Wall)
                                    {
                                        map[pos.y, pos.x] = 0;
                                    }
                                    map[pos.y, pos.x] |= item.cell;
                                }
                            }
                        }
                    }
                }
                return map;
            }

            private int Lcm(int a, int b)
            {
                return (a * b) / Gcd(a, b);
            }

            private int Gcd(int a, int b)
            {
                return b == 0 ? a : Gcd(b, a % b);
            }
        }

        [System.Flags]
        private enum Cell : byte
        {
            Empty = 0,
            Wall = 1,
            Left = 2,
            Up = 4,
            Right = 8,
            Down = 16
        }
    }
}
