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
            return FindHighestPressure(start, valves.ToArray(), 30);
        }

        public override object? SolveSecond()
        {
            (Valve start, Valve[] valves) = ParseInput(Input.Lines);
            return FindHighestPressureTogether(start, valves.ToArray(), 26);//2316
        }

        private int FindHighestPressure(Valve start, Valve[] valves, int time)
        {
            int max = 0;
            Stack<State> options = new Stack<State>();
            Dictionary<StateIdentifier, int> stateScoreMemory = new Dictionary<StateIdentifier, int>();
            options.Push(new State(start, new BitMask(), 0, 0, time));
            while(options.Count > 0)
            {
                State state = options.Pop();
                Valve v = state.current;

                StateIdentifier sId = new StateIdentifier(v.Index, state.timeLeft);
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

        private int FindHighestPressureTogether(Valve start, Valve[] valves, int time)
        {
            int max = 0;
            Stack<StateV2> options = new Stack<StateV2>();
            Dictionary<StateIdentifierV2, int> stateScoreMemory = new Dictionary<StateIdentifierV2, int>();
            options.Push(new StateV2(start, start, new BitMask(), 0, 0, time));
            while(options.Count > 0)
            {
                StateV2 state = options.Pop();
                Valve v = state.current;
                Valve e = state.elephant;

                StateIdentifierV2 sId = new StateIdentifierV2(v.Index, e.Index, state.timeLeft);
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

                    AddElephantValveOpenIfPossible(options, v, e, state, used, v.FlowRate + e.FlowRate);
                    AddElephantMoves(options, v, e, state, used, v.FlowRate);
                }
                foreach(Valve vTunnel in v.Tunnels)
                {
                    AddElephantValveOpenIfPossible(options, vTunnel, e, state, state.used, e.FlowRate);
                    AddElephantMoves(options, vTunnel, e, state, state.used, 0);
                }
            }
            return max;
        }

        private void AddElephantValveOpenIfPossible(Stack<StateV2> options, Valve v, Valve e, StateV2 state, BitMask used, int pressureIncrease)
        {
            if(e.FlowRate > 0 && used[e.Index] == false)
            {
                used[e.Index] = true;
                int p = state.pressure + pressureIncrease;
                options.Push(new StateV2(v, e, used, state.released + p, p, state.timeLeft - 1));
            }
        }

        private void AddElephantMoves(Stack<StateV2> options, Valve v, Valve e, StateV2 state, BitMask used, int pressureIncrease)
        {
            foreach(Valve eTunnel in e.Tunnels)
            {
                int p = state.pressure + pressureIncrease;
                options.Push(new StateV2(v, eTunnel, used, state.released + p, p, state.timeLeft - 1));
            }
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
        private readonly record struct StateV2(Valve current, Valve elephant, BitMask used, int released, int pressure, int timeLeft);
        private readonly record struct StateIdentifierV2(int valveIndex, int elefantIndex, int time);

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
