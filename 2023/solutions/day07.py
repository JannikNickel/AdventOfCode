from input import Input
from collections import Counter
from collections.abc import Generator
from functools import cmp_to_key

SORTED_VALUES = {value: index for index, value in enumerate(["A", "K", "Q", "J", "T", "9", "8", "7", "6", "5", "4", "3", "2"])}
SORTED_VALUES_JOKER = {value: index for index, value in enumerate(["A", "K", "Q", "T", "9", "8", "7", "6", "5", "4", "3", "2", "J"])}

def parse_input(input: Input) -> list[tuple[str, int]]:
    return [(part[0], int(part[1])) for part in [line.split() for line in input.lines]]

def hand_type(cards: str) -> int:
    counts = sorted(Counter(cards).values(), reverse=True)
    if len(counts) == 1:
        return 6
    if len(counts) == 2 and (counts[0] == 4 or counts[1] == 4):
        return 5
    if len(counts) == 2 and (counts[0] == 3 or counts[1] == 3):
        return 4
    if len(counts) == 3 and (counts[0] == 3 or counts[1] == 3 or counts[2] == 3):
        return 3
    if len(counts) == 3 and (counts[0] == 2 or counts[1] == 2 or counts[2] == 2):
        return 2
    if len(counts) == 4:
        return 1
    return 0

def cmp_card_by_card(card_values: dict[str, int], a: str, b: str) -> int:
    for i in range(5):
        if (at := card_values[a[i]]) != (bt := card_values[b[i]]):
            return 1 if at < bt else -1
    return 0

def cmp_hands(card_values: dict[str, int], a: str, b: str) -> int:
    if (at := hand_type(a)) != (bt := hand_type(b)):
        return 1 if at > bt else -1
    return cmp_card_by_card(card_values, a, b)

def generate_perms(cards: str) -> Generator[str, None, None]:
    possible = [c for c in cards if c != "J"]
    for c0 in possible if cards[0] == "J" else [cards[0]]:
        for c1 in possible if cards[1] == "J" else [cards[1]]:
            for c2 in possible if cards[2] == "J" else [cards[2]]:
                for c3 in possible if cards[3] == "J" else [cards[3]]:
                    for c4 in possible if cards[4] == "J" else [cards[4]]:
                        yield f"{c0}{c1}{c2}{c3}{c4}"

def find_best_type(cards: str) -> str:
    best = hand_type(cards)
    for perm in generate_perms(cards):
        if (val := hand_type(perm)) > best:
            best = val
    return best

def cmp_hands_joker(card_values: dict[str, int], a: str, b: str) -> int:
    if (at := find_best_type(a)) != (bt := find_best_type(b)):
        return 1 if at > bt else -1
    return cmp_card_by_card(card_values, a, b)

def calc_sum(hands: list[tuple[str, int]]) -> int:
    return sum([(i + 1) * bid for i, (_, bid) in enumerate(hands)])

def solve_first(input: Input) -> object:
    hands = parse_input(input)
    sorted_hands = sorted(hands, key=cmp_to_key(lambda a, b: cmp_hands(SORTED_VALUES, a[0], b[0])))
    return calc_sum(sorted_hands)

def solve_second(input: Input) -> object:
    hands = parse_input(input)
    sorted_hands = sorted(hands, key=cmp_to_key(lambda a, b: cmp_hands_joker(SORTED_VALUES_JOKER, a[0], b[0])))
    return calc_sum(sorted_hands)
