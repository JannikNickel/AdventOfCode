using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;

namespace AdventOfCode2022.Day20
{
    public class Solution : SolutionBase
    {
        public Solution() : base(20, "Grove Positioning System")
        {
            
        }

        public override object? SolveFirst()
        {
            RingItem<long>[] numbers = ParseNumbers(Input.Lines).Select((n, i) => new RingItem<long>(n, i)).ToArray();
            Ring<long> r = new Ring<long>(numbers);
            foreach(RingItem<long> n in numbers)
            {
                r.Move(n.Index, n.Value);
            }
            long index = r.FindIndex(n => n == 0);
            return r[index + 1000] + r[index + 2000] + r[index + 3000];
        }

        public override object? SolveSecond()
        {
            RingItem<long>[] numbers = ParseNumbers(Input.Lines).Select((n, i) => new RingItem<long>(n * 811589153, i)).ToArray();
            Ring<long> r = new Ring<long>(numbers);
            for(long i = 0;i < 10;i++)
            {
                foreach(RingItem<long> n in numbers)
                {
                    r.Move(n.Index, n.Value);
                }
            }
            long index = r.FindIndex(n => n == 0);
            return r[index + 1000] + r[index + 2000] + r[index + 3000];
        }

        private long[] ParseNumbers(string[] lines)
        {
            return lines.Select(n => long.Parse(n)).ToArray();
        }

        private class Ring<T>
        {
            private RingItem<T>[] items;

            public T this[long index]
            {
                get => items[index % items.Length].Value;
            }

            public Ring(RingItem<T>[] items)
            {
                this.items = new RingItem<T>[items.Length];
                Array.Copy(items, this.items, items.Length);
            }

            public void Move(long index, long offset)
            {
                offset %= (items.Length - 1);

                long dir = offset < 0 ? -1 : 1;
                offset = Math.Abs(offset);
                for(long i = 1;i <= offset;i++)
                {
                    long nIndex = NormalizeIndex(index + dir);
                    Swap(index, nIndex);
                    index = NormalizeIndex(index + dir);
                }
            }

            private long NormalizeIndex(long index)
            {
                return index < 0 ? (index + items.Length) : (index % items.Length);
            }

            private void Swap(long index, long nIndex)
            {
                RingItem<T> item = items[index];
                items[index] = items[nIndex];
                items[nIndex] = item;
                item.Index = nIndex;
                items[index].Index = index;
            }

            public long FindIndex(Predicate<T> predicate)
            {
                return Array.FindIndex(items, n => predicate(n.Value));
            }
        }

        private class RingItem<T>
        {
            public T Value { get; init; }
            public long Index { get; set; }

            public RingItem(T value, long index)
            {
                Value = value;
                Index = index;
            }
        }
    }
}
