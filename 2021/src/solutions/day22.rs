use super::Solution;
use crate::input::Input;
use std::collections::HashSet;

pub struct Day22;

impl Solution for Day22 {
    fn day(&self) -> i32 { 22 }
    fn name(&self) -> &'static str { "Reactor Reboot" }

    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let ops = parse_input(input);
        let res = initialization(&ops);
        Some(Box::new(res))
    }

    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let ops = parse_input(input);
        let res = reboot(&ops);
        Some(Box::new(res))
    }
}

fn reboot(ops: &[Operation]) -> i64 {
    let mut pos: Vec<Operation> = Vec::new();
    let mut neg: Vec<Operation> = Vec::new();

    for op in ops {
        let o1: Vec<Operation> = pos.iter().map(|n| n.overlap(op)).filter(|n| n.is_some()).map(|n| n.unwrap()).collect();
        let o2: Vec<Operation> = neg.iter().filter(|n| !op.on || n.on).map(|n| n.overlap(op)).filter(|n| n.is_some()).map(|n| n.unwrap()).collect();
        pos.extend(o2);
        neg.extend(o1);
        if op.on {
            pos.push(Operation { on: false, ..*op });
        }
    }
    pos.iter().map(|n| n.volume()).sum::<i64>() - neg.iter().map(|n| n.volume()).sum::<i64>()
}

fn initialization(ops: &[Operation]) -> usize {
    const MIN: i64 = -50;
    const MAX: i64 = 50;
    let mut set = HashSet::new();
    for op in ops {
        for x in i64::max(op.x_min, MIN)..=i64::min(op.x_max, MAX) {
            for y in i64::max(op.y_min, MIN)..=i64::min(op.y_max, MAX) {
                for z in i64::max(op.z_min, MIN)..=i64::min(op.z_max, MAX) {
                    if op.on {
                        set.insert((x, y, z));
                    } else {
                        set.remove(&(x, y, z));
                    }
                }
            }
        }
    }
    set.len()
}

fn parse_input(input: &Input) -> Vec<Operation> {
    let mut ops = Vec::new();
    for line in &input.lines {
        let on = line.starts_with("on");
        if let [x0, x1, y0, y1, z0, z1] = line.replace("on x=", "").replace("off x=", "").replace("y=", "").replace("z=", "").replace("..", ",")
            .split(',')
            .map(|n| n.parse::<i64>().unwrap())
            .collect::<Vec<i64>>()[..] {            

            ops.push(Operation { on: on, x_min: x0, x_max: x1, y_min: y0, y_max: y1, z_min: z0, z_max: z1 });
        }
    }
    ops
}

#[derive(Debug, Clone, Copy)]
struct Operation {
    on: bool,
    x_min: i64,
    x_max: i64,
    y_min: i64,
    y_max: i64,
    z_min: i64,
    z_max: i64
}

impl Operation {
    fn new(on: bool, x_min: i64, x_max: i64, y_min: i64, y_max: i64, z_min: i64, z_max: i64) -> Operation {
        Operation { on, x_min, x_max, y_min, y_max, z_min, z_max }
    }

    fn volume(&self) -> i64 {
        (self.x_max - self.x_min + 1) * (self.y_max - self.y_min + 1) * (self.z_max - self.z_min + 1)
    }

    fn overlap(&self, other: &Operation) -> Option<Operation> {
        let op = Operation::new(true,
            i64::max(self.x_min, other.x_min), i64::min(self.x_max, other.x_max),
            i64::max(self.y_min, other.y_min), i64::min(self.y_max, other.y_max),
            i64::max(self.z_min, other.z_min), i64::min(self.z_max, other.z_max));
        if op.x_min <= op.x_max && op.y_min <= op.y_max && op.z_min <= op.z_max {
            return Some(op);
        }
        None
    }
}
