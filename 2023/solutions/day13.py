from input import Input
from itertools import groupby

def parse_input(input: Input) -> list[list[list[str]]]:
    return [list(list(g) for g in group) for match, group in groupby(input.lines, lambda line: line.strip() != "") if match]

def find_refl(pattern: list[str], ignored: int = -1) -> int:
    for i in range(1, len(pattern)):
        if i != ignored:
            if all([pattern[i + k] == pattern[i - k - 1] for k in range(min(i, len(pattern) - i))]):
                return i
    return 0

def calc_refl(pattern: list[str], old_v: int = -1, old_h: int = -1) -> tuple[int, int, int]:
    v = find_refl(pattern, old_v)
    pattern = list(zip(*pattern))
    h = find_refl(pattern, old_h)
    return v * 100 + h, v, h

def calc_refl_w_smudge(pattern: list[str]) -> int:
    _, old_v, old_h = calc_refl(pattern)
    for i in range(len(pattern)):
        for k, curr in enumerate(pattern[i]):
            pattern[i][k] = "#" if curr == "." else "."
            value, _, _ = calc_refl(pattern, old_v, old_h)
            pattern[i][k] = curr
            if value != 0:
                return value
    return 0

def solve_first(input: Input) -> object:
    patterns = parse_input(input)
    return sum(calc_refl(pattern)[0] for pattern in patterns)

def solve_second(input: Input) -> object:
    patterns = parse_input(input)
    return sum(calc_refl_w_smudge(pattern) for pattern in patterns)
