using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AdventOfCode2022.Day13
{
    public class Solution : SolutionBase
    {
        public Solution() : base(13, "Distress Signal")
        {
            
        }

        public override object? SolveFirst()
        {
            List<(NestedList left, NestedList right)> pairs = ParsePairs(Input.Raw);
            return pairs.Select((n, i) => (i + 1, n.left.CompareTo(n.right))).Where(n => n.Item2 == -1).Sum(n => n.Item1);
        }

        public override object? SolveSecond()
        {
            NestedList decKey1 = NestedList.Parse("[[2]]");
            NestedList decKey2 = NestedList.Parse("[[6]]");
            IEnumerable<NestedList> list = ParsePairs(Input.Raw).SelectMany(n => new NestedList[] { n.left, n.right }).Concat(new NestedList[] { decKey1, decKey2 });
            list = list.OrderBy(n => n);
            return (list.TakeWhile(n => n != decKey1).Count() + 1) * (list.TakeWhile(n => n != decKey2).Count() + 1);
        }

        private List<(NestedList left, NestedList right)> ParsePairs(string input)
        {
            List<(NestedList left, NestedList right)> pairs = new List<(NestedList left, NestedList right)>();
            foreach(string s in input.Split(new string[] { "\r\n\r\n", "\n\n" }, StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries))
            {
                string[] lines = s.Split(new string[] { "\r\n", "\n" }, StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);
                pairs.Add((NestedList.Parse(lines[0]), NestedList.Parse(lines[1])));
            }
            return pairs;
        }

        private class NestedList : IComparable<NestedList>
        {
            private List<NestedList> elements = new List<NestedList>();

            public int Value { get; init; } = -1;
            public bool IsValue => Value != -1;
            public int Count => elements.Count;

            public NestedList this[int index]
            {
                get => elements[index];
            }

            public NestedList(int value)
            {
                this.Value = value;
            }

            public NestedList()
            {
                
            }

            public NestedList AsList()
            {
                if(IsValue)
                {
                    NestedList list = new NestedList();
                    list.elements.Add(new NestedList(Value));
                    return list;
                }
                return this;
            }

            public static NestedList Parse(string line)
            {
                NestedList list = new NestedList();
                for(int i = 1;i < line.Length - 1;i++)
                {
                    if(char.IsDigit(line[i]))
                    {
                        int from = i;
                        do
                        {
                            i++;
                        }
                        while(line[i] != ',' && i < line.Length - 1);
                        int value = int.Parse(line.Substring(from, i - from));
                        list.elements.Add(new NestedList(value));
                    }
                    else if(line[i] == '[')
                    {
                        int from = i;
                        int open = 1;
                        do
                        {
                            i++;
                            open += (line[i] == '[' ? 1 : (line[i] == ']' ? -1 : 0));
                        }
                        while(open != 0);
                        i++;
                        list.elements.Add(NestedList.Parse(line.Substring(from, i - from)));
                    }
                }
                return list;
            }

            public int CompareTo(NestedList? other)
            {
                if(other == null)
                {
                    throw new ArgumentNullException(nameof(other));
                }

                NestedList l = this;
                NestedList r = other;
                if(l.IsValue ^ r.IsValue)
                {
                    l = l.AsList();
                    r = r.AsList();
                }
                if(l.IsValue && r.IsValue)
                {
                    return l.Value < r.Value ? -1 : (l.Value > r.Value ? 1 : 0);
                }
                for(int i = 0;i < l.Count;i++)
                {
                    if(i >= r.Count)
                    {
                        return 1;
                    }
                    int comp = l[i].CompareTo(r[i]);
                    if(comp != 0)
                    {
                        return comp;
                    }
                }
                return l.Count < r.Count ? -1 : 0;
            }

            public override string ToString()
            {
                return Value != -1 ? $"{Value}" : $"[{string.Join(',', elements)}]";
            }
        }
    }
}
