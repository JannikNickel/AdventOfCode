automod::dir!("src/solutions/");

use crate::input::Input;

pub trait Solution {
    fn day(&self) -> i32;
    fn name(&self) -> &'static str;
    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>>;
    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>>;
}

pub fn all_solutions() -> Vec<Box<dyn Solution>> {
    let v: Vec<Box<dyn Solution>> = vec![
        Box::new(day01::Day01),
        Box::new(day02::Day02),
        Box::new(day03::Day03),
        Box::new(day04::Day04),
        Box::new(day05::Day05),
        Box::new(day06::Day06),
        Box::new(day07::Day07),
        Box::new(day08::Day08),
        Box::new(day09::Day09),
        Box::new(day10::Day10),
        Box::new(day11::Day11),
        Box::new(day12::Day12),
        Box::new(day13::Day13),
        Box::new(day14::Day14),
        Box::new(day15::Day15),
        Box::new(day16::Day16),
        Box::new(day17::Day17),
        Box::new(day18::Day18),
        Box::new(day19::Day19),
        Box::new(day20::Day20),
        Box::new(day21::Day21),
        Box::new(day22::Day22),
        Box::new(day23::Day23),
        Box::new(day24::Day24),
        Box::new(day25::Day25)
    ];
    v
}
