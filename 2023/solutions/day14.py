from input import Input

def parse_input(input: Input) -> list[list[str]]:
    return [list(line) for line in input.lines]

def shift_north(platform: list[list[str]]) -> list[list[str]]:
    platform = [row[:] for row in platform]
    for x in range(len(platform[0])):
        for y in range(len(platform)):
            if platform[y][x] == "O":
                for l in range(y - 1, -2, -1):
                    if l == -1 or platform[l][x] != ".":
                        if l + 1 != y:
                            platform[y][x] = "."
                            platform[l + 1][x] = "O"
                        break
    return platform

def calc_north_load(platform: list[list[str]]) -> int:
    height = len(platform)
    sum = 0
    for i, row in enumerate(platform):
        for _, s in enumerate(row):
            sum += height - i if s == "O" else 0
    return sum

def rotate(platform: list[list[str]]) -> list[list[str]]:
    rotated = [list(row) for row in zip(*reversed(platform))]
    return rotated

def cycle(platform: list[list[str]]) -> list[list[str]]:
    for _ in range(4):
        platform = shift_north(platform)
        platform = rotate(platform)
    return platform

def platform_str(platform: list[list[str]]) -> str:
    return "".join("".join(row) for row in platform)

def solve_first(input: Input) -> object:
    platform = parse_input(input)
    platform = shift_north(platform)
    return calc_north_load(platform)

def solve_second(input: Input) -> object:
    platform = parse_input(input)
    states = dict()
    cycles = 1000000000
    i = 0
    while i < cycles:
        platform = cycle(platform)
        h = platform_str(platform)
        if h in states:
            off = i - states[h]
            remaining = cycles - i
            i += remaining // off * off
            states.clear()
        states[h] = i
        i += 1
    return calc_north_load(platform)
