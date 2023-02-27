using System;
using System.Collections.Generic;
using System.Linq;

namespace AdventOfCode2022.Day06
{
    public class Solution : SolutionBase
    {
        public Solution() : base(6, "Tuning Trouble")
        {
            
        }

        public override object? SolveFirst()
        {
            return FindUniqueCharEnd(Input.Raw, 4);
        }

        public override object? SolveSecond()
        {
            return FindUniqueCharEnd(Input.Raw, 14);
        }

        private int FindUniqueCharEnd(string line, int length)
        {
            int l = line.Length - (length - 1);
            for(int i = 0;i < l;i++)
            {
                ReadOnlySpan<char> sequence = line.AsSpan(i, length);
                if(UniqueChars(sequence))
                {
                    return i + length;
                }
            }
            return -1;
        }

        private bool UniqueChars(ReadOnlySpan<char> sequence)
        {
            int mask = 0;
            for(int i = 0;i < sequence.Length;i++)
            {
                int bit = sequence[i] - 'a';
                if((mask & (1 << bit)) != 0)
                {
                    return false;
                }
                mask |= (1 << bit);
            }
            return true;
        }
    }
}
