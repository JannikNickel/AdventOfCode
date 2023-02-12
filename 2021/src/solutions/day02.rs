use super::Solution;
use crate::input::Input;

pub struct Day02;

impl Solution for Day02 {
    fn day(&self) -> i32 { 2 }
    fn name(&self) -> &'static str { "Dive!" }

    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let mut h = 0;
        let mut d = 0;

        for line in &input.lines {
            if let [op, num] = line.split(' ').collect::<Vec<&str>>()[..] {
                let n = num.parse::<i32>().unwrap();
                match op {
                    "forward" => h += n,
                    "down" => d += n,
                    "up" => d -= n,
                    _ => {}
                };
            };
        }

        Some(Box::new(h * d))
    }

    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let mut h = 0;
        let mut d = 0;
        let mut aim = 0;

        for line in &input.lines {
            if let [op, num] = line.split(' ').collect::<Vec<&str>>()[..] {
                let n = num.parse::<i32>().unwrap();
                match op {
                    "forward" => {
                        h += n;
                        d += aim * n;
                    },
                    "down" => aim += n,
                    "up" => aim -= n,
                    _ => {}
                };
            };
        }

        Some(Box::new(h * d))
    }
}