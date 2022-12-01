using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AdventOfCode2022
{
    public abstract class SolutionBase
    {
        private const string inputDir = "./Input/";

        public readonly int day;
        public readonly string title;
        private Input? input;

        protected Input Input => input!;

        protected SolutionBase(int day, string title)
        {
            this.day = day;
            this.title = title;
        }

        public void Prepare()
        {
            input = new Input($"{inputDir}{day:D2}.txt");
        }

        public abstract object? SolveFirst();
        public abstract object? SolveSecond();

        public static IEnumerable<SolutionBase> Solutions()
        {
            for(int i = 0;i < 25;i++)
            {
                Type? t = Type.GetType($"AdventOfCode2022.Day{i:D2}.Solution");
                if(t != null)
                {
                    SolutionBase? instance = Activator.CreateInstance(t) as SolutionBase;
                    if(instance != null)
                    {
                        yield return instance;
                    }
                }
            }
        }
    }
}
