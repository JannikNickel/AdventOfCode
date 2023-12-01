from input import Input
import re

def word_to_int(word: str) -> int:
    match word:
        case "one" | "1": return 1
        case "two" | "2": return 2
        case "three" | "3": return 3
        case "four" | "4": return 4
        case "five" | "5": return 5
        case "six" | "6": return 6
        case "seven" | "7": return 7
        case "eight" | "8": return 8
        case "nine" | "9": return 9

def solve_first(input: Input) -> object:
    sum = 0
    for line in input.lines:
        indices = [i for i in range(0, len(line)) if line[i].isdigit()]
        if len(indices) > 0:
            sum += 10 * int(line[indices[0]]) + int(line[indices[-1]])
    return sum

def solve_second(input: Input) -> object:
    sum = 0
    for line in input.lines:
        matches = re.findall("(?=(one|two|three|four|five|six|seven|eight|nine|1|2|3|4|5|6|7|8|9))", line)
        if len(matches) > 0:
            sum += 10 * word_to_int(matches[0]) + word_to_int(matches[-1])
    return sum
