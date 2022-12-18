using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AdventOfCode2022.Day18
{
    public class Solution : SolutionBase
    {
        private Vec3[] dirs = new Vec3[6]
        {
            new Vec3(1, 0, 0),
            new Vec3(-1, 0, 0),
            new Vec3(0, 1, 0),
            new Vec3(0, -1, 0),
            new Vec3(0, 0, 1),
            new Vec3(0, 0, -1)
        };

        public Solution() : base(18, "Boiling Boulders")
        {
            
        }

        public override object? SolveFirst()
        {
            Vec3[] cubes = ParseCubes(Input.Lines);
            HashSet<Vec3> set = new HashSet<Vec3>(cubes);
            return cubes.Sum(n => dirs.Count(k => set.Contains(n + k) == false));
        }

        public override object? SolveSecond()
        {
            Vec3[] cubes = ParseCubes(Input.Lines);
            byte[,,] map = Gen3DMap(cubes);
            return CalcSurfaceArea(map);
        }

        private byte[,,] Gen3DMap(Vec3[] cubes)
        {
            int size = cubes.Max(n => Math.Max(n.x, Math.Max(n.y, n.z))) + 3;
            byte[,,] map = new byte[size, size, size];
            foreach(Vec3 cube in cubes)
            {
                map[cube.x + 1, cube.y + 1, cube.z + 1] = 1;
            }
            return map;
        }

        private int CalcSurfaceArea(byte[,,] map)
        {
            Queue<Vec3> open = new Queue<Vec3>();
            HashSet<Vec3> closed = new HashSet<Vec3>();
            open.Enqueue(new Vec3(0, 0, 0));
            int size = map.GetLength(0);
            int area = 0;
            while(open.Count > 0)
            {
                Vec3 v = open.Dequeue();
                closed.Add(v);
                foreach(Vec3 n in dirs.Select(n => v + n))
                {
                    if(n.x >= 0 && n.y >= 0 && n.z >= 0 && n.x < size && n.y < size && n.z < size)
                    {
                        if(map[n.x, n.y, n.z] == 1)
                        {
                            area++;
                            continue;
                        }
                        if(closed.Contains(n) == false)
                        {
                            open.Enqueue(n);
                            closed.Add(n);
                        }
                    }
                }
            }
            return area;
        }

        private Vec3[] ParseCubes(string[] lines)
        {
            return lines.Select(n => n.Split(',')).Select(n => new Vec3(int.Parse(n[0]), int.Parse(n[1]), int.Parse(n[2]))).ToArray();
        }

        private readonly record struct Vec3(int x, int y, int z)
        {
            public static Vec3 operator +(Vec3 lhs, Vec3 rhs) => new Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
        }
    }
}
