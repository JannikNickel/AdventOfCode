using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AdventOfCode2022.Day21
{
    public class Solution : SolutionBase
    {
        public Solution() : base(21, "Monkey Math")
        {

        }

        public override object? SolveFirst()
        {
            Monkey[] monkeys = ParseMonkeys(Input.Lines);
            foreach(Monkey monkey in monkeys)
            {
                monkey.Solve();
            }
            return monkeys.First(n => n.Name == "root").Value;
        }

        public override object? SolveSecond()
        {
            Monkey[] monkeys = ParseMonkeys(Input.Lines);
            long input = 0;
            long lastDiff = Solve(monkeys, 0).l - Solve(monkeys, 0).r;
            long inc = int.MaxValue;
            long diffThreshold = 10000;
            while(lastDiff != 0)
            {
                input += inc;
                (long l, long r) s = Solve(monkeys, input);
                long diff = s.l - s.r;
                if(Math.Abs(diff) > Math.Abs(lastDiff))
                {
                    inc = (long)Math.Floor(inc / 2d) * -1;
                }

                //This is required, because the calculations to "inc" are casted to long which results in results that could be slighty off
                if(Math.Abs(diff) < diffThreshold)
                {
                    for(long i = -diffThreshold;i < diffThreshold;i++)
                    {
                        (long l, long r) = Solve(monkeys, input + i);
                        if(l == r)
                        {
                            return input + i;
                        }
                    }
                }
                lastDiff = diff;
            }
            return input;
        }

        private (long l, long r) Solve(Monkey[] monkeys, long input)
        {
            foreach(Monkey monkey in monkeys)
            {
                monkey.ResetResult();
            }
            monkeys.First(n => n.Name == "humn").Value = input;
            foreach(Monkey monkey in monkeys)
            {
                monkey.Solve();
            }
            Monkey root = monkeys.First(n => n.Name == "root");
            return (root.LeftDependency?.Value ?? 0, root.RightDependency?.Value ?? 0);
        }

        private Monkey[] ParseMonkeys(string[] lines)
        {
            Monkey[] monkeys = lines.Select(n => new Monkey(n.Split(':')[0])).ToArray();
            for(int i = 0;i < monkeys.Length;i++)
            {
                string[] parts = lines[i].Split(':')[1].Split(' ', StringSplitOptions.TrimEntries | StringSplitOptions.RemoveEmptyEntries);
                if(parts.Length > 1)
                {
                    monkeys[i].LeftDependency = monkeys.First(n => n.Name == parts[0]);
                    monkeys[i].RightDependency = monkeys.First(n => n.Name == parts[2]);
                    monkeys[i].Op = parts[1] switch
                    {
                        "+" => (lhs, rhs) => lhs.Value + rhs.Value,
                        "-" => (lhs, rhs) => lhs.Value - rhs.Value,
                        "*" => (lhs, rhs) => lhs.Value * rhs.Value,
                        "/" => (lhs, rhs) => lhs.Value / rhs.Value,
                        _ => (_, _) => 0
                    };
                }
                else
                {
                    monkeys[i].Value = int.Parse(parts[0]);
                }
            }
            return monkeys;
        }

        private class Monkey
        {
            public string Name { get; init; }
            public Monkey? LeftDependency { get; set; }
            public Monkey? RightDependency { get; set; }
            public Func<Monkey, Monkey, long>? Op { get; set; }
            public long Value { get; set; }

            public Monkey(string name)
            {
                Name = name;
            }

            public void Solve()
            {
                if(Op == null || LeftDependency == null || RightDependency == null)
                {
                    return;
                }
                if(LeftDependency.Value == 0)
                {
                    LeftDependency.Solve();
                }
                if(RightDependency.Value == 0)
                {
                    RightDependency.Solve();
                }
                Value = Op(LeftDependency, RightDependency);
            }

            public void ResetResult()
            {
                if(Op != null)
                {
                    Value = 0;
                }
            }
        }
    }
}
