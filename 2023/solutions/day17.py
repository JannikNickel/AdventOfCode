from input import Input
from dataclasses import dataclass
from heapq import heappush, heappop

@dataclass(eq = True, order = True, frozen = True)
class State:
    pos: tuple[int, int]
    dir: tuple[int, int]
    step_count: int

def parse_input(input: Input) -> list[list[int]]:
    return [[int(n) for n in line] for line in input.lines]

def dirs(dir: tuple[int, int]) -> list[tuple[int, int]]:
    return [dir, (dir[1], -dir[0]), (-dir[1], dir[0])]

def find_path(map: list[list[int]], start: tuple[int, int], target: tuple[int, int], max_straight: int, min_straight: int) -> int:
    width, height = len(map[0]), len(map)
    open = []
    closed = set()
    mv0, mv1 = map[start[1]][start[0] + 1], map[start[1] + 1][start[0]]
    heappush(open, (mv0, State((start[0] + 1, start[1]), (1, 0), 1)))
    heappush(open, (mv1, State((start[0], start[1] + 1), (0, 1), 1)))
    while len(open) > 0:
        depth, curr = heappop(open)
        if curr.pos == target and curr.step_count >= min_straight:
            return depth
        
        if curr in closed:
            continue
        closed.add(curr)

        for dir in dirs(curr.dir):
            p = (curr.pos[0] + dir[0], curr.pos[1] + dir[1])
            if p[0] >= 0 and p[0] < width and p[1] >= 0 and p[1] < height:
                if dir == curr.dir and curr.step_count >= max_straight:
                    continue
                if dir != curr.dir and curr.step_count < min_straight:
                    continue
                heappush(open, (depth + map[p[1]][p[0]], State(p, dir, curr.step_count + 1 if dir == curr.dir else 1)))
    return 0

def solve_first(input: Input) -> object:
    map = parse_input(input)
    return find_path(map, (0, 0), (len(map[0]) - 1, len(map) - 1), 3, 0)

def solve_second(input: Input) -> object:
    map = parse_input(input)
    return find_path(map, (0, 0), (len(map[0]) - 1, len(map) - 1), 10, 4)
