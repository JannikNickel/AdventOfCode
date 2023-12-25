from input import Input
from collections import defaultdict, deque
from itertools import combinations
from math import prod

def parse_input(input: Input) -> tuple[list[str], dict[str, set[str]]]:
    nodes = set()
    edges = defaultdict(set)
    for line in input.lines:
        parts = [p.rstrip(":") for p in line.split()]
        nodes |= set(parts)
        for p in parts[1:]:
            edges[parts[0]].add(p)
            edges[p].add(parts[0])
    return nodes, edges

def count_sets(nodes: set[str], edges: dict[str, set[str]]) -> list[int]:
    sets = []
    closed = set()
    for node in nodes:
        size = 0
        open = [node]
        while len(open) > 0:
            curr = open.pop()
            if curr in closed:
                continue
            closed.add(curr)
            size += 1
            for n in edges[curr]:
                open.append(n)
        if size > 0:
            sets.append(size)
    return sets

def betweenness_centrality(nodes: set[str], edges: dict[str, set[str]]) -> dict[str, float]:
    # Brandes' algorithm (https://snap.stanford.edu/class/cs224w-readings/brandes01centrality.pdf)
    res = defaultdict(int)
    for node in nodes:
        s = []
        parent = defaultdict(list)
        paths = {n: 1 if n == node else 0 for n in nodes}
        distances = {n: -1 if n != node else 0 for n in nodes}

        queue = deque([node])
        while len(queue) > 0:
            curr = queue.popleft()
            s.append(curr)
            for n in edges[curr]:
                if distances[n] < 0:
                    queue.append(n)
                    distances[n] = distances[curr] + 1
                if distances[n] == distances[curr] + 1:
                    paths[n] += paths[curr]
                    parent[n].append(curr)
        
        deps = {n: 0 for n in nodes}
        while len(s) > 0:
            w = s.pop()
            for curr in parent[w]:
                deps[curr] += (paths[curr] / paths[w]) * (1 + deps[w])
            if w != node:
                res[w] += deps[w]
    return res

def test_combinations_with_len(centrality: dict[str, float], edges: dict[str, set[str]], closed: set[tuple[tuple[str, str], ...]], length: int) -> int | None:
    nodes = set([n[0] for n in centrality[:length]])
    possible_edges = [(k, n) for k, v in edges.items() for n in v if k in nodes]
    for targets in combinations(possible_edges, 3):
        if targets in closed:
            continue
        closed.add(targets)

        e = edges.copy()
        for key, value in e.items():
            e[key] = set([e for e in value if (key, e) not in targets and (e, key) not in targets])
        parts = count_sets(nodes, e)
        if len(parts) == 2:
            return prod(parts)
    return None

def solve_first(input: Input) -> object:
    nodes, edges = parse_input(input)

    centrality = betweenness_centrality(nodes, edges)
    centrality = sorted(centrality.items(), key = lambda n: -n[1])

    length = 6
    closed = set()
    while length < len(nodes):
        if res := test_combinations_with_len(centrality, edges, closed, length):
            return res
        length += 1
    return None

def solve_second(input: Input) -> object:
    # Day 25 only has 1 puzzle
    return None
