from input import Input
from dataclasses import dataclass
from collections import defaultdict
from collections.abc import Generator

@dataclass(eq = True, unsafe_hash = True)
class Vec3():
    x: float
    y: float
    z: float

@dataclass(eq = True, unsafe_hash = True)
class Bounds():
    min: Vec3
    max: Vec3

    def contains(self, p: Vec3) -> bool:
        return self.min.x <= p.x and p.x <= self.max.x and self.min.y <= p.y and p.y <= self.max.y and self.min.z <= p.z and p.z <= self.max.z
    
    def points(self) -> Generator[Vec3, None, None]:
        for x in range(self.min.x, self.max.x + 1):
            for y in range(self.min.y, self.max.y + 1):
                for z in range(self.min.z, self.max.z + 1):
                    yield Vec3(x, y, z)

def parse_input(input: Input) -> list[Bounds]:
    return [Bounds(Vec3(*[int(n) for n in l.split(",")]), Vec3(*[int(n) for n in r.split(",")])) for l, r in (line.split("~") for line in input.lines)]

def search_area(settled: dict[Vec3, Bounds], b: Bounds, z_off: int) -> set[Bounds]:
    res = set()
    for x in range(b.min.x, b.max.x + 1):
        for y in range(b.min.y, b.max.y + 1):
            p = Vec3(x, y, b.min.z + z_off)
            if below := settled.get(p, None):
                res.add(below)
    return res

def simulate_fall(bricks: list[Bounds]) -> tuple[dict[Bounds, set[Bounds]], dict[Bounds, set[Bounds]]]:
    bricks = sorted(bricks, key = lambda b: b.min.z)
    upwards = defaultdict(set)
    downwards = defaultdict(set)
    settled = dict()
    for b in bricks:
        while b.min.z > 1:
            below = search_area(settled, b, -1)
            if len(below) > 0:
                for be in below:
                    upwards[be].add(b)
                downwards[b] |= below
                break
            b.min.z -= 1
            b.max.z -= 1
        for pos in b.points():
            settled[pos] = b
    return upwards, downwards

def find_disconnected(up, down, closed, brick):
    sum = 1
    for u in up[brick]:
        if u not in closed and all(d in closed for d in down[u]):
            closed.add(u)
            sum += find_disconnected(up, down, closed, u)
    return sum

def solve_first(input: Input) -> object:
    bricks = parse_input(input)
    up, down = simulate_fall(bricks)
    return sum(1 for b in bricks if all(len(down[u]) != 1 for u in up[b]))

def solve_second(input: Input) -> object:
    bricks = parse_input(input)
    up, down = simulate_fall(bricks)
    sum = 0
    for b in bricks:
        closed = set([b])
        for u in (n for n in up[b] if len(down[n]) == 1):
            closed.add(u)
            sum += find_disconnected(up, down, closed, u)
    return sum
