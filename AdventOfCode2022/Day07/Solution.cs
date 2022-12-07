using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AdventOfCode2022.Day07
{
    public class Solution : SolutionBase
    {
        public Solution() : base(7, "No Space Left On Device")
        {
            
        }

        public override object? SolveFirst()
        {
            Node<FileSystemEntry> root = ReadDirectoryTree(Input.Lines);
            int sum = root.AllChildren.Where(n => n.Data.size == 0).Select(n => CalcDirSize(n)).Where(n => n <= 100000).Sum();
            return sum;
        }

        public override object? SolveSecond()
        {
            Node<FileSystemEntry> root = ReadDirectoryTree(Input.Lines);

            const int totalSpace = 70000000;
            const int updateSpace = 30000000;
            int usedSpace = CalcDirSize(root);
            int amountToFree = updateSpace - (totalSpace - usedSpace);

            int smallest = root.AllChildren.Where(n => n.Data.size == 0).Select(n => CalcDirSize(n)).Where(n => n >= amountToFree).Min();
            return smallest;
        }

        private Node<FileSystemEntry> ReadDirectoryTree(string[] lines)
        {
            Node<FileSystemEntry>? node = new Node<FileSystemEntry>(new FileSystemEntry("/", 0), null);
            Node<FileSystemEntry> root = node;
            foreach(string line in lines)
            {
                string[] parts = line.Split(' ');
                if(parts[0] == "$")
                {
                    string cmd = parts[1];
                    if(cmd == "ls")
                    {
                        continue;
                    }
                    if(cmd == "cd")
                    {
                        string arg = parts[2];
                        if(arg != "/")
                        {
                            if(arg == "..")
                            {
                                node = node?.Parent;
                            }
                            else
                            {
                                node = node?.FindChild(n => n.name == arg) ?? node?.AddChild(new FileSystemEntry(arg, 0));
                            }
                        }
                    }
                }
                else
                {
                    bool dir = parts[0] == "dir";
                    int size = !dir ? int.Parse(parts[0]) : 0;
                    string entryName = parts[1];
                    if(node?.FindChild(n => n.name == entryName) == null)
                    {
                        node?.AddChild(new FileSystemEntry(entryName, size));
                    }
                }
            }

            return root;
        }
        private int CalcDirSize(Node<FileSystemEntry> tree)
        {
            return tree.Sum(n => n.size);
        }

        private class Node<T> : IEnumerable<T>
        {
            public T Data { get; init; }
            public Node<T>? Parent { get; init; }
            public Node<T>? Root
            {
                get
                {
                    Node<T> root = this;
                    while(root.Parent != null)
                    {
                        root = root.Parent;
                    }
                    return root;
                }
            }

            public IEnumerable<Node<T>> AllChildren
            {
                get
                {
                    Queue<Node<T>> open = new Queue<Node<T>>();
                    open.Enqueue(this);
                    while(open.Count > 0)
                    {
                        Node<T> n = open.Dequeue();
                        foreach(Node<T> child in n.children)
                        {
                            open.Enqueue(child);
                        }
                        yield return n;
                    }
                }
            }

            private List<Node<T>> children = new List<Node<T>>();

            public Node(T data, Node<T>? parent)
            {
                Data = data;
                Parent = parent;
            }

            public Node<T> AddChild(T data)
            {
                Node<T> n = new Node<T>(data, this);
                children.Add(n);
                return n;
            }

            public Node<T>? FindChild(Predicate<T> pred)
            {
                foreach(Node<T> child in children)
                {
                    if(pred(child.Data))
                    {
                        return child;
                    }
                }
                return null;
            }

            public IEnumerator<T> GetEnumerator()
            {
                foreach(Node<T> item in AllChildren)
                {
                    yield return item.Data;
                }
            }

            IEnumerator IEnumerable.GetEnumerator()
            {
                throw new NotImplementedException();
            }
        }

        private readonly record struct FileSystemEntry(string name, int size);
    }
}
