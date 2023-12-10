from input import Input

def dirs(curr: str):
    match curr:
        case "|": return [(0, -1), (0, 1)]
        case "-": return [(-1, 0), (1, 0)]
        case "L": return [(0, -1), (1, 0)]
        case "J": return [(0, -1), (-1, 0)]
        case "7": return [(0, 1), (-1, 0)]
        case "F": return [(0, 1), (1, 0)]
        case _: return []

def map_dir(curr: str, dir: tuple[int, int]):
    return next((d for d in dirs(curr) if not (-d[0] == dir[0] and -d[1] == dir[1])), None)

def find_start_pos(map: list[list[str]]) -> tuple[int, int]:
    return next(((col, row) for row, line in enumerate(map) if (col := line.find("S")) != -1), None)

def find_start_dir(map: list[list[str]], start: tuple[int, int]) -> tuple[int, int]:
    for s_off in [(-1, 0), (0, -1), (1, 0), (0, 1)]:
        s = (start[0] + s_off[0], start[1] + s_off[1])
        if s[0] >= 0 and s[0] < len(map[0]) and s[1] >= 0 and s[1] < len(map):
            for d in dirs(map[s[1]][s[0]]):
                if s[0] + d[0] == start[0] and s[1] + d[1] == start[1]:
                    return (-d[0], -d[1])
    return None

def find_circle(map: list[list[str]], start: tuple[int, int]) -> [list[tuple[int, int]], str]:
    path = []
    curr = start
    dir = find_start_dir(map, start)
    s_dir = dir
    while curr[0] != start[0] or curr[1] != start[1] or len(path) == 0:
        curr = (curr[0] + dir[0], curr[1] + dir[1])
        nd = map_dir(map[curr[1]][curr[0]], dir)
        if nd != None:
            dir = nd
        path.append(curr)
    
    s_char = next((c for c in "|-LJ7F" if s_dir in dirs(c) and (-dir[0], -dir[1]) in dirs(c)), None)
    return path, s_char

def solve_first(input: Input) -> object:
    map = input.lines
    start = find_start_pos(map)
    return len(find_circle(map, start)[0]) // 2

def solve_second(input: Input) -> object:
    map = input.lines
    start = find_start_pos(map)
    path, start_char = find_circle(map, start)
    map[start[1]] = map[start[1]][:start[0]] + start_char + map[start[1]][start[0] + 1:]

    enclosed = 0
    inside = False
    enter_dir_up = None
    for row in range(len(map)):
        for col in range(len(map[0])):
            if (col, row) not in path:
                enclosed += inside
                continue

            c = map[row][col]
            if c == "|":
                inside = not inside
            elif c == "L" or c == "F":
                enter_dir_up = c == "L"
            elif c == "J" or c == "7":
                exit_dir_up = c == "J"
                if exit_dir_up != enter_dir_up:
                    inside = not inside
    return enclosed
