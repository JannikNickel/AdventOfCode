using System;
using System.Collections.Generic;

namespace AdventOfCode2022
{
    public static class EnumerableExtensions
    {
        public static void ForEach<T>(this IEnumerable<T> source, Action<T> action)
        {
            foreach(T item in source)
            {
                action?.Invoke(item);
            }
        }
    }
}
