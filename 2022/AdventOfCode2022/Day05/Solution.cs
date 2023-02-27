using System;
using System.Collections.Generic;
using System.Linq;

namespace AdventOfCode2022.Day05
{
    public class Solution : SolutionBase
    {
        public Solution() : base(5, "Supply Stacks")
        {
            
        }

        public override object? SolveFirst()
        {
            (Stack<char>[] stacks, string[] moves) = ParseInput(Input.Raw);
            foreach(string move in moves)
            {
                (int amount, int from, int to) = ParseMove(move);
                for(int i = 0;i < amount;i++)
                {
                    stacks[to - 1].Push(stacks[from - 1].Pop());
                }
            }
            return string.Concat(stacks.Select(n => n.Peek()));
        }

        public override object? SolveSecond()
        {
            Stack<char> crane = new Stack<char>();
            (Stack<char>[] stacks, string[] moves) = ParseInput(Input.Raw);
            foreach(string move in moves)
            {
                (int amount, int from, int to) = ParseMove(move);
                for(int i = 0;i < amount;i++)
                {
                    crane.Push(stacks[from - 1].Pop());
                }
                for(int i = 0;i < amount;i++)
                {
                    stacks[to - 1].Push(crane.Pop());
                }
                crane.Clear();
            }
            return string.Concat(stacks.Select(n => n.Peek()));
        }

        private (Stack<char>[] stacks, string[] moves) ParseInput(string input)
        {
            string[] parts = input.Split(new string[] { "\n\n", "\r\n\r\n" }, StringSplitOptions.None);
            string[] stackLines = parts[0].Split(new string[] { "\n", "\r\n" }, StringSplitOptions.None);
            int stackCount = int.Parse(stackLines.Last().Last(n => n != ' ').ToString());
            Stack<char>[] stacks = new Stack<char>[stackCount];
            for(int i = 0;i < stackCount;i++)
            {
                stacks[i] = new Stack<char>();
                int offset = 1 + i * 4;
                for(int k = stackLines.Length - 1 - 1;k >= 0;k--)
                {
                    if(stackLines[k][offset] != ' ')
                    {
                        stacks[i].Push(stackLines[k][offset]);
                    }
                }
            }

            return (stacks, parts[1].Split(new string[] { "\n", "\r\n" }, StringSplitOptions.None));
        }

        private (int amount, int from, int to) ParseMove(string line)
        {
            string[] parts = line.Split(new string[] { "move ", " from ", " to " }, StringSplitOptions.RemoveEmptyEntries);
            return (int.Parse(parts[0]), int.Parse(parts[1]), int.Parse(parts[2]));
        }
    }
}
