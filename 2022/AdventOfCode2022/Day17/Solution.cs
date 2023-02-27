using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AdventOfCode2022.Day17
{
    public class Solution : SolutionBase
    {
        private Shape[] shapes = new Shape[]
        {
            new Shape("####"),
            new Shape(".#.\n###\n.#."),
            new Shape("..#\n..#\n###"),
            new Shape("#\n#\n#\n#"),
            new Shape("##\n##")
        };

        public Solution() : base(17, "Pyroclastic Flow")
        {
            
        }

        public override object? SolveFirst()
        {
            int[] dirs = ParseDirections(base.Input.Raw);
            ExpandingGrid grid = new ExpandingGrid(7, dirs, false);
            return grid.SimulateShapes(shapes, 2022);
        }

        public override object? SolveSecond()
        {
            int[] dirs = ParseDirections(base.Input.Raw);
            ExpandingGrid grid = new ExpandingGrid(7, dirs, false);
            return grid.SimulateShapes(shapes, 1000000000000);
        }

        private int[] ParseDirections(string input)
        {
            return input.Select(n => n == '>' ? 1 : -1).ToArray();
        }

        private record struct Vec2(int x, int y);

        private class Shape
        {
            private byte[,] grid;

            public int Width { get; init; }
            public int Height { get; init; }
            public byte this[int x, int y] => grid[y, x];

            public Shape(string shape)
            {
                string[] lines = shape.Split("\n", StringSplitOptions.RemoveEmptyEntries);
                Width = lines[0].Length;
                Height = lines.Length;
                grid = new byte[Height, Width];
                for(int y = 0;y < Height;y++)
                {
                    for(int x = 0;x < Width;x++)
                    {
                        grid[y, x] = (byte)(lines[Height - 1 - y][x] == '#' ? 1 : 0);
                    }
                }
            }
        }

        private class ExpandingGrid
        {
            private readonly int width;
            private readonly int[] dirs;
            private readonly bool debug;

            private int dirStep = 0;
            private List<BitMask> rows = new List<BitMask>();

            public int Highest
            {
                get
                {
                    for(int i = rows.Count - 1;i >= 0;i--)
                    {
                        if(rows[i].IsEmpty == false)
                        {
                            return i + 1;
                        }
                    }
                    return 0;
                }
            }

            public ExpandingGrid(int width, int[] dirs, bool debug = false)
            {
                this.width = width;
                this.dirs = dirs;
                this.debug = debug;
            }

            private string GenSearchKey(int topAmount, int shapeIndex, int dirStep)
            {
                string key = "";
                int k = 0;
                int kIndex = rows.Count - 1;
                while(k < topAmount)
                {
                    if(rows[kIndex].IsEmpty == false)
                    {
                        key += $"{(byte)(int)rows[kIndex + 1]}_";
                        k++;
                    }
                    kIndex--;
                }
                return key + $"[{shapeIndex}]_[{dirStep % dirs.Length}]";
            }

            public long SimulateShapes(Shape[] shapes, long amount)
            {
                const int topAmount = 50;
                Dictionary<string, (long iteration, long highest)> seen = new Dictionary<string, (long iteration, long highest)>();
                int shapeIndex = 0;
                long simulatedHeight = 0;
                for(long i = 0;i < amount;i++)
                {
                    Shape shape = shapes[shapeIndex];
                    SimulateShape(shape);
                    shapeIndex = (shapeIndex + 1) % shapes.Length;

                    if(amount > 2022)
                    {
                        if(simulatedHeight == 0 && i >= topAmount)
                        {
                            string key = GenSearchKey(topAmount, shapeIndex, dirStep);
                            if(seen.TryGetValue(key, out (long iteration, long highest) cycle))
                            {
                                long cycleShapeAmount = i - cycle.iteration;
                                long cycleHeight = Highest - cycle.highest;
                                long sim = (long)Math.Floor((amount - i) / (double)cycleShapeAmount) - 1;
                                simulatedHeight += sim * cycleHeight;
                                i += sim * cycleShapeAmount;
                            }
                            else
                            {
                                seen[key] = (i, Highest);
                            }
                        }
                    }
                }
                return Highest + simulatedHeight;
            }

            public void SimulateShape(Shape shape)
            {
                Vec2 pos = new Vec2(2, Highest + 3);
                EnsureRows(pos.y + shape.Height);
                Print(shape, pos);

                while(true)
                {
                    int dir = dirs[dirStep++ % dirs.Length];
                    if(SidewaysBlocked(shape, pos, dir) == false)
                    {
                        pos.x += dir;
                    }
                    Print(shape, pos);
                    if(BottomBlocked(shape, pos))
                    {
                        StoreShape(shape, pos);
                        break;
                    }
                    pos.y--;
                    Print(shape, pos);
                }
            }

            private bool SidewaysBlocked(Shape shape, Vec2 pos, int dir)
            {
                pos.x += dir;
                if(pos.x < 0 || pos.x + shape.Width > width)
                {
                    return true;
                }
                return ShapeOverlapping(shape, pos);
            }

            private bool BottomBlocked(Shape shape, Vec2 pos)
            {
                pos.y--;
                if(pos.y < 0)
                {
                    return true;
                }
                return ShapeOverlapping(shape, pos);
            }

            private bool ShapeOverlapping(Shape shape, Vec2 pos)
            {
                for(int i = 0;i < shape.Width;i++)
                {
                    for(int k = 0;k < shape.Height;k++)
                    {
                        Vec2 p = new Vec2(pos.x + i, pos.y + k);
                        if(shape[i, k] > 0 && rows[p.y][p.x])
                        {
                            return true;
                        }
                    }
                }
                return false;
            }

            private void StoreShape(Shape shape, Vec2 pos)
            {
                for(int i = 0;i < shape.Width;i++)
                {
                    for(int k = 0;k < shape.Height;k++)
                    {
                        Vec2 p = new Vec2(pos.x + i, pos.y + k);
                        if(shape[i, k] > 0)
                        {
                            BitMask mask = rows[p.y];
                            mask[p.x] = true;
                            rows[p.y] = mask;
                        }
                    }
                }
            }

            private void EnsureRows(int h)
            {
                int amount = h - rows.Count;
                for(int i = 0;i < amount;i++)
                {
                    rows.Add(new BitMask());
                }
            }

            private void Print(Shape shape, Vec2 pos)
            {
                if(debug == false)
                {
                    return;
                }
                for(int y = rows.Count - 1;y >= 0;y--)
                {
                    for(int x = 0;x < width;x++)
                    {
                        char c = rows[y][x] ? '#' : '.';
                        if(x >= pos.x && y >= pos.y && x < pos.x + shape.Width && y < pos.y + shape.Height)
                        {
                            c = shape[x - pos.x, y - pos.y] == 1 ? '@' : '.';
                        }
                        Console.Write(c);
                    }
                    Console.WriteLine();
                }
                Console.WriteLine();
                System.Threading.Thread.Sleep(250);
            }
        }

        public struct BitMask
        {
            public int data;

            public bool IsEmpty => data == 0;

            public bool this[int index]
            {
                get => (data & (1 << index)) != 0;
                set
                {
                    if(value)
                    {
                        data |= (1 << index);
                    }
                    else
                    {
                        data &= ~(1 << index);
                    }
                }
            }

            public static implicit operator int(BitMask mask)
            {
                return mask.data;
            }
        }
    }
}
