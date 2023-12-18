import sys
import time
import importlib
from types import ModuleType
from input import *

ANSI_ESC_INFO = "\x1B[90m"
ANSI_ESC_CLOSE = "\033[0m"

INPUT_PATH = "input"
PUZZLES = [
    "Trebuchet?!",
    "Cube Conundrum",
    "Gear Ratios",
    "Scratchcards",
    "If You Give A Seed A Fertilizer",
    "Wait For It",
    "Camel Cards",
    "Haunted Wasteland",
    "Mirage Maintenance",
    "Pipe Maze",
    "Cosmic Expansion",
    "Hot Springs",
    "Point of Incidence",
    "Parabolic Reflector Dish",
    "Lens Library",
    "The Floor Will Be Lava",
    "Clumsy Crucible",
    "Lavaduct Lagoon"
]

def main():
    argv = sys.argv[1:]
    argc = len(argv)
    solutions = None
    if argc > 0:
        if argv[0] == "--day" and argc > 1:
            try:
                day = int(argv[1])
                if day >= 1 and day <= len(PUZZLES):
                    solutions = [day]
            except ValueError:
                pass
        elif argv[0] == "--all":
            solutions = [*range(1, len(PUZZLES) + 1)]

    if solutions == None:
        print("Fallback to last solution!")
        solutions = [len(PUZZLES)]

    for solution in solutions:
        solve_day(solution)

def load_input(day: int) -> Input:
    return Input(f"{INPUT_PATH}/{day:02}.txt")

def solve_day(day: int):
    print(f"{ANSI_ESC_INFO}Preparing solution {day:02} - {PUZZLES[day - 1]}{ANSI_ESC_CLOSE}")
    input = load_input(day)
    module = importlib.import_module(f"solutions.day{day:02}")
    solve_part(module, input, 1)
    solve_part(module, input, 2)

def solve_part(module: ModuleType, input: Input, part: int):
    print(f"{ANSI_ESC_INFO}Solving part {part}...{ANSI_ESC_CLOSE}")
    func = getattr(module, "solve_first" if part == 1 else "solve_second")
    t_start = time.time_ns()
    result = func(input)
    t_end = time.time_ns()
    print(f"{ANSI_ESC_INFO}Solved part {part} ({((t_end - t_start) * 1e-6):.2f}ms)")
    print(f"{ANSI_ESC_INFO}Result:{ANSI_ESC_CLOSE}\n{result}\n")

if __name__ == "__main__":
    main()
