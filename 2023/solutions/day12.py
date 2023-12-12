from input import Input
from functools import cache

def parse_input(input: Input) -> list[tuple[str, list[int]]]:
    return [(l, [int(n) for n in r.split(",")]) for l, r in [line.split() for line in input.lines]]

@cache
def valid_combinations(springs: str, groups: tuple[int, ...]) -> int:
    springs = springs.lstrip(".")
    if len(groups) == 0:
        return "#" not in springs
    if len(springs) == 0:
        return len(groups) == 0
    
    if springs[0] == "?":
        return valid_combinations("." + springs[1:], groups) + valid_combinations("#" + springs[1:], groups)
    elif springs[0] == "#":
        remaining = groups[0]
        if "." not in springs[:remaining]:
            if remaining < len(springs) and springs[remaining] != "#":
                return valid_combinations(springs[remaining + 1:], groups[1:])
            elif len(springs) == remaining:
                return len(groups) == 1
    return 0

def solve_first(input: Input) -> object:
    rows = parse_input(input)
    return sum(valid_combinations(row[0], tuple(row[1])) for row in rows)

def solve_second(input: Input) -> object:
    rows = parse_input(input)
    rows = [("?".join(springs for _ in range(5)), groups * 5) for springs, groups in rows]
    return sum(valid_combinations(row[0], tuple(row[1])) for row in rows)
