using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Threading;

namespace AdventOfCode2022.Day16
{
    public class Solution : SolutionBase
    {
        public Solution() : base(16, "Proboscidea Volcanium")
        {

        }

        public override object? SolveFirst()
        {
            (Valve start, _) = ParseInput(Input.Lines);
            return FindHighestPressure(start, new BitMask(), 30);
        }

        public override object? SolveSecond()
        {
            (Valve start, Valve[] valves) = ParseInput(Input.Lines);
            IEnumerable<IEnumerable<int>>[] splits = Partitions(valves.Where(n => n.FlowRate > 0).Select(n => n.Index).ToArray(), 2).ToArray();
            int max = 0;
            object l = new object();
            Parallel.For(0, splits.Length, (int i) =>
            {
                int result = FindHighestPressure(start, new BitMask(splits[i].First()), 26) + FindHighestPressure(start, new BitMask(splits[i].Skip(1).First()), 26);
                lock(l)
                {
                    max = Math.Max(max, result);
                }
            });
            return max;
        }

        private int FindHighestPressure(Valve start, BitMask initalUsed, int time)
        {
            int max = 0;
            Stack<State> options = new Stack<State>();
            Dictionary<StateIdentifier, int> stateScoreMemory = new Dictionary<StateIdentifier, int>();
            options.Push(new State(start, initalUsed, 0, 0, time));
            while(options.Count > 0)
            {
                State state = options.Pop();
                Valve v = state.current;

                StateIdentifier sId = new StateIdentifier(v.Index, state.used, state.timeLeft);
                if(stateScoreMemory.TryGetValue(sId, out int released) && released >= state.released)
                {
                    continue;
                }
                stateScoreMemory[sId] = state.released;
                max = Math.Max(max, state.released);

                if(state.timeLeft <= 1)
                {
                    continue;
                }

                if(v.FlowRate > 0 && state.used[v.Index] == false)
                {
                    BitMask used = state.used;
                    used[v.Index] = true;
                    int p = state.pressure + v.FlowRate;
                    options.Push(new State(v, used, state.released + p, p, state.timeLeft - 1));
                }
                foreach(Valve n in v.Tunnels)
                {
                    options.Push(new State(n, state.used, state.released + state.pressure, state.pressure, state.timeLeft - 1));
                }
            }
            return max;
        }

        private (Valve root, Valve[] valves) ParseInput(string[] lines)
        {
            Valve[] valves = lines.Select((line, index) => new Valve(Regex.Match(line, @"[A-Z][A-Z]").Value, int.Parse(Regex.Match(line, @"\d+").Value), index)).ToArray();
            for(int i = 0;i < lines.Length;i++)
            {
                valves[i].Tunnels = Regex.Matches(lines[i], @"[A-Z][A-Z]").Skip(1).Select(n => valves.First(k => k.Name == n.Value)).ToArray();
            }
            return (valves.First(n => n.Name == "AA"), valves);
        }

        private IEnumerable<IEnumerable<IEnumerable<T>>> Partitions<T>(T[] source, int splits)
        {
            IEnumerable<IEnumerable<IEnumerable<T>>> Generate(List<List<T>> groups, int length, int index)
            {
                if(index >= length)
                {
                    yield return new List<List<T>>(groups.Select(g => g.ToList()));
                }
                else
                {
                    if(length - index > splits - groups.Count)
                    {
                        foreach(List<T> group in new List<List<T>>(groups))
                        {
                            group.Add(source[index]);
                            foreach(IEnumerable<IEnumerable<T>> part in Generate(groups, length, index + 1))
                            {
                                yield return part;
                            }
                            group.RemoveAt(group.Count - 1);
                        }
                    }

                    if(groups.Count < splits)
                    {
                        groups.Add(new List<T>() { source[index] });
                        foreach(IEnumerable<IEnumerable<T>> part in Generate(groups, length, index + 1))
                        {
                            yield return part;
                        }
                        groups.RemoveAt(groups.Count - 1);
                    }
                }
            }

            return Generate(new List<List<T>>(), source.Length, 0);
        }

        private readonly record struct State(Valve current, BitMask used, int released, int pressure, int timeLeft);
        private readonly record struct StateIdentifier(int valveIndex, long used, int time);

        public struct BitMask
        {
            private long data;

            public bool this[int index]
            {
                get => (data & (1L << index)) != 0;
                set
                {
                    if(value)
                    {
                        data |= (1L << index);
                    }
                    else
                    {
                        data &= ~(1L << index);
                    }
                }
            }

            public BitMask(IEnumerable<int> indices)
            {
                data = 0;
                foreach(int i in indices)
                {
                    this[i] = true;
                }
            }

            public static implicit operator long(BitMask mask)
            {
                return mask.data;
            }
        }

        public class Valve
        {
            public string Name { get; init; }
            public int FlowRate { get; init; }
            public int Index { get; init; }
            public Valve[] Tunnels { get; set; }

            public Valve(string name, int flowRate, int index)
            {
                Name = name;
                FlowRate = flowRate;
                Index = index;
                Tunnels = Array.Empty<Valve>();
            }

            public override string ToString() => $"{Index} > {Name} ({FlowRate})";
        }
    }
}
