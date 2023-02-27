using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AdventOfCode2022.Day25
{
    public class Solution : SolutionBase
    {
        private (char display, int value)[] digits = new (char, int)[]
        {
            ('0', 0),
            ('1', 1),
            ('2', 2),
            ('=', -2),
            ('-', -1),
        };

        public Solution() : base(25, "Full of Hot Air")
        {
            
        }

        public override object? SolveFirst()
        {
            long sum = Input.Lines.Sum(n => SnafuToDec(n));
            return DecToSnafu(sum);
        }

        public override object? SolveSecond()
        {
            //Day 25 only has 1 puzzle
            return null;
        }

        private long SnafuToDec(string snafu)
        {
            long dec = 0;
            for(int i = 0;i < snafu.Length;i++)
            {
                char c = snafu[snafu.Length - 1 - i];
                dec += (long)Math.Pow(5, i) * digits.First(n => n.display == c).value;
            }
            return dec;
        }

        private string DecToSnafu(long dec)
        {
            string result = "";
            while(dec != 0)
            {
                long remainder = dec % 5;
                result += digits[remainder].display;
                dec /= 5;
                if(remainder > 2)
                {
                    dec += 1;
                }
            }
            return new string(result.Reverse().ToArray());
        }
    }
}
