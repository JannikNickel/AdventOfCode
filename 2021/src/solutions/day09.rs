use super::Solution;
use crate::input::Input;
use crate::common::structures::{Heightmap, HeightmapPoint};
use std::collections::{HashSet, VecDeque};

pub struct Day09;

impl Solution for Day09 {
    fn day(&self) -> i32 { 9 }
    fn name(&self) -> &'static str { "Smoke Basin" }

    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let heightmap = parse_input(input);
        let res: i32 = heightmap.iter2d()
            .filter(|n| heightmap.vn_neighbour_iter(n.x, n.y).all(|k| n.value < *k))
            .map(|n| n.value + 1)
            .sum();

        Some(Box::new(res))
    }

    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let heightmap = parse_input(input);
        let mut closed: HashSet<(usize, usize)> = HashSet::new();
        let mut basins = Vec::new();
        for point in heightmap.iter2d() {
            if point.value == 9 || closed.contains(&(point.x, point.y)) {
                continue;
            }
            basins.push(flood_fill(&heightmap, &point, &mut closed));
        }
        
        basins.sort_by(|a, b| b.cmp(a));
        let res: i32 = basins.iter().take(3).fold(1, |s, v| s * v);
        Some(Box::new(res))
    }
}

fn flood_fill(heightmap: &Heightmap<i32>, origin: &HeightmapPoint<i32>, closed: &mut HashSet<(usize, usize)>) -> i32 {
    let mut open = VecDeque::new();
    open.push_back(*origin);
    closed.insert((origin.x, origin.y));
    
    let mut size = 0;
    while !open.is_empty() {
        let v = open.pop_front().unwrap();
        size += 1;
        for neighbour in heightmap.vn_neighbour_iter2d(v.x, v.y) {
            if neighbour.value == 9 || closed.contains(&(neighbour.x, neighbour.y)) {
                continue;
            }
            closed.insert((neighbour.x, neighbour.y));
            open.push_back(neighbour);
        }
    }
    size
}

fn parse_input(input: &Input) -> Heightmap<i32> {
    let w = input.lines[0].len();
    let h = input.lines.len();
    let mut map = Heightmap::new(w, h, 0);

    for (i, line) in input.lines.iter().enumerate() {
        for k in 0..w {
            map.set(k, i, (line.chars().nth(k).unwrap() as i32 - '0' as i32) as i32)
        }
    }
    map
}
