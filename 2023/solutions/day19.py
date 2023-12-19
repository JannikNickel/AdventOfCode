from input import Input
from dataclasses import dataclass
from math import prod
import re

Mapping = tuple[str, str, int, str]

@dataclass
class Part():
    x: int
    m: int
    a: int
    s: int

def parse_mapping(mapping: str) -> list[Mapping]:
    parts = [(p[0], p[1], *(p[2:].split(":"))) if ":" in p else (p,) for p in mapping[0:-1].split(",")]
    return [(p[0], p[1], int(p[2]), p[3]) if len(p) > 1 else p for p in parts]

def parse_input(input: Input) -> tuple[dict[str, list[Mapping]], list[Part]]:
    split = input.lines.index("")
    maps = dict()
    for line in input.lines[:split]:
        l, r = line.split("{")
        maps[l] = parse_mapping(r)
    part_lines = input.lines[split + 1:]
    parts = [Part(*[int(n) for n in re.findall(r"\d+", line)]) for line in part_lines]
    return (maps, parts)

def is_accepted(mapping: Mapping, part: Part) -> bool:
    curr = "in"
    while curr:
        if curr == "A" or curr == "R":
            return curr == "A"
        for rule in mapping[curr]:
            if len(rule) == 1 or (rule[1] == "<" and getattr(part, rule[0]) < rule[2]) or (rule[1] == ">" and getattr(part, rule[0]) > rule[2]):
                curr = rule[0 if len(rule) == 1 else 3]
                break
    return False

def solve_ranged(mapping: Mapping) -> int:
    open = [("in", [(1, 4000)] * 4)]
    total = 0
    while len(open) > 0:
        curr, ranges = open.pop()
        if curr == "A" or curr == "R":
            total += prod(r[1] - r[0] + 1 for r in ranges) if curr == "A" else 0
            continue
        
        for rule in mapping[curr][:-1]:
            c, op, num, next = rule
            i = "xmas".index(c)
            next_range = ranges.copy()
            r = ranges[i]
            if r[0] < num and num < r[1]:
                next_range[i] = (r[0], num - 1) if op == "<" else (num + 1, r[1])
                ranges[i] = (num, r[1]) if op == "<" else (r[0], num)
                open.append((next, next_range))
        open.append((mapping[curr][-1][0], ranges))

    return total

def solve_first(input: Input) -> object:
    mapping, parts = parse_input(input)
    return sum(p.x + p.m + p.a + p.s for p in parts if is_accepted(mapping, p))

def solve_second(input: Input) -> object:
    mapping, _ = parse_input(input)
    return solve_ranged(mapping)
