from input import Input
from collections import defaultdict
import re

Pos = tuple[str, int]
Edge = tuple[Pos, int]

def parse_input(input: Input) -> tuple[list[str], Pos, Pos]:
    map = input.lines
    start = (next(i for i, c in enumerate(map[0]) if c == "."), 0)
    target = (next(i for i, c in enumerate(map[-1]) if c == "."), len(map) - 1)
    return map, start, target

def slope_dir(char: str) -> Pos:
    match char:
        case "<": return (-1, 0)
        case ">": return (1, 0)
        case "^": return (0, -1)
        case "v": return (0, 1)

def neighbours(map: list[str], pos: Pos) -> list[tuple[Pos, Pos]]:
    res = []
    for d in [(-1, 0), (1, 0), (0, 1), (0, -1)]:
        p = (pos[0] + d[0], pos[1] + d[1])
        if p[0] >= 0 and p[0] < len(map[0]) and p[1] >= 0 and p[1] < len(map):
            if map[p[1]][p[0]] in ".<>^v":
                res.append((p, d))
    return res

def dfs_map(map: list[str], start: Pos, target: Pos, all_nodes: set[Pos]) -> int | None:
    open = [(start, 0)]
    closed = set()
    while len(open) > 0:
        curr, depth = open.pop()
        if curr == target:
            return depth
        
        if curr in closed:
            continue
        closed.add(curr)
        
        for pos, dir in neighbours(map, curr):
            char = map[pos[1]][pos[0]]
            if char in "<>^v" and dir != slope_dir(char):
                continue
            if pos == target or pos not in all_nodes:
                open.append((pos, depth + 1))
    return None

def get_nodes(map: list[str], start: Pos, end: Pos):
    nodes = set([start, end])
    for i, row in enumerate(map):
        for k, c in enumerate(row):
            n = [p for p, _ in neighbours(map, (k, i)) if map[p[1]][p[0]] in ".<>^v"]
            if c != "#" and len(n) >= 3:
                nodes.add((k, i))
    return nodes

def find_edges(map: list[str], nodes: list[Pos]) -> list[Edge]:
    all_nodes = set(nodes)
    edges = defaultdict(list)
    for node in nodes:
        for target in nodes:
            if node != target:
                if found := dfs_map(map, node, target, all_nodes):
                    edges[node].append((target, found))
    return edges

def dfs_graph(edges: list[Edge], start: Pos, target: Pos) -> int:
    best = 0
    open = [(start, 0, set())]
    while len(open) > 0:
        curr, depth, closed = open.pop()
        if curr == target:
            best = max(best, depth)
            continue

        if curr in closed:
            continue
        closed.add(curr)

        for n in edges[curr]:
            open.append((n[0], depth + n[1], closed.copy()))
    return best

def find_longest_path(map: list[str], start: Pos, target: Pos):
    nodes = get_nodes(map, start, target)
    edges = find_edges(map, nodes)    
    return dfs_graph(edges, start, target)

def solve_first(input: Input) -> object:
    map, start, target = parse_input(input)
    return find_longest_path(map, start, target)

def solve_second(input: Input) -> object:
    map, start, target = parse_input(input)
    map = [re.sub("[<>^v]", ".", row) for row in map]
    return find_longest_path(map, start, target)
