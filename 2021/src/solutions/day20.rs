use super::Solution;
use crate::input::Input;
use crate::common::math::Vec2;
use std::collections::HashSet;

pub struct Day20;

impl Solution for Day20 {
    fn day(&self) -> i32 { 20 }
    fn name(&self) -> &'static str { "Trench Map" }

    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let (mut image, algorithm) = parse_input(input);
        for i in 0..2 {
            image = enhance(image, &algorithm, i);
        }
        Some(Box::new(image.len()))
    }

    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let (mut image, algorithm) = parse_input(input);
        for i in 0..50 {
            image = enhance(image, &algorithm, i);
        }
        Some(Box::new(image.len()))
    }
}

fn enhance(image: HashSet<Vec2>, algorithm: &[bool], iter: usize) -> HashSet<Vec2> {
    let default = algorithm[0] && iter % 2 == 1;
    let min_x = image.iter().map(|n| n.y).min().unwrap() - 1;
    let min_y = image.iter().map(|n| n.x).min().unwrap() - 1;
    let max_x = image.iter().map(|n| n.y).max().unwrap() + 1;
    let max_y = image.iter().map(|n| n.x).max().unwrap() + 1;
    
    let mut new = HashSet::new();
    for i in min_y..=max_y {
        for k in min_x..=max_x {
            if set(&image, algorithm, default, k, i, (min_x, min_y, max_x, max_y)) {
                new.insert(Vec2::new(i, k));
            }
        }
    }
    new
}

fn set(image: &HashSet<Vec2>, algorithm: &[bool], default: bool, x: i32, y: i32, bounds: (i32, i32, i32, i32)) -> bool {
    let mut index = 0;
    let mut s = 9;
    for ny in (y - 1)..=(y + 1) {
        for nx in (x - 1)..=(x + 1) {
            s -= 1;
            if image.contains(&Vec2::new(ny, nx)) || (default && (ny <= bounds.1 || ny >= bounds.3 || nx <= bounds.0 || nx >= bounds.2)) {
                index |= 1 << s;
            }
        }
    }    
    algorithm[index]
}

fn parse_input(input: &Input) -> (HashSet<Vec2>, Vec<bool>) {
    let algorithm = input.lines[0].chars().map(|n| n == '#').collect();
    let mut image = HashSet::new();
    for (i, line) in input.lines.iter().skip(2).enumerate() {
        for (k, c) in line.chars().enumerate() {
            if c == '#' {
                image.insert(Vec2::new(i as i32, k as i32));
            }
        }
    }    
    (image, algorithm)
}
