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
        Box::new(day08::Day08)
    ];
    v
}
