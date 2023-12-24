from input import Input
from dataclasses import dataclass
from itertools import combinations
from sympy import Symbol, nonlinsolve

@dataclass(eq = True)
class Vec3:
    x: float
    y: float
    z: float

    def __getitem__(self, index):
        match index:
            case 0: return self.x
            case 1: return self.y
            case 2: return self.z

    def __add__(self, other):
        return Vec3(self.x + other.x, self.y + other.y, self.z + other.z)
    
    def __sub__(self, other):
        return Vec3(self.x - other.x, self.y - other.y, self.z - other.z)
    
    def __mul__(self, other):
        return Vec3(self.x * other, self.y * other, self.z * other)
    
    def dot2d(lhs, rhs):
        return lhs.x * rhs.x + lhs.y * rhs.y

def parse_input(input: Input) -> list[tuple[Vec3, Vec3]]:
    return [tuple(Vec3(*[int(n) for n in part.split(",")]) for part in line.split("@")) for line in input.lines]

def det(a, b):
    return a.x * b.y - a.y * b.x

def line_intersection(start0, dir0, start1, dir1) -> Vec3 | None:
    end1 = start0 + dir0
    end2 = start1 + dir1
    xdiff = Vec3(start0.x - end1.x, (start1.x - end2.x), 0)
    ydiff = Vec3(start0.y - end1.y, (start1.y - end2.y), 0)
    
    div = det(xdiff, ydiff)
    if div == 0:
        return None
    
    d = Vec3(det(start0, end1), det(start1, end2), 0)
    x = det(d, xdiff) / div
    y = det(d, ydiff) / div
    intersection = Vec3(x, y, 0)

    if Vec3.dot2d(intersection - start0, dir0) < 0 or Vec3.dot2d(intersection - start1, dir1) < 0:
        return None    
    return intersection

def solve_first(input: Input) -> object:
    hails = parse_input(input)
    area = (200000000000000, 400000000000000)
    sum = 0
    for h0, h1 in combinations(hails, 2):
        if i := line_intersection(*h0, *h1):
            if i.x >= area[0] and i.x <= area[1] and i.y >= area[0] and i.y <= area[1]:
                sum += 1
    return sum

def solve_second(input: Input) -> object:
    hails = parse_input(input)

    # Find 3 hails with different velocity to solve
    selected_hails = [hails[0]]
    for h in hails:
        if len(selected_hails) < 3 and not any(n for n in selected_hails if n[1] == h[1]):
            selected_hails.append(h)

    # Solve nonlinear system where the thrown rock collides with each of the other 3 rocks at a (different) time
    # start_pos + start_vel * t[n] == hail_pos + hail_vel * t[n]    (n = 0, 1, 2)
    start_pos = [Symbol("s_x"), Symbol("s_y"), Symbol("s_z")]
    start_vel = [Symbol("vel_x"), Symbol("vel_y"), Symbol("vel_z")]
    times = [Symbol("t0"), Symbol("t1"), Symbol("t2")]
    equations = []
    for i, h in enumerate(selected_hails):
        # pos.x + vel.x * t == hail.x + hail_vel.x * t   =>   0 = pos.x + vel.x * t - (hail.x + hail_vel.x * t)
        # pos.y + vel.y * t == hail.y + hail_vel.y * t   =>   0 = pos.y + vel.y * t - (hail.y + hail_vel.y * t)
        # pos.z + vel.z * t == hail.z + hail_vel.z * t   =>   0 = pos.z + vel.z * t - (hail.z + hail_vel.z * t)
        t = times[i]
        equations.append((start_pos[0] + start_vel[0] * t) - (h[0].x + h[1].x * t))
        equations.append((start_pos[1] + start_vel[1] * t) - (h[0].y + h[1].y * t))
        equations.append((start_pos[2] + start_vel[2] * t) - (h[0].z + h[1].z * t))
    res = tuple(nonlinsolve(equations, [*start_pos, *start_vel, *times]))[0]
    return res[0] + res[1] + res[2]
