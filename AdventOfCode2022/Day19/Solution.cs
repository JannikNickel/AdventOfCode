using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;

namespace AdventOfCode2022.Day19
{
    public class Solution : SolutionBase
    {
        public Solution() : base(19, "Not Enough Minerals")
        {
            
        }

        public override object? SolveFirst()
        {
            Blueprint[] bps = ParseBlueprints(Input.Lines);
            return bps.Sum(n => n.id * FindBestBpOutcome(n, 24));
        }

        public override object? SolveSecond()
        {
            Blueprint[] bps = ParseBlueprints(Input.Lines);
            return bps.Take(3).Select(n => FindBestBpOutcome(n, 32)).Aggregate((a, b) => a * b);
        }

        private int FindBestBpOutcome(Blueprint bp, int time)
        {
            Stack<State> states = new Stack<State>();
            HashSet<State> seen = new HashSet<State>();
            states.Push(new State(time, new Inventory(1, 0, 0, 0, 0, 0, 0, 0)));
            int best = 0;

            while(states.Count > 0)
            {
                State s = states.Pop();

                if(seen.Contains(s))
                {
                    continue;
                }
                seen.Add(s);

                if(s.time <= 0)
                {
                    best = Math.Max(best, s.inventory.geodes);
                    continue;
                }

                if(s.inventory.geodes + Enumerable.Range(0, s.time).Sum(n => s.inventory.geodeRobots + n) < best)
                {
                    continue;
                }

                int t = s.time - 1;
                Inventory nextInventory = s.inventory.SimulateMinute();

                if(nextInventory.oreRobots < bp.MaxOreRequired && bp.oreRobotRecipe.CanBeUsed(s.inventory))
                {
                    states.Push(new State(t, bp.oreRobotRecipe.UseItems(nextInventory) with { oreRobots = nextInventory.oreRobots + 1 }));
                }
                if(nextInventory.clayRobots < bp.MaxClayRequired && bp.clayRobotRecipe.CanBeUsed(s.inventory))
                {
                    states.Push(new State(t, bp.clayRobotRecipe.UseItems(nextInventory) with { clayRobots = nextInventory.clayRobots + 1 }));
                }
                if(nextInventory.obsidianRobots < bp.MaxObsidianRequired && bp.obsidianRobotRecipe.CanBeUsed(s.inventory))
                {
                    states.Push(new State(t, bp.obsidianRobotRecipe.UseItems(nextInventory) with { obsidianRobots = nextInventory.obsidianRobots + 1 }));
                }
                if(bp.geodeRobotRecipe.CanBeUsed(s.inventory))
                {
                    states.Push(new State(t, bp.geodeRobotRecipe.UseItems(nextInventory) with { geodeRobots = nextInventory.geodeRobots + 1 }));
                }
                states.Push(new State(t, nextInventory));
            }

            return best;
        }

        private Blueprint[] ParseBlueprints(string[] lines)
        {
            return lines.Select(n => Blueprint.Parse(n)).ToArray();
        }

        private record struct State(int time, Inventory inventory);

        private record struct Inventory(int oreRobots, int clayRobots, int obsidianRobots, int geodeRobots, int ore, int clay, int obsidian, int geodes)
        {
            public Inventory SimulateMinute() => new Inventory(oreRobots, clayRobots, obsidianRobots, geodeRobots, ore + oreRobots, clay + clayRobots, obsidian + obsidianRobots, geodes + geodeRobots);
        }

        private class Blueprint
        {
            public readonly int id;
            public readonly Recipe oreRobotRecipe;
            public readonly Recipe clayRobotRecipe;
            public readonly Recipe obsidianRobotRecipe;
            public readonly Recipe geodeRobotRecipe;

            public int MaxOreRequired => oreRobotRecipe.ore + clayRobotRecipe.ore + obsidianRobotRecipe.ore + geodeRobotRecipe.ore;
            public int MaxClayRequired => oreRobotRecipe.clay + clayRobotRecipe.clay + obsidianRobotRecipe.clay + geodeRobotRecipe.clay;
            public int MaxObsidianRequired => oreRobotRecipe.obsidian + clayRobotRecipe.obsidian + obsidianRobotRecipe.obsidian + geodeRobotRecipe.obsidian;

            public Blueprint(int id, Recipe oreRobotRecipe, Recipe clayRobotRecipe, Recipe obsidianRobotRecipe, Recipe geodeRobotRecipe)
            {
                this.id = id;
                this.oreRobotRecipe = oreRobotRecipe;
                this.clayRobotRecipe = clayRobotRecipe;
                this.obsidianRobotRecipe = obsidianRobotRecipe;
                this.geodeRobotRecipe = geodeRobotRecipe;
            }

            public static Blueprint Parse(string line)
            {
                MatchCollection matches = Regex.Matches(line, @"\d+");
                return new Blueprint(int.Parse(matches[0].Value),
                    new Recipe(int.Parse(matches[1].Value), 0, 0),
                    new Recipe(int.Parse(matches[2].Value), 0, 0),
                    new Recipe(int.Parse(matches[3].Value), int.Parse(matches[4].Value), 0),
                    new Recipe(int.Parse(matches[5].Value), 0, int.Parse(matches[6].Value)));
            }
        }

        private readonly record struct Recipe(int ore, int clay, int obsidian)
        {
            public bool CanBeUsed(in Inventory inv)
            {
                return ore <= inv.ore && clay <= inv.clay && obsidian <= inv.obsidian;
            }

            public Inventory UseItems(Inventory inventory)
            {
                return inventory with { ore = inventory.ore - ore, clay = inventory.clay - clay, obsidian = inventory.obsidian - obsidian };
            }
        }
    }
}
