using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AdventOfCode2022.Day12
{
    public class Solution : SolutionBase
    {
        private Vec2[] neighbourOffsets = new Vec2[]
        {
            new Vec2(1, 0),
            new Vec2(0, 1),
            new Vec2(-1, 0),
            new Vec2(0, -1)
        };

        public Solution() : base(12, "Hill Climbing Algorithm")
        {
            
        }

        public override object? SolveFirst()
        {
            char[,] map = ParseMap(Input.Lines, out Vec2 start, out Vec2 target);
            return BFS(map, target, start);
        }

        public override object? SolveSecond()
        {
            char[,] map = ParseMap(Input.Lines, out _, out Vec2 target);
            return BFS(map, target);
        }

        private int BFS(char[,] map, Vec2 start, Vec2? target = null)
        {
            int sizeX = map.GetLength(0);
            int sizeY = map.GetLength(1);
            int[,] depths = new int[sizeX, sizeY];
            Queue<Vec2> open = new Queue<Vec2>();
            HashSet<Vec2> closed = new HashSet<Vec2>();
            open.Enqueue(start);
            while(open.Count > 0)
            {
                Vec2 v = open.Dequeue();
                if(target == v)
                {
                    return depths[v.x, v.y];
                }
                foreach(Vec2 np in neighbourOffsets.Select(n => new Vec2(v.x + n.x, v.y + n.y)))
                {
                    if(np.x >= 0 && np.x < sizeX && np.y >= 0 && np.y < sizeY)
                    {
                        if(map[np.x, np.y] + 1 >= map[v.x, v.y] && closed.Contains(np) == false)
                        {
                            closed.Add(np);
                            depths[np.x, np.y] = depths[v.x, v.y] + 1;
                            open.Enqueue(np);
                        }
                    }
                }
            }

            return closed.Where(n => map[n.x, n.y] == 'a').Select(n => depths[n.x, n.y]).Min();
        }

        private char[,] ParseMap(string[] lines, out Vec2 start, out Vec2 target)
        {
            start = default;
            target = default;
            char[,] map = new char[lines.Length, lines[0].Length];
            for(int i = 0;i < lines.Length;i++)
            {
                for(int k = 0;k < lines[i].Length;k++)
                {
                    char c = lines[i][k];
                    if(c == 'E')
                    {
                        c = 'z';
                        target = new Vec2(i, k);
                    }
                    else if(c == 'S')
                    {
                        c = 'a';
                        start = new Vec2(i, k);
                    }
                    map[i, k] = c;
                }
            }
            return map;
        }

        private record struct Vec2(int x, int y);
    }
}
