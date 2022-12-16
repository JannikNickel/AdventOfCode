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
            (Valve start, Valve[] valves) = ParseInput(Input.Lines);
            return FindHighestPressure(start, valves.ToArray());
        }

        public override object? SolveSecond()
        {
            return null;
        }

        private int FindHighestPressure(Valve start, Valve[] valves)
        {
            int max = 0;
            Stack<State> options = new Stack<State>();
            Dictionary<StateIdentifier, int> stateScoreMemory = new Dictionary<StateIdentifier, int>();
            options.Push(new State(start, new BitMask(), 0, 0, 30));
            while(options.Count > 0)
            {
                State state = options.Pop();
                Valve v = state.current;

                StateIdentifier sId = new StateIdentifier(state.timeLeft, v.Index);
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

        private readonly record struct State(Valve current, BitMask used, int released, int pressure, int timeLeft);
        private readonly record struct StateIdentifier(int valveIndex, int time);

        public struct BitMask
        {
            public long data;

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
