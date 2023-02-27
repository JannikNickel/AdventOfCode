use super::Solution;
use crate::input::Input;
use std::{collections::HashMap, hash::Hash};

pub struct Day24;

impl Solution for Day24 {
    fn day(&self) -> i32 { 24 }
    fn name(&self) -> &'static str { "Arithmetic Logic Unit" }

    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let instructions = parse_input(input);
        let res = alu(&instructions).iter().max_by_key(|n| n.max).unwrap().max;
        Some(Box::new(res))
    }

    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let instructions = parse_input(input);
        let res = alu(&instructions).iter().min_by_key(|n| n.min).unwrap().min;
        Some(Box::new(res))
    }
}

#[derive(Debug, Clone, Copy, Eq)]
struct State {
    regs: [i64; 4],
    min: u64,
    max: u64
}

impl Hash for State {
    fn hash<H: std::hash::Hasher>(&self, state: &mut H) {
        self.regs.hash(state);
    }
}

impl PartialEq for State {
    fn eq(&self, other: &Self) -> bool {
        self.regs == other.regs
    }
}

fn alu(instructions: &Vec<Instruction>) -> Vec<State> {
    let mut states: Vec<State> = vec![State { regs: [0; 4], min: 0, max: 0 }];
    for inst in instructions {
        match inst {
            Instruction::INP(r) => {
                let mut map = HashMap::new();
                for mut state in states {
                    state.regs[r.index()] = 0;
                    let entry = map.entry(state).or_insert(State { regs: state.regs, min: u64::MAX, max: 0 });
                    entry.min = u64::min(entry.min, state.min);
                    entry.max = u64::max(entry.max, state.max);
                }
                let mut next = Vec::with_capacity(map.len() * 9);
                for state in map.into_iter().map(|n| n.1) {
                    for i in 1..=9 {
                        let mut regs = state.regs;
                        regs[r.index()] = i as i64;
                        next.push(State { regs: regs, min: state.min * 10 + i, max: state.max * 10 + i });
                    }
                }
                states = next;
            },
            Instruction::ADD(r0, r1) => {
                states.iter_mut().for_each(|state| state.regs[r0.index()] += r1.value(&state.regs));
            },
            Instruction::MUL(r0, r1) => {
                states.iter_mut().for_each(|state| state.regs[r0.index()] *= r1.value(&state.regs));
            },
            Instruction::DIV(r0, r1) => {
                states.iter_mut().for_each(|state| state.regs[r0.index()] /= r1.value(&state.regs));
            },
            Instruction::MOD(r0, r1) => {
                states.iter_mut().for_each(|state| state.regs[r0.index()] %= r1.value(&state.regs));
            },
            Instruction::EQL(r0, r1) => {
                states.iter_mut().for_each(|state| state.regs[r0.index()] = (state.regs[r0.index()] == r1.value(&state.regs)) as i64);
            }
        }
    }
    states.iter().filter(|n| n.regs[Reg::Z.index()] == 0).map(|n| *n).collect()
}

#[derive(Debug, Clone, Copy)]
enum Reg {
    W, X, Y, Z, Const(i64)
}

impl Reg {
    fn from(value: &str) -> Self {
        match value {
            "w" => Reg::W,
            "x" => Reg::X,
            "y" => Reg::Y,
            "z" => Reg::Z,
            c => Reg::Const(c.parse::<i64>().unwrap())
        }
    }

    fn value(&self, regs: &[i64; 4]) -> i64 {
        match self {
            Reg::Const(c) => *c,
            _ => regs[self.index()]
        }
    }

    fn index(&self) -> usize {
        match self {
            Reg::W => 0,
            Reg::X => 1,
            Reg::Y => 2,
            Reg::Z => 3,
            Reg::Const(_) => panic!()
        }
    }
}

enum Instruction {
    INP(Reg),
    ADD(Reg, Reg),
    MUL(Reg, Reg),
    DIV(Reg, Reg),
    MOD(Reg, Reg),
    EQL(Reg, Reg)
}

fn parse_input(input: &Input) -> Vec<Instruction> {
    let mut res = Vec::new();
    for line in &input.lines {
        let parts = line.split(' ').collect::<Vec<&str>>();
        res.push(match parts[0] {
            "inp" => Instruction::INP(Reg::from(parts[1])),
            "add" => Instruction::ADD(Reg::from(parts[1]), Reg::from(parts[2])),
            "mul" => Instruction::MUL(Reg::from(parts[1]), Reg::from(parts[2])),
            "div" => Instruction::DIV(Reg::from(parts[1]), Reg::from(parts[2])),
            "mod" => Instruction::MOD(Reg::from(parts[1]), Reg::from(parts[2])),
            "eql" => Instruction::EQL(Reg::from(parts[1]), Reg::from(parts[2])),
            _ => panic!()
        });
    }
    res
}
