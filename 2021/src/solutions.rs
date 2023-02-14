mod day01;
mod day02;
mod day03;
mod day04;
mod day05;

pub use day01::Day01;
pub use day02::Day02;
pub use day03::Day03;
pub use day04::Day04;
pub use day05::Day05;
use crate::input::Input;

pub trait Solution {
    fn day(&self) -> i32;
    fn name(&self) -> &'static str;
    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>>;
    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>>;
}

pub fn all_solutions() -> Vec<Box<dyn Solution>> {
    let v: Vec<Box<dyn Solution>> = vec![
        Box::new(Day01),
        Box::new(Day02),
        Box::new(Day03),
        Box::new(Day04),
        Box::new(Day05)
    ];
    v
}
