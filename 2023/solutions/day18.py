from input import Input

Plan = list[tuple[str, int, str]]

def parse_input(input: Input) -> Plan:
    return [(p[0], int(p[1]), p[2].lstrip("(#").rstrip(")")) for p in (line.split() for line in input.lines)]

def dir_to_vec(dir: str) -> tuple[int, int]:
    match dir:
        case "U": return (0, -1)
        case "R": return (1, 0)
        case "D": return (0, 1)
        case "L": return (-1, 0)

def trace_poly(plan: Plan) -> list[tuple[int, int]]:
    poly = []
    pos = (0, 0)
    for dir, steps, _ in plan:
        dir_vec = dir_to_vec(dir)
        pos = (pos[0] + dir_vec[0] * steps, pos[1] + dir_vec[1] * steps)
        poly.append(pos)
    return poly

def poly_area_peri(poly: list[tuple[int, int]]) -> tuple[int, int]:
    area = 0
    peri = 0
    for i in range(len(poly)):
        k = (i + 1) % len(poly)
        area += poly[i][0] * poly[k][1] - poly[k][0] * poly[i][1]
        peri += abs(poly[i][0] - poly[k][0]) + abs(poly[i][1] - poly[k][1])
    return abs(area // 2), peri

def calc_area(plan: Plan) -> int:
    poly = trace_poly(plan)
    area, peri = poly_area_peri(poly)
    return area + (peri // 2 + 1)

def solve_first(input: Input) -> object:
    plan = parse_input(input)
    return calc_area(plan)

def solve_second(input: Input) -> object:
    plan = parse_input(input)
    plan = [("RDLU"[int(c[5])], int(c[:5], 16), "") for _, _, c in plan]
    return calc_area(plan)
