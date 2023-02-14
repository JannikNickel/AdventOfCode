use super::Solution;
use crate::input::Input;

pub struct Day06;

impl Solution for Day06 {
    fn day(&self) -> i32 { 6 }
    fn name(&self) -> &'static str { "Lanternfish" }

    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let res = simulate(&parse_input(input), 80);
        Some(Box::new(res))
    }

    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let res = simulate(&parse_input(input), 256);
        Some(Box::new(res))
    }
}

fn simulate(fishes: &Vec<i32>, days: i32) -> u64 {
    let mut generation: [u64; 9] = [0; 9];
    for fish in fishes {
        generation[*fish as usize] += 1;
    }

    let mut index = 0;
    for _ in 0..days {
        let spawn = generation[index % 9];
        index += 1;
        generation[(index + 6) % 9] += spawn;
    }

    generation.iter().map(|n| *n as u64).sum()
}

fn parse_input(input: &Input) -> Vec<i32> {
    input.source.split(',').map(|n| n.parse::<i32>().unwrap()).collect()
}
