from input import Input
from typing import NamedTuple
from itertools import batched
import re

class Mapping(NamedTuple):
    dest_start: int
    source_start: int
    size: int

def parse_input(input: Input) -> tuple[list[int], dict[str, tuple[str, list[Mapping]]]]:
    seeds = [int(seed) for seed in input.lines[0][6:].split()]
    m = dict()
    mappings = re.split(r"\r?\n\r?\n", input.raw)[1:]
    for map in mappings:
        lines = map.splitlines()
        types = re.findall(r"(.+)-to-(.+) map:", lines[0])
        maps = [Mapping(int(p[0]), int(p[1]), int(p[2])) for p in [line.split() for line in lines[1:]]]
        m[types[0][0]] = (types[0][1], maps)
    return (seeds, m)

def calc_location(mapping: dict[str, tuple[str, list[Mapping]]], value: int) -> int:
    curr = "seed"
    while curr in mapping:
        target, translations = mapping[curr]
        curr = target
        translation = next((t for t in translations if t.source_start <= value and value < t.source_start + t.size), None)
        if translation:
            value = translation.dest_start + (value - translation.source_start)
    return value

def split_ranges(mapping: Mapping, ranges: list[tuple[int, int]]) -> tuple[list[tuple[int, int]], list[tuple[int, int]]]:
    mid_ranges = []
    outer_ranges = []
    for r in ranges:
        source_range = (mapping.source_start, mapping.source_start + mapping.size)
        r_low = (r[0], min(source_range[0], r[1]))
        r_mid = (max(r[0], source_range[0]), min(r[1], source_range[1]))
        r_high = (max(source_range[1], r[0]), r[1])
        
        if r_low[0] < r_low[1]:
            outer_ranges.extend([r_low])
        if r_high[0] < r_high[1]:
            outer_ranges.extend([r_high])

        if r_mid[0] < r_mid[1]:
            dest_off = mapping.dest_start - mapping.source_start
            mid_ranges.extend([(r_mid[0] + dest_off, r_mid[1] + dest_off)])
    return (outer_ranges, mid_ranges)

def solve_first(input: Input) -> object:
    seeds, mapping = parse_input(input)
    return min(calc_location(mapping, seed) for seed in seeds)

def solve_second(input: Input) -> object:
    seeds, mappings = parse_input(input)
    ranges = [(batch[0], batch[0] + batch[1]) for batch in batched(seeds, 2)]
    for _, mapping in mappings.values():
        open = ranges
        ranges = []
        for map in mapping:
            open, completed = split_ranges(map, open)
            ranges.extend(completed)
        ranges.extend(open)
    return min(r[0] for r in ranges)
