from input import Input

def parse_input(input: Input) -> list[int, list[list[int]]]:
    cards = []
    for line in input.lines:
        line = line[(line.find(":") + 1):]
        card = [[int(num) for num in part.split()] for part in line.split(" | ")]
        cards.append(card)
    return cards

def count_matching(card: list[list[int]]) -> int:
    return len(set(card[0]).intersection(set(card[1])))

def solve_first(input: Input) -> object:
    cards = parse_input(input)
    sum = 0
    for card in cards:
        if matching := count_matching(card):
            sum += 2 ** (matching - 1)
    return sum

def solve_second(input: Input) -> object:
    cards = parse_input(input)
    card_len = len(cards)
    card_amounts = [1 for _ in range(card_len)]
    for i in range(card_len):
        if matching := count_matching(cards[i]):
            amount = card_amounts[i]
            for k in range(i + 1, min(i + 1 + matching, card_len)):
                card_amounts[k] += amount
    return sum(card_amounts)
