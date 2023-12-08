from input import Input
from typing import Callable
from math import lcm
import re

def parse_input(input: Input) -> tuple[str, dict[str, tuple[str, str]]]:
    dirs = input.lines[0]
    graph = {parts[0]: (parts[1], parts[2]) for parts in [re.findall(r"[A-Z0-9]+", line) for line in input.lines[2:]]}
    return dirs, graph

def find_steps(graph: dict[str, tuple[str, str]], dirs: str, start: str, predicate: Callable[[str], bool]) -> int:
    i = 0
    while not predicate(start):
        start = graph[start][dirs[i % len(dirs)] == "R"]
        i += 1
    return i

def solve_first(input: Input) -> object:
    dirs, graph = parse_input(input)
    return find_steps(graph, dirs, "AAA", lambda n: n == "ZZZ")

def solve_second(input: Input) -> object:
    dirs, graph = parse_input(input)
    starts = [node for node in graph.keys() if node[-1] == "A"]
    steps = [find_steps(graph, dirs, start, lambda n: n[-1] == "Z") for start in starts]
    return lcm(*steps)
