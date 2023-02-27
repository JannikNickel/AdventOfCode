using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;

namespace AdventOfCode2022.Day11
{
    public class Solution : SolutionBase
    {
        public Solution() : base(11, "Monkey in the Middle")
        {
            
        }

        public override object? SolveFirst()
        {
            Monkey[] monkeys = ParseMonkeys(Input.Raw);
            for(ulong i = 0;i < 20;i++)
            {
                foreach(Monkey monkey in monkeys)
                {
                    monkey.Turn(monkeys);
                }
            }
            return monkeys.OrderByDescending(n => n.Inspections).Take(2).Select(n => n.Inspections).Aggregate<ulong>((a, b) => a * b);
        }

        public override object? SolveSecond()
        {
            Monkey[] monkeys = ParseMonkeys(Input.Raw);
            ulong commonMultiple = monkeys.Select(n => n.TestDivisor).Aggregate((a, b) => a * b);
            for(int i = 0;i < 10000;i++)
            {
                foreach(Monkey monkey in monkeys)
                {
                    monkey.Turn(monkeys, commonMultiple);
                }
            }
            return monkeys.OrderByDescending(n => n.Inspections).Take(2).Select(n => n.Inspections).Aggregate<ulong>((a, b) => a * b);
        }

        private Monkey[] ParseMonkeys(string input)
        {
            Monkey[] monkeys = input.Split(new string[] { "\r\n\r\n", "\n\n" }, StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries).Select(n =>
            {
                string[] lines = n.Split(new string[] { "\r\n", "\n" }, StringSplitOptions.RemoveEmptyEntries);
                IEnumerable<ulong> startingItems = Regex.Matches(lines[1], @"\d+").Select(n => ulong.Parse(n.Value));
                string[] opComps = lines[2].Replace("Operation: new = ", "").Split(' ', StringSplitOptions.RemoveEmptyEntries);
                bool lhsIsVar = opComps[0] == "old";
                bool rhsIsVar = opComps[2] == "old";
                Func<ulong, ulong> operation = opComps[1] == "*" ?
                    (n => (lhsIsVar ? n : ulong.Parse(opComps[0])) * (rhsIsVar ? n : ulong.Parse(opComps[2])))
                    : (n => (lhsIsVar ? n : ulong.Parse(opComps[0])) + (rhsIsVar ? n : ulong.Parse(opComps[2])));
                ulong testDivisor = ulong.Parse(Regex.Match(lines[3], @"\d+").Value);
                int testResultTrue = int.Parse(Regex.Match(lines[4], @"\d+").Value);
                int testResultFalse = int.Parse(Regex.Match(lines[5], @"\d+").Value);
                return new Monkey(operation, testDivisor, testResultTrue, testResultFalse, startingItems);
            }).ToArray();
            return monkeys;
        }

        private class Monkey
        {
            private Func<ulong, ulong> operation;
            private ulong testDivisor;
            private int testResultTrue;
            private int testResultFalse;
            private Queue<ulong> items;

            public ulong TestDivisor => testDivisor;
            public ulong Inspections { get; private set; } = 0;

            public Monkey(Func<ulong, ulong> operation, ulong testDivisor, int testResultTrue, int testResultFalse, IEnumerable<ulong> items)
            {
                this.operation = operation;
                this.testDivisor = testDivisor;
                this.testResultTrue = testResultTrue;
                this.testResultFalse = testResultFalse;
                this.items = new Queue<ulong>(items ?? Array.Empty<ulong>());
            }

            public void Turn(Monkey[] monkeys, ulong lm = 0)
            {
                while(items.Count > 0)
                {
                    ulong item = items.Dequeue();
                    item = operation(item);
                    item = lm == 0 ? ((ulong)Math.Floor(item / 3d)) : (item % lm);
                    monkeys[item % testDivisor == 0 ? testResultTrue : testResultFalse].items.Enqueue(item);
                    Inspections++;
                }
            }
        }
    }
}
