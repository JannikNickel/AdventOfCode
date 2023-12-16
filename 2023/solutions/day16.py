from input import Input

def next_dir(char: str, dir: tuple[int, int]) -> list[tuple[int, int]]:
    horizontal = dir[0] != 0
    if char == "|" and horizontal:
        return [(0, 1), (0, -1)]
    if char == "-" and not horizontal:
        return [(1, 0), (-1, 0)]
    if char == "/":
        return [(-dir[1], -dir[0])]
    if char == "\\":
        return [(dir[1], dir[0])]
    return [dir]

def traverse_map(map: list[str], pos: tuple[int, int], dir: tuple[int, int], closed: set[tuple[tuple[int, int], tuple[int, int]]] = None) -> set[tuple[int, int]]:
    visited = set()
    closed = closed or set()
    while (pos, dir) not in closed:
        closed.add((pos, dir))
        pos = (pos[0] + dir[0], pos[1] + dir[1])
        if pos[0] < 0 or pos[0] >= len(map[0]) or pos[1] < 0 or pos[1] >= len(map):
            break
        visited.add(pos)

        next = next_dir(map[pos[1]][pos[0]], dir)
        dir = next[0]
        for nd in next[1:]:
            visited |= traverse_map(map, pos, nd, closed)
    return visited

def solve_first(input: Input) -> object:
    map = input.lines
    res = traverse_map(map, (-1, 0), (1, 0))
    return len(res)

def solve_second(input: Input) -> object:
    map = input.lines
    width, height = len(map[0]), len(map)
    starts = []
    starts += [((i, -1), (0, 1)) for i in range(width)]
    starts += [((i, width), (0, -1)) for i in range(width)]
    starts += [((-1, i), (1, 0)) for i in range(height)]
    starts += [((width, i), (-1, 0)) for i in range(height)]
    return max(len(traverse_map(map, start, dir)) for start, dir in starts)
