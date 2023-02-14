use super::Solution;
use crate::input::Input;
use crate::common::math::Vec2;
use std::collections::HashMap;

pub struct Day05;

impl Solution for Day05 {
    fn day(&self) -> i32 { 5 }
    fn name(&self) -> &'static str { "Hydrothermal Venture" }

    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let cells = parse_input(input, false);
        let res: usize = cells.iter().filter(|n| *n.1 > 1).count();
        Some(Box::new(res))
    }

    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let cells = parse_input(input, true);
        let res: usize = cells.iter().filter(|n| *n.1 > 1).count();
        Some(Box::new(res))
    }
}

fn parse_input(input: &Input, diagonal: bool) -> HashMap<Vec2, i32> {
    let mut cells = HashMap::new();
    for line in input.lines.iter().map(|n| n.replace(" -> ", " ").replace(',', " ")) {
        if let [x_from, y_from, x_to, y_to] = line.split(' ').map(|n| n.parse::<i32>().unwrap()).collect::<Vec<i32>>()[..] {
            let mut from = Vec2::new(x_from, y_from);
            let to = Vec2::new(x_to, y_to);

            let mut dir = to - from;
            if !diagonal && dir.x != 0 && dir.y != 0 {
                continue;
            }
            let steps = i32::max(i32::abs(dir.x), i32::abs(dir.y));
            dir.x = i32::clamp(dir.x, -1, 1);
            dir.y = i32::clamp(dir.y, -1, 1);

            for _ in 0..=steps {
                let entry = cells.entry(from).or_insert(0);
                *entry += 1;
                from += dir;
            }
        };
    }
    cells
}
