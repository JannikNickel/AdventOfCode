using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AdventOfCode2022.Day23
{
    public class Solution : SolutionBase
    {
        private static Vec2[][] directions = new Vec2[][]
        {
            new Vec2[] { new Vec2(0, 1), new Vec2(1, 1), new Vec2(-1, 1) },
            new Vec2[] { new Vec2(0, -1), new Vec2(1, -1), new Vec2(-1, -1) },
            new Vec2[] { new Vec2(-1, 0), new Vec2(-1, 1), new Vec2(-1, -1) },
            new Vec2[] { new Vec2(1, 0), new Vec2(1, 1), new Vec2(1, -1) }
        };
        private static Vec2[] allDirs = new Vec2[]
        {
            new Vec2(0, 1),
            new Vec2(1, 1),
            new Vec2(1, 0),
            new Vec2(1, -1),
            new Vec2(0, -1),
            new Vec2(-1, -1),
            new Vec2(-1, 0),
            new Vec2(-1, 1)
        };

        public Solution() : base(23, "Unstable Diffusion")
        {
            
        }

        public override object? SolveFirst()
        {
            Elf[] elves = ParseElves(Input.Lines);
            return Simulate(elves, 10, out _);
        }

        public override object? SolveSecond()
        {
            Elf[] elves = ParseElves(Input.Lines);
            Simulate(elves, int.MaxValue, out int round);
            return round;
        }

        private int Simulate(Elf[] elves, int roundLimit, out int rounds)
        {
            Dictionary<Vec2, Elf> positions = new Dictionary<Vec2, Elf>(elves.Select(n => new KeyValuePair<Vec2, Elf>(n.Position, n)));
            int dIndex = 0;
            rounds = 0;
            for(int i = 0;i < roundLimit;i++)
            {
                elves.ForEach(n => n.Propose(positions, dIndex));
                elves.ForEach(n => n.CheckMove(positions));
                elves.ForEach(n => n.Move(positions));
                rounds++;
                if(elves.All(n => n.CanMove == false))
                {
                    break;
                }
                dIndex = (dIndex + 1) % directions.Length;
            }
            return FreeArea(positions);
        }

        private int FreeArea(Dictionary<Vec2, Elf> positions)
        {
            int xMin = positions.Keys.Min(n => n.x);
            int xMax = positions.Keys.Max(n => n.x);
            int yMin = positions.Keys.Min(n => n.y);
            int yMax = positions.Keys.Max(n => n.y);

            int c = 0;
            for(int y = yMax;y >= yMin;y--)
            {
                for(int x = xMin;x <= xMax;x++)
                {
                    if(positions.ContainsKey(new Vec2(x, y)) == false)
                    {
                        c++;
                    }
                }
            }
            return c;
        }

        private Elf[] ParseElves(string[] lines)
        {
            List<Elf> elves = new List<Elf>();
            for(int i = 0;i < lines.Length;i++)
            {
                for(int k = 0;k < lines[i].Length;k++)
                {
                    if(lines[i][k] == '#')
                    {
                        Vec2 pos = new Vec2(k, lines.Length - 1 - i);
                        elves.Add(new Elf(pos));
                    }
                }
            }
            return elves.ToArray();
        }

        private readonly record struct Vec2(int x, int y)
        {
            public static Vec2 operator +(Vec2 lhs, Vec2 rhs) => new Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
        }

        private class Elf
        {
            public Vec2 Position { get; private set; }
            public bool CanMove { get; private set; }

            private Vec2 proposed;

            public Elf(Vec2 pos)
            {
                Position = pos;
            }

            public void Propose(Dictionary<Vec2, Elf> positions, int dIndex)
            {
                proposed = Position;
                if(allDirs.All(n => positions.ContainsKey(Position + n) == false))
                {
                    return;
                }

                for(int i = 0;i < directions.Length;i++)
                {
                    Vec2[] d = directions[(dIndex + i) % directions.Length];
                    if(d.All(n => positions.ContainsKey(Position + n) == false))
                    {
                        proposed = Position + d[0];
                        break;
                    }
                }
            }

            public void CheckMove(Dictionary<Vec2, Elf> positions)
            {
                CanMove = Position != proposed && !positions.Any(n => n.Value.proposed == proposed && n.Value != this);
            }

            public void Move(Dictionary<Vec2, Elf> elves)
            {
                if(CanMove)
                {
                    elves.Remove(Position);
                    Position = proposed;
                    elves[Position] = this;
                }
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
