using System;
using System.Collections.Generic;
using System.Linq;

namespace AdventOfCode2022.Day08
{
    public class Solution : SolutionBase
    {
        public Solution() : base(8, "Treetop Tree House")
        {
            
        }

        public override object? SolveFirst()
        {
            int[,] trees = ParseTreeHeights(Input.Lines, out int gridSize);
            int[,] visibility = new int[gridSize, gridSize];
            CalcVisibility(trees, visibility, gridSize, (1, 0));
            CalcVisibility(trees, visibility, gridSize, (-1, 0));
            CalcVisibility(trees, visibility, gridSize, (0, 1));
            CalcVisibility(trees, visibility, gridSize, (0, -1));
            return visibility.Cast<int>().Count(n => n != 0);
        }

        public override object? SolveSecond()
        {
            int[,] trees = ParseTreeHeights(Input.Lines, out int gridSize);
            int maxScenicScore = 0;
            for(int i = 0;i < gridSize;i++)
            {
                for(int k = 0;k < gridSize;k++)
                {
                    maxScenicScore = Math.Max(maxScenicScore, CalcScenicScore(trees, gridSize, i, k));
                }
            }
            return maxScenicScore;
        }

        private void CalcVisibility(int[,] trees, int[,] visibility, int gridSize, (int x, int y) dir)
        {
            int i = 0;
            int k = 0;
            int iMult = (dir.x < 0 ? -1 : 1);
            int kMult = (dir.y < 0 ? -1 : 1);
            int iStart = (dir.x < 0 ? (gridSize - 1) : 0);
            int kStart = (dir.y < 0 ? (gridSize - 1) : 0);
            ref int outer = ref (dir.x != 0 ? ref k : ref i);
            ref int inner = ref (dir.x != 0 ? ref i : ref k);

            int tallest = -1;
            while(outer < gridSize)
            {
                int x = i * iMult + iStart;
                int y = k * kMult + kStart;

                int tree = trees[x, y];
                if(tree > tallest)
                {
                    visibility[x, y]++;
                    tallest = tree;

                    //No other trees can be visible beyond this one
                    if(tallest == 9)
                    {
                        inner = gridSize;
                    }
                }

                i += Math.Abs(dir.x);
                k += Math.Abs(dir.y);
                if(inner >= gridSize)
                {
                    inner = 0;
                    outer++;
                    tallest = -1;
                }
            }
        }

        private int CalcScenicScore(int[,] trees, int gridSize, int i, int k)
        {
            int height = trees[i, k];

            int NextTreeDst((int x, int y) dir)
            {
                int lx = i + dir.x;
                int ly = k + dir.y;
                int sum = 0;

                while(lx >= 0 && ly >= 0 && lx < gridSize && ly < gridSize)
                {
                    sum++;
                    if(trees[lx, ly] >= height)
                    {
                        break;
                    }
                    lx += dir.x;
                    ly += dir.y;
                }
                return sum;
            }

            return NextTreeDst((1, 0)) * NextTreeDst((-1, 0)) * NextTreeDst((0, 1)) * NextTreeDst((0, -1));
        }

        private int[,] ParseTreeHeights(string[] lines, out int gridSize)
        {
            gridSize = lines.Length;
            int[,] grid = new int[gridSize, gridSize];
            for(int i = 0;i < gridSize;i++)
            {
                for(int k = 0;k < gridSize;k++)
                {
                    grid[i, k] = lines[i][k] - '0';
                }
            }
            return grid;
        }
    }
}
