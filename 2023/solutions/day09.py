from input import Input

def parse_input(input: Input) -> list[list[int]]:
    return [[int(num) for num in line.split()] for line in input.lines]

def extrapolate(history: list[int], dir: int) -> int:
    diffs = [history]
    while any(d != 0 for d in diffs[-1]):
        diffs.append([diffs[-1][i + 1] - diffs[-1][i] for i in range(len(diffs[-1]) - 1)])

    idx = -1 if dir == 1 else 0
    diffs[-1].append(0)
    for i in range(len(diffs) - 2, -1, -1):
        diffs[i].insert(idx % (len(diffs[i]) + 1), diffs[i][idx] + (diffs[i + 1][idx] * dir))
    return diffs[0][idx]

def solve_first(input: Input) -> object:
    histories = parse_input(input)
    return sum(extrapolate(h, 1) for h in histories)

def solve_second(input: Input) -> object:
    histories = parse_input(input)
    return sum(extrapolate(h, -1) for h in histories)
