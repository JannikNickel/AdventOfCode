from input import Input
from itertools import combinations

def parse_input(input: Input) -> tuple[list[tuple[int, int]], int, int]:
    points = [(k, i) for i, row in enumerate(input.lines) for k, char in enumerate(row) if char == "#"]
    return points, len(input.lines[0]), len(input.lines)

def expand_vertical(points: list[tuple[int, int]], width: int, height: int, expansion: int) -> tuple[list[tuple[int, int]], int, int]:
    i = 0
    while i < height:
        if not any(p[1] == i for p in points):
            for k, p in enumerate(points):
                if p[1] > i:
                    points[k] = (p[0], p[1] + expansion)
            i += expansion
            height += expansion
        i += 1
    return points, width, height

def expand(points: list[tuple[int, int]], width: int, height: int, expansion: int) -> tuple[list[tuple[int, int]], int, int]:
    points, width, height = expand_vertical(points, width, height, expansion)
    points, height, width = expand_vertical([(p[1], p[0]) for p in points], height, width, expansion)
    return [(p[1], p[0]) for p in points], width, height

def man_dst(start: tuple[int, int], end: tuple[int, int]) -> int:
    return abs(end[0] - start[0]) + abs(end[1] - start[1])

def calc_path_lengths(input: Input, expansion: int) -> int:
    points, width, height = parse_input(input)
    points, width, height = expand(points, width, height, expansion)
    return sum([man_dst(start, end) for start, end in combinations(points, 2)])

def solve_first(input: Input) -> object:
    return calc_path_lengths(input, 1)

def solve_second(input: Input) -> object:
    return calc_path_lengths(input, 1000000 - 1)
