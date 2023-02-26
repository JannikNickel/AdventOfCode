use super::Solution;
use crate::input::Input;
use std::hash::{Hash, Hasher};
use std::collections::{BinaryHeap, VecDeque, HashSet};
use std::mem::swap;
use std::str::FromStr;

pub struct Day23;

impl Solution for Day23 {
    fn day(&self) -> i32 { 23 }
    fn name(&self) -> &'static str { "Amphipod" }

    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let initial = parse_input(&input.lines, 2);
        let res = solve(&initial);
        Some(Box::new(res))
    }

    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let mut lines = input.lines.clone();
        lines.insert(3, String::from_str("  #D#C#B#A#").unwrap());
        lines.insert(4, String::from_str("  #D#B#A#C#").unwrap());
        let initial = parse_input(&lines, 4);
        let res = solve(&initial);
        Some(Box::new(res))
    }
}

fn solve(initial: &[VecDeque<Cell>; 4]) -> i32 {
    let room_size = initial[0].len();
    let room_targets = [Cell::A, Cell::B, Cell::C, Cell::D];

    let mut open: BinaryHeap<Burrow> = BinaryHeap::new();
    let mut closed: HashSet<Burrow> = HashSet::new();
    open.push(Burrow::new(initial));

    while let Some(v) = open.pop() {
        if closed.contains(&v) {
            continue;
        }
        closed.insert(v.clone());

        if v.rooms.iter().enumerate().all(|(i, n)| n.len() == room_size && n.iter().all(|n| *n == room_targets[i])) {
            return v.cost;
        }

        'cl: for (i, corridor) in v.corridor.iter().enumerate() {            
            if (*corridor as usize) >= (Cell::E as usize) {
                continue;
            }

            let room_index = *corridor as usize;
            if v.rooms[room_index].iter().any(|n| *n != *corridor) {
                continue;
            }

            let mut from = i;
            let mut to = (room_index + 1) * 2;
            if from < to {
                from += 1;
            } else if from > to {
                from -= 1;
                to += 1;
                swap(&mut from, &mut to);
            }
            for k in from..to {
                let current = v.corridor[k];
                if (current as i32) < (Cell::E as i32) {
                    continue 'cl;
                }
            }

            let mut next = v.clone();
            next.cost += ((room_size - next.rooms[room_index].len()) as i32 + i32::abs(i as i32 - (room_index as i32 + 1) * 2 as i32)) * corridor.cost();
            next.corridor[i] = Cell::E;
            next.rooms[room_index].push_back(*corridor);
            open.push(next);
        }

        for (i, room) in v.rooms.iter().enumerate() {
            if let Some(amphi) = room.iter().last() {
                let ci = i as i32 * 2 + 2;
                open.extend(next_corr_moves(&v, ci, 1, room_size, i, amphi));
                open.extend(next_corr_moves(&v, ci, -1, room_size, i, amphi));
            }
        }
    }

    0
}

fn next_corr_moves(state: &Burrow, start_index: i32, dir: i32, room_size: usize, room_index: usize, amphi: &Cell) -> Vec<Burrow> {
    let mut corr_index = start_index as i32 + dir;
    let mut res = Vec::new();
    loop {
        let corr = state.corridor[corr_index as usize];
        if corr == Cell::E {
            let mut next = state.clone();
            next.cost += ((room_size + 1 - next.rooms[room_index].len()) as i32 + i32::abs(start_index as i32 - corr_index as i32)) * amphi.cost();
            next.rooms[room_index].pop_back();
            next.corridor[corr_index as usize] = *amphi;
            res.push(next);
        }

        corr_index += dir;
        if corr_index < 0 || corr_index >= state.corridor.len() as i32 || (corr as i32) < (Cell::E as i32) {
            break;
        }
    }
    res
}

#[derive(Debug, Clone, Copy, Hash, PartialEq, Eq)]
enum Cell {
    A, B, C, D, E, F
}

impl Cell {
    fn cost(&self) -> i32 {
        match &self {
            Cell::A => 1,
            Cell::B => 10,
            Cell::C => 100,
            Cell::D => 1000,
            _ => 0
        }
    }
}

#[derive(Debug, Eq, Clone)]
struct Burrow {
    cost: i32,
    corridor: [Cell; 11],
    rooms: [VecDeque<Cell>; 4]
}

impl PartialEq for Burrow {
    fn eq(&self, other: &Self) -> bool {
        self.corridor == other.corridor && self.rooms == other.rooms
    }
}

impl Hash for Burrow {
    fn hash<H: Hasher>(&self, state: &mut H) {
        self.corridor.hash(state);
        self.rooms.hash(state);
    }
}

impl PartialOrd for Burrow {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        Some(self.cmp(other))
    }
}

impl Ord for Burrow {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        other.cost.cmp(&self.cost)
    }
}

impl Burrow {
    fn new(initial: &[VecDeque<Cell>; 4]) -> Burrow {
        let mut state = Burrow {
            cost: 0,
            corridor: [Cell::E; 11],
            rooms: initial.clone()
        };
        state.corridor[2] = Cell::F;
        state.corridor[4] = Cell::F;
        state.corridor[6] = Cell::F;
        state.corridor[8] = Cell::F;
        state
    }
}

fn parse_input(lines: &[String], depth: usize) -> [VecDeque<Cell>; 4] {
    let mut res = [VecDeque::new(), VecDeque::new(), VecDeque::new(), VecDeque::new()];
    for (i, ci) in [3, 5, 7, 9].into_iter().enumerate() {
        for k in 2..(2 + depth) {
            if let Some(c) = lines[k].chars().nth(ci) {
                res[i].push_front(match c {
                    'A' => Cell::A,
                    'B' => Cell::B,
                    'C' => Cell::C,
                    'D' => Cell::D,
                    _ => panic!()
                });
            }
        }
    }
    res
}
