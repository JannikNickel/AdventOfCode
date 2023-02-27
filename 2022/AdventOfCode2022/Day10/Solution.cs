using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AdventOfCode2022.Day10
{
    public class Solution : SolutionBase
    {
        public Solution() : base(10, "Cathode-Ray Tube")
        {

        }

        public override object? SolveFirst()
        {
            int sum = 0;
            SimulateCPU(Input.Lines, delegate (int cycle, int xReg)
            {
                if((cycle - 20) % 40 == 0)
                {
                    sum += cycle * xReg;
                }
            });
            return sum;
        }

        public override object? SolveSecond()
        {
            StringBuilder screenBuffer = new StringBuilder((40 + 1) * 6);
            SimulateCPU(Input.Lines, delegate (int cycle, int xReg)
            {
                int h = (cycle - 1) % 40;
                screenBuffer.Append(h >= xReg - 1 && h <= xReg + 1 ? '#' : '.');
                if(cycle % 40 == 0)
                {
                    screenBuffer.Append('\n');
                }
            });
            return screenBuffer.ToString();
        }

        private void SimulateCPU(string[] instructions, Action<int, int> tick)
        {
            int xReg = 1;
            IEnumerator<(OpCode op, int param)> instructionLoader = LoadInstructions(instructions);
            IEnumerator<(bool completed, int result)>? currentInstruction = null;
            for(int i = 1;i <= 240;i++)
            {
                if(currentInstruction == null)
                {
                    if(instructionLoader.MoveNext() == false)
                    {
                        continue;
                    }
                    currentInstruction = ExecuteInstruction(instructionLoader.Current.op, instructionLoader.Current.param, xReg);
                }

                tick?.Invoke(i, xReg);

                currentInstruction.MoveNext();
                (bool complete, xReg) = currentInstruction.Current;
                if(complete == true)
                {
                    currentInstruction = null;
                }
            }
        }

        private IEnumerator<(OpCode op, int param)> LoadInstructions(string[] lines)
        {
            foreach((OpCode op, int param) instruction in Input.Lines.Select(n => ParseInstruction(n)))
            {
                yield return instruction;
            }
        }

        private IEnumerator<(bool completed, int result)> ExecuteInstruction(OpCode instruction, int param, int xReg)
        {
            switch(instruction)
            {
                case OpCode.noop:
                    yield return (true, xReg);
                    break;
                case OpCode.addx:
                    yield return (false, xReg);
                    yield return (true, xReg + param);
                    break;
                default:
                    break;
            }
        }

        private (OpCode op, int param) ParseInstruction(string line)
        {
            string[] parts = line.Split(' ');
            return (Enum.Parse<OpCode>(parts[0]), parts.Length > 1 ? int.Parse(parts[1]) : default);
        }
    }

    public enum OpCode
    {
        noop,
        addx
    }
}
