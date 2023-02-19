use super::Solution;
use crate::input::Input;

pub struct Day17;

impl Solution for Day17 {
    fn day(&self) -> i32 { 17 }
    fn name(&self) -> &'static str { "Trick Shot" }

    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let target = parse_input(input);
        let res = *simulate_all(&target).iter().max().unwrap();
        Some(Box::new(res))
    }

    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let target = parse_input(input);
        let res = simulate_all(&target).iter().count();
        Some(Box::new(res))
    }
}

fn simulate_all(target: &Bounds) -> Vec<i32> {
    let mut solutions = Vec::new();
    for x in 0..=target.x_max {
        for y in target.y_min..=target.y_min.abs() {
            if let Some(h) = simulate(target, x, y) {
                solutions.push(h);
            }
        }
    }    
    solutions
}

fn simulate(target: &Bounds, mut x_vel: i32, mut y_vel: i32) -> Option<i32> {
    let mut px = 0;
    let mut py = 0;
    let mut max_y = i32::MIN;
    while px <= target.x_max && py >= target.y_min {
        px += x_vel;
        py += y_vel;
        x_vel -= sign(x_vel);
        y_vel -= 1;
        max_y = i32::max(max_y, py);
        if target.in_bounds(px, py) {
            return Some(max_y);
        }
    }
    None
}

fn sign(value: i32) -> i32 {
    return match value {
        v if v < 0 => -1,
        v if v > 0 => 1,
        _ => 0
    }
}

fn parse_input(input: &Input) -> Bounds {
    if let [x0, x1, y0, y1] = input.source.replace("target area: x=", "").replace(", y=", "..").split("..").map(|n| n.parse::<i32>().unwrap()).collect::<Vec<i32>>()[..] {
        return Bounds {
            x_min: i32::min(x0, x1),
            x_max: i32::max(x0, x1),
            y_min: i32::min(y0, y1),
            y_max: i32::max(y0, y1)
        };
    }
    panic!();
}

struct Bounds {
    x_min: i32,
    x_max: i32,
    y_min: i32,
    y_max: i32
}

impl Bounds {
    fn in_bounds(&self, x: i32, y: i32) -> bool {
        x >= self.x_min && x <= self.x_max && y >= self.y_min && y <= self.y_max
    }
}
