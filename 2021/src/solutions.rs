mod day01;
mod day02;

pub use day01::Day01;
pub use day02::Day02;
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
        Box::new(Day02)
    ];
    v
}
