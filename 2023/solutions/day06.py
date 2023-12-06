from input import Input
from math import prod

def parse_input(input: Input) -> list[tuple[int, int]]:
    return list(zip([int(n) for n in input.lines[0].split()[1:]], [int(n) for n in input.lines[1].split()[1:]]))

def ways_to_win(time: int, dst: int) -> int:
    sum = 0
    for i in range(1, time):
        sum += (time - i) * i > dst
    return sum

def solve_first(input: Input) -> object:
    pairs = parse_input(input)
    return prod(ways_to_win(time, dst) for time, dst in pairs)

def solve_second(input: Input) -> object:
    pairs = parse_input(input)
    time = int("".join(str(pair[0]) for pair in pairs))
    dst = int("".join(str(pair[1]) for pair in pairs))
    return ways_to_win(time, dst)
