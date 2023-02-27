using System;
using System.Collections.Generic;
using System.Linq;

namespace AdventOfCode2022.Day02
{
    public class Solution : SolutionBase
    {
        public Solution() : base(2, "Rock Paper Scissors")
        {
            
        }

        public override object? SolveFirst()
        {
            int score = 0;
            foreach(string line in Input.Lines)
            {
                (char opponent, char own) = ParseLine(line);
                Shape opponentShape = (Shape)(opponent - 'A');
                Shape ownShape = (Shape)(own - 'X');
                score += CalcRoundScore(opponentShape, ownShape);
            }
            return score;
        }

        public override object? SolveSecond()
        {
            int score = 0;
            foreach(string line in Input.Lines)
            {
                (char opponent, char own) = ParseLine(line);
                Shape opponentShape = (Shape)(opponent - 'A');
                Shape ownShape = opponentShape;

                if(own != 'Y')
                {
                    ownShape = opponentShape switch
                    {
                        Shape.Rock => own == 'X' ? Shape.Scissors : Shape.Paper,
                        Shape.Paper => own == 'X' ? Shape.Rock : Shape.Scissors,
                        Shape.Scissors => own == 'X' ? Shape.Paper : Shape.Rock,
                        _ => default
                    };
                }

                score += CalcRoundScore(opponentShape, ownShape);
            }
            return score;
        }

        private (char, char) ParseLine(string line)
        {
            string[] parts = line.Split(' ');
            return (parts[0][0], parts[1][0]);
        }

        private static int CalcRoundScore(Shape opponentShape, Shape ownShape)
        {
            int score = (int)ownShape + 1;
            if((ownShape == Shape.Rock && opponentShape == Shape.Scissors) || (ownShape == Shape.Scissors && opponentShape == Shape.Paper) || (ownShape == Shape.Paper && opponentShape == Shape.Rock))
            {
                score += 6;
            }
            else if(ownShape == opponentShape)
            {
                score += 3;
            }
            return score;
        }

        private enum Shape
        {
            Rock = 0,
            Paper = 1,
            Scissors = 2
        }
    }
}
