from input import Input
from dataclasses import dataclass
from itertools import combinations
from fractions import Fraction

@dataclass(eq = True)
class Vec3:
    x: int | float
    y: int | float
    z: int | float

    def __iter__(self):
        yield self.x
        yield self.y
        yield self.z

    def __add__(self, other):
        return Vec3(self.x + other.x, self.y + other.y, self.z + other.z)
    
    def __sub__(self, other):
        return Vec3(self.x - other.x, self.y - other.y, self.z - other.z)
    
    def __mul__(self, other):
        return Vec3(self.x * other, self.y * other, self.z * other)
    
    def dot2d(lhs, rhs):
        return lhs.x * rhs.x + lhs.y * rhs.y
    
    def cross(a, b):
        return Vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x)

def parse_input(input: Input) -> list[tuple[Vec3, Vec3]]:
    return [tuple(Vec3(*[int(n) for n in part.split(",")]) for part in line.split("@")) for line in input.lines]

def det(a: Vec3, b: Vec3) -> int | float:
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

def linearly_independent(v0: Vec3, v1: Vec3) -> bool:
    if all(n == 0 for n in v0) or all(n == 0 for n in v1):
        return False
    ratios = []
    for a, b in zip(v0, v1):
        if b == 0:
            if a != 0:
                return True
            continue
        ratios.append(a / b)
    return not all(r == ratios[0] for r in ratios[1:])

def skew_sym_matrix(v: Vec3):
    return [
        [0, -v.z, v.y],
        [v.z, 0, -v.x],
        [-v.y, v.x, 0]
    ]

def sub_matrix3x3(a: list[list[int]], b: list[list[int]]) -> list[list[int]]:
    return [[a[i][k] - b[i][k] for k in range(3)] for i in range(3)]

def partial_pivot(A: list[list[Fraction]], b: list[Fraction], n: int):
    for i in range(n):
        max = abs(A[i][i])
        max_row = i
        for k in range(i + 1, n):
            if abs(A[k][i]) > max:
                max = abs(A[k][i])
                max_row = k
        A[i], A[max_row] = A[max_row], A[i]
        b[i], b[max_row] = b[max_row], b[i]

def solve_linear(A: list[list[Fraction]], b: list[Fraction]) -> list[Fraction]:
    n = len(A)
    for i in range(n):
        partial_pivot(A, b, n)
        for k in range(i + 1, n):
            c = -A[k][i] / A[i][i]
            for l in range(i, n):
                A[k][l] += c * A[i][l]
            b[k] += c * b[i]
            
    x = [0 for _ in range(n)]
    for i in range(n - 1, -1, -1):
        x[i] = b[i] / A[i][i]
        for k in range(i - 1, -1, -1):
            b[k] -= A[k][i] * x[i]
    return x

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
    # Find 3 linearly independent hailstones
    selected_hails = [hails[0]]
    for h in hails:
        if len(selected_hails) < 3 and not any(not linearly_independent(h[1], n[1]) for n in selected_hails):
            selected_hails.append(h)

    # => p0, v0 = thrown rock start parameters, p_i, v_i = i-th hailstone parameters, t_i = intersection time with i-th hailstone
    # p0 + t_i * v0 == p_i + t_i * v_i

    # => Rearrange to bring pos, vel to one side
    # t_i * v0 - t_i * v_i == p_i - p0  

    # => Factor out t_i
    # t_i * (v0 - v_i) == p_i - p0

    # => Cross product both of both sides (cross product of a vector with itself is zero)
    # (v0 - v_i) x (t_i * (v0 - v_i)) == (v0 - v_i) x (p_i - p0)
    
    # => Simplify (scalar multiplication in cross product)
    # => k * (a x b) == (k * a) x b = a x (k * b)
    # t_i * ((v0 - v_i) x (v0 - v_i)) == (v0 - v_i) x (p_i - p0)

    # => Cross product of a vector with itself is zero
    # t_i * 0 == (v0 - v_i) x (p_i - p0)
    # 0 == (v0 - v_i) x (p_i - p0)

    # => Cross product is linear, choose 3 different hailstones to form enough linear equations
    # (p0 - p_i) x (v0 - v_i) == 0

    p0, p1, p2 = selected_hails[0][0], selected_hails[1][0], selected_hails[2][0]
    v0, v1, v2 = selected_hails[0][1], selected_hails[1][1], selected_hails[2][1]
    b0 = Vec3.cross(p1, v1) - Vec3.cross(p0, v0)
    b1 = Vec3.cross(p2, v2) - Vec3.cross(p0, v0)
    b = [*b0, *b1]

    # Ab = a x b (for A = skew_sym_matrix)
    a0 = sub_matrix3x3(skew_sym_matrix(v0), skew_sym_matrix(v1))
    a1 = sub_matrix3x3(skew_sym_matrix(v0), skew_sym_matrix(v2))
    a2 = sub_matrix3x3(skew_sym_matrix(p0), skew_sym_matrix(p1))
    a3 = sub_matrix3x3(skew_sym_matrix(p0), skew_sym_matrix(p2))
    A = [[0] * 6 for _ in range(6)]
    for i in range(3):
        A[i][:3] = a0[i]
        A[i][3:] = a2[i]
        A[i + 3][:3] = a1[i]
        A[i + 3][3:] = a3[i]
        
    A = [[Fraction(v) for v in row] for row in A]
    b = [Fraction(v) for v in b]
    res = solve_linear(A, b)
    return res[0] + res[1] + res[2]
