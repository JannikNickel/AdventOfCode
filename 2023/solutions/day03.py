from input import Input
from math import prod
from collections import defaultdict
from collections.abc import Generator
from typing import Callable

def find_adjacent_symbols(schematic: list[list[str]], row: int, c_from: int, c_to: int, predicate: Callable[[str], bool]) -> list[tuple[int, int]]:
    res = []
    for x in range(c_from - 1, c_to + 1):
        for y in range(row - 1, row + 2):
            if x >= 0 and x < len(schematic[0]) and y >= 0 and y < len(schematic):
                if not schematic[y][x].isdigit() and predicate(schematic[y][x]):
                    res.append((x, y))
    return res

def has_adjacent_symbol(schematic: list[list[str]], row: int, c_from: int, c_to: int) -> bool:
    return len(find_adjacent_symbols(schematic, row, c_from, c_to, lambda char: char != '.')) > 0

def iter_nums(schematic: list[list[str]]) -> Generator[tuple[int, int, int, int], None, None]:
    for ri, row in enumerate(schematic):
        ci = 0
        while ci < len(row):
            start = ci
            while ci < len(row) and row[ci].isdigit():
                ci += 1
            if start != ci:
                yield (int(row[start:ci]), ri, start, ci)
            ci += 1

def solve_first(input: Input) -> object:
    sum = 0
    for num, row, c_start, c_end in iter_nums(input.lines):
        if has_adjacent_symbol(input.lines, row, c_start, c_end):
            sum += num
    return sum

def solve_second(input: Input) -> object:
    res = defaultdict(list)
    for num, row, c_start, c_end in iter_nums(input.lines):
        positions = find_adjacent_symbols(input.lines, row, c_start, c_end, lambda char: char == "*")
        for pos in positions:
            res[pos].append(num)
    return sum([prod(value) for value in res.values() if len(value) == 2])
