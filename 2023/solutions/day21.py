from input import Input
from collections import deque
from math import ceil

DIRS = [(1, 0), (0, 1), (-1, 0), (0, -1)]

def parse_input(input: Input) -> tuple[list[str], tuple[int, int]]:
    map = input.lines.copy()
    start = next((k, i) for i, row in enumerate(map) for k, char in enumerate(row) if char == "S")
    map[start[1]] = map[start[1]][:start[0]] + "." + map[start[1]][start[0] + 1:]
    return (map, start)

def count_possibilities(map: list[str], start: tuple[int, int], limit: int) -> int:
    width, height = len(map[0]), len(map)
    open = deque([(start, 0)])
    known = set()
    tiles = set()
    while len(open) > 0:
        curr, steps = open.popleft()
        
        if (curr, steps) in known:
            continue
        known.add((curr, steps))

        if steps >= limit:
            tiles.add(curr)
            continue

        for dir in DIRS:
            p = (curr[0] + dir[0], curr[1] + dir[1])
            if map[p[1] % height][p[0] % width] == ".":
                open.append((p, steps + 1))
    return len(tiles)

def solve_quadratic(map: list[str], start: tuple[int, int], limit: int) -> int:
    # This only works because the row and column of the starting tile is empty in the actual input
    # Therefore the pattern expands in a shape of a rhombus (quadratic expansion)
    size = len(map)
    v = [count_possibilities(map, start, limit % size + size * i) for i in range(3)]

    # Quadratic sequence
    diff1 = [v[1] - v[0], v[2] - v[1]]
    diff2 = diff1[1] - diff1[0]
    a = diff2 // 2          # 2a = diff2       => a = diff2 / 2
    b = diff1[0] - 3 * a    # 3a + b = diff1   => b = diff1 - 3a
    c = v[0] - b - a        # a + b + c = v[0] => c = v[0] - b - a

    x = ceil(limit / size)
    return a * x ** 2 + b * x + c # ax^2 + bx + c

def solve_first(input: Input) -> object:
    map, start = parse_input(input)
    return count_possibilities(map, start, 64)

def solve_second(input: Input) -> object:
    map, start = parse_input(input)
    return solve_quadratic(map, start, 26501365)
