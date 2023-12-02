from input import Input
from typing import NamedTuple
from math import prod

class Round(NamedTuple):
    red: int
    green: int
    blue: int        

def cube_amount(cubes: list[list[str, str]], color: str) -> int:
    return next((int(x[0]) for x in cubes if x[1] == color), 0)

def parse_input(input: Input) -> list[list[Round]]:
    games = []
    for line in input.lines:
        line = line[(line.find(":") + 2):]
        rounds = [[cube.strip().split(" ") for cube in grab.split(",")] for grab in line.split(";")]
        games.append([Round(cube_amount(round, "red"), cube_amount(round, "green"), cube_amount(round, "blue")) for round in rounds])
    return games

def solve_first(input: Input) -> object:
    limit = Round(12, 13, 14)
    games = parse_input(input)
    sum = 0
    for i, game in enumerate(games):
        if all((round.red <= limit.red and round.green <= limit.green and round.blue <= limit.blue) for round in game):
            sum += (i + 1)
    return sum

def solve_second(input: Input) -> object:
    games = parse_input(input)
    sum = 0
    for game in games:
        sum += prod(max(round[i] for round in game) for i in range(0, 3))
    return sum
