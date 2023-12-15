from input import Input

def parse_input(input: Input) -> list[tuple]:
    return [tuple(part.split("=")) if "=" in part else (part[:-1],) for part in input.raw.split(",")]

def calc_hash(string: str) -> int:
    h = 0
    for char in string:
        h = (h + ord(char)) * 17 % 256
    return h

def solve_first(input: Input) -> object:
    strings = input.raw.split(",")
    return sum(calc_hash(s) for s in strings)

def solve_second(input: Input) -> object:
    ops = parse_input(input)
    boxes = [[] for _ in range(256)]
    for op in ops:
        label = op[0]
        box = boxes[calc_hash(label)]
        entry = next((pair for pair in box if pair[0] == label), None)
        if len(op) == 2:
            if entry:
                box[box.index(entry)] = op
            else:
                box.append(op)
        elif entry:
            box.remove(entry)
    return sum((i + 1) * (k + 1) * int(focal[1]) for (i, box) in enumerate(boxes) for (k, focal) in enumerate(box))
