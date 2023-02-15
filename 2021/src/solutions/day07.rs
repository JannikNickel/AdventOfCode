use super::Solution;
use crate::input::Input;
use std::ops::Range;

pub struct Day07;

impl Solution for Day07 {
    fn day(&self) -> i32 { 7 }
    fn name(&self) -> &'static str { "The Treachery of Whales" }

    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let positions = parse_input(input);
        let best = find_min(&positions, &|from, to| i32::abs(to - from));
        Some(Box::new(best))
    }

    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let positions = parse_input(input);
        let best = find_min(&positions, &|from, to| int_sum(i32::abs(to - from)));
        Some(Box::new(best))
    }
}

fn find_min(positions: &Vec<i32>, fuel_func: &dyn Fn(i32, i32) -> i32) -> i32 {
    min_max_range(&positions)
        .map(|n| positions
            .iter()
            .map(|p| fuel_func(n, *p))
            .sum::<i32>()
        ).min().unwrap()
}

fn int_sum(n: i32) -> i32 {
    n * (n + 1) / 2
}

fn min_max_range(v: &Vec<i32>) -> Range<i32> {
    *(v.iter().min().unwrap())..*(v.iter().max().unwrap())
}

fn parse_input(input: &Input) -> Vec<i32> {
    input.source.split(',').map(|n| n.parse::<i32>().unwrap()).collect()
}
