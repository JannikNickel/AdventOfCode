use super::Solution;
use crate::input::Input;

pub struct Day01;

impl Solution for Day01 {
    fn day(&self) -> i32 { 2 }
    fn name(&self) -> &'static str { "Dive!" }

    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let mut amount = 0;
        for i in 1..input.lines.len() {
            let prev = input.lines[i - 1].parse::<i32>().unwrap();
            if input.lines[i].parse::<i32>().unwrap() > prev {
                amount += 1;
            }
        }

        Some(Box::new(amount))
    }

    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let mut amount = 0;
        let mut prev = i32::MAX;
        for i in input.lines.windows(3).map(|n| n[0].parse::<i32>().unwrap() + n[1].parse::<i32>().unwrap() + n[2].parse::<i32>().unwrap()) {
            if i > prev {
                amount += 1;
            }
            prev = i;
        }
        Some(Box::new(amount))
    }
}