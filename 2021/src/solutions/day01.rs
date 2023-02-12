use super::Solution;
use crate::input::Input;

pub struct Day01;

impl Solution for Day01 {
    fn day(&self) -> i32 { 2 }
    fn name(&self) -> &'static str { "Dive!" }

    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        None
    }

    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        None
    }
}