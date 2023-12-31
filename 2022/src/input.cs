using System;
using System.IO;

namespace AdventOfCode2022
{
    public class Input
    {
        public string Raw { get; init; }
        public string[] Lines { get; init; }

        public Input(string file)
        {
            Raw = File.ReadAllText(file);
            Lines = Raw.Split(new string[] { "\n", "\r\n" }, StringSplitOptions.None);
        }
    }
}
