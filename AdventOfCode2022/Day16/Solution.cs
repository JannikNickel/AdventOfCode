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
            //To skip more permutations
            //TODO calc distances between all nodes
            //TODO calc if paths are even possible?

            (Valve start, Valve[] valves) = ParseInput(Input.Lines);
            IEnumerable<Valve> reducedValveSet = valves.Where(n => n == start || n.FlowRate > 0);
            int[,] distanceMatrix = CalcDistanceMatrix(valves);
            int max = 0;
            foreach(var order in Permutations(reducedValveSet, reducedValveSet.Count()))
            {
                if(order.First().Name != start.Name)
                {
                    break;
                }
                max = Math.Max(max, SimulateOrder(order, distanceMatrix));
            }
            return max;
        }

        public override object? SolveSecond()
        {
            return null;
        }

        private int[,] CalcDistanceMatrix(Valve[] valves)
        {
            int amount = valves.Length;
            int[,] matrix = new int[amount, amount];
            for(int i = 0;i < amount;i++)
            {
                for(int k = i + 1;k < amount;k++)
                {
                    int dst = CalcValveDistance(valves[i], valves[k], amount);
                    matrix[i, k] = dst;
                    matrix[k, i] = dst;
                }
            }
            return matrix;
        }

        private int CalcValveDistance(Valve from, Valve to, int valveCount)
        {
            Queue<Valve> open = new Queue<Valve>();
            HashSet<Valve> closed = new HashSet<Valve>();
            open.Enqueue(from);
            int[] depths = new int[valveCount];
            
            while(open.Count > 0)
            {
                Valve v = open.Dequeue();
                closed.Add(v);

                if(v == to)
                {
                    return depths[v.Index];
                }

                foreach(Valve n in v.Tunnels)
                {
                    if(closed.Contains(n) == false)
                    {
                        open.Enqueue(n);
                        depths[n.Index] = depths[v.Index] + 1;
                    }
                }
            }

            return -1;
        }

        private IEnumerator<Valve> Iterator(IEnumerable<Valve> collection)
        {
            foreach(Valve valve in collection)
            {
                yield return valve;
            }
        }

        private int SimulateOrder(IEnumerable<Valve> order, int[,] distanceMatrix)
        {
            int sum = 0;
            int minute = 30;
            IEnumerator<Valve> iterator = Iterator(order);
            iterator.MoveNext();
            Valve current = iterator.Current;
            do
            {
                if(current.FlowRate != 0)
                {
                    minute--;
                    sum += current.FlowRate * minute;
                }
                if(!iterator.MoveNext())
                {
                    break;
                }
                Valve next = iterator.Current;
                minute -= distanceMatrix[current.Index, next.Index];
                current = next;
            }
            while(minute > 0);

            return sum;
        }

        private IEnumerable<IEnumerable<T>> Permutations<T>(IEnumerable<T> collection, int length)
        {
            if(length == 1)
            {
                return collection.Select(n => new T[] { n });
            }
            return Permutations(collection, length - 1)
                .SelectMany(n => collection.Where(k => n.Contains(k) == false), (t1, t2) => t1.Concat(new T[] { t2 }));
        }

        private (Valve root, Valve[] valves) ParseInput(string[] lines)
        {
            Valve[] valves = lines.OrderByDescending(n => n.StartsWith("Valve AA")).Select((line, index) => new Valve(Regex.Match(line, @"[A-Z][A-Z]").Value, int.Parse(Regex.Match(line, @"\d+").Value), index)).ToArray();
            for(int i = 0;i < lines.Length;i++)
            {
                valves[i].Tunnels = Regex.Matches(lines[i], @"[A-Z][A-Z]").Skip(1).Select(n => valves.First(k => k.Name == n.Value)).ToArray();
            }
            return (valves.First(n => n.Name == "AA"), valves);
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
            }

            public override string ToString()
            {
                return $"{Name} ({FlowRate})";
            }
        }
    }
}
