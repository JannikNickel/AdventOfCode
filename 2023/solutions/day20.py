from input import Input
from enum import IntEnum
from collections import deque
from math import lcm

class Pulse(IntEnum):
    LOW = 0
    HIGH = 1

class Component:
    def __init__(self, outputs: list[str]):
        self._outputs = outputs
        self.last_out = None

    @property
    def outputs(self) -> list[str]:
        return self._outputs

    def process(self, signal: Pulse, source: str) -> Pulse | None:
        return signal

class FlipFlop(Component):
    def __init__(self, outputs: list[str]):
        super().__init__(outputs)
        self.state = False

    def process(self, signal: Pulse, source: str) -> Pulse | None:
        if signal == Pulse.HIGH:
            return None
        self.state = not self.state
        return Pulse.HIGH if self.state else Pulse.LOW

class Conjunction(Component):
    def __init__(self, outputs: list[str], inputs: list[str]):
        super().__init__(outputs)
        self._inputs = inputs
        self._memory = {inp: Pulse.LOW for inp in inputs}

    @property
    def inputs(self) -> list[str]:
        return self._inputs

    def process(self, signal: Pulse, source: str) -> Pulse | None:
        self._memory[source] = signal
        return Pulse.LOW if all(p == Pulse.HIGH for p in self._memory.values()) else Pulse.HIGH

def parse_input(input: Input) -> dict[str, Component]:
    map = [(lr[0], [l.strip() for l in lr[1].split(",")]) for lr in (p.split(" -> ") for p in input.lines)]
    components = dict()
    for m in map:
        match m[0][0]:
            case "%":
                components[m[0][1:]] = FlipFlop(m[1])
            case "&":
                components[m[0][1:]] = Conjunction(m[1], [l.lstrip("%&") for l, r in map if m[0][1:] in r])
            case _:
                components[m[0]] = Component(m[1])
    return components

def simulate_network(components: dict[str, Component]) -> tuple[int, int]:
    sig_count = [0, 0]
    open = deque([("button", "broadcaster", Pulse.LOW)])
    while len(open) > 0:
        prev, curr, signal = open.popleft()
        sig_count[signal] += 1
        if curr not in components:
            continue
        comp = components[curr]
        new_signal = comp.process(signal, prev)
        if new_signal == Pulse.HIGH:
            comp.last_out = new_signal
        if new_signal != None:
            for out in comp.outputs:
                open.append((curr, out, new_signal))
    return tuple(sig_count)

def solve_first(input: Input) -> object:
    components = parse_input(input)
    sl, sh = tuple(sum(values) for values in zip(*[simulate_network(components) for _ in range(1000)]))
    return sl * sh

def solve_second(input: Input) -> object:
    # Not sure if this works for all inputs, but in my case there is a single conjunction that connects to rx
    # => for rx to receive a low pulse, all inputs of the single conjunction node have to be a high pulse
    # Cycle detection for each input node of the conjuction works with lcm because the cycles lengths are prime
    components = parse_input(input)
    rx_target = next(key for key, value in components.items() if "rx" in value.outputs)
    conj_targets = components[rx_target].inputs
    cycles = [0 for _ in conj_targets]
    i = 0
    while True:
        _ = simulate_network(components)
        i += 1
        for k, conj in enumerate(conj_targets):
            if cycles[k] == 0 and components[conj].last_out == Pulse.HIGH:
                cycles[k] = i
        if all(c != 0 for c in cycles):
            break
    return lcm(*cycles)
