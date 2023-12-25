from input import Input
from collections import defaultdict
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

def draw(nodes: set[str], edges: dict[str, set[str]]):
    import graphviz
    dot = graphviz.Graph()
    for n in nodes:
        dot.node(n)
    closed = set()
    for origin, targets in edges.items():
        for target in targets:
            if (origin, target) not in closed and (target, origin) not in closed:
                closed.add((origin, target))
                dot.edge(origin, target)
    dot.render("test.dot")
    # dot -Ksfdp -o output.svg -Tsvg test.dot

def solve_first(input: Input) -> object:
    nodes, edges = parse_input(input)

    #draw(nodes, edges)
    target = [("gxv", "tpn"), ("zcj", "rtt"), ("txl", "hxq")]

    for key, value in edges.items():
        edges[key] = set([e for e in value if (key, e) not in target and (e, key) not in target])    
    return prod(count_sets(nodes, edges))

def solve_second(input: Input) -> object:
    # Day 25 only has 1 puzzle
    return None
