use super::Solution;
use crate::input::Input;
use std::{collections::HashSet, hash::Hash};

pub struct Day15;

impl Solution for Day15 {
    fn day(&self) -> i32 { 15 }
    fn name(&self) -> &'static str { "Chiton " }

    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let mut grid = parse_input(input);
        let risk = dijkstra(&mut grid);
        Some(Box::new(risk))
    }

    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let mut grid = parse_input(input);
        expand(&mut grid);
        let risk = dijkstra(&mut grid);
        Some(Box::new(risk))
    }
}

fn expand(grid: &mut Vec<Vec<Cell>>) {
    let w = grid.len();
    let h = grid[0].len();
    for i in 0..4 {
        for k in 0..w {
            for l in 0..h {
                let cell = Cell {
                    x: grid[k][l].x,
                    y: (l + i * h + h) as i32,
                    risk_level: if grid[k][l + i * h].risk_level + 1 > 9 { 1 } else { grid[k][l + i * h].risk_level + 1 },
                    total_risk_level: i32::MAX
                };
                grid[k].push(cell);
            }
        }
    }
    for i in 0..4 {
        for k in 0..w {
            grid.push(grid[k + i * w].iter().map(|n| Cell {
                x: (w * i + w + k) as i32,
                y: n.y,
                risk_level: if n.risk_level + 1 > 9 { 1 } else { n.risk_level + 1 },
                total_risk_level: i32::MAX
            }).collect());
        }
    }
}

fn dijkstra(grid: &mut Vec<Vec<Cell>>) -> i32 {
    let neighbours = [(-1, 0), (0, 1), (1, 0), (0, -1)];
    let w = grid.len() as i32;
    let h = grid[0].len() as i32;
    
    grid[0][0].total_risk_level = 0;
    let end = grid[w as usize - 1][h as usize - 1];
    let mut open = Vec::from_iter(vec![(0, 0)]);
    let mut closed = HashSet::new();

    while !open.is_empty() {
        open.sort_by(|a, b| grid[b.0][b.1].total_risk_level.cmp(&grid[a.0][a.1].total_risk_level));
        let (vx, vy) = open.pop().unwrap();
        let v = grid[vx][vy];
        if v == end {
            return v.total_risk_level;
        }

        for off in neighbours {
            let x = v.x + off.0;
            let y = v.y + off.1;
            if x < 0 || y < 0 || x >= w || y >= h {
                continue;
            }
            
            let mut n = &mut grid[x as usize][y as usize];
            if v.total_risk_level + n.risk_level < n.total_risk_level {
                n.total_risk_level = v.total_risk_level + n.risk_level;
                if !closed.contains(n) {
                    closed.insert(*n);
                    open.push((x as usize, y as usize));
                }
            }
        }
    }
    0
}

fn parse_input(input: &Input) -> Vec<Vec<Cell>> {
    let w = input.lines[0].chars().count();
    let h = input.lines.len();
    let mut grid = vec![vec![Cell { x: 0, y: 0, risk_level: 0, total_risk_level: i32::MAX }; h]; w];
    for (i, line) in input.lines.iter().enumerate() {
        for k in 0..w {
            let risk = (line.chars().nth(k).unwrap() as i32 - '0' as i32) as i32;
            grid[k][i].x = k as i32;
            grid[k][i].y = i as i32;
            grid[k][i].risk_level = risk;
        }
    }
    grid
}

#[derive(Debug, Clone, Copy, Eq)]
struct Cell {
    x: i32,
    y: i32,
    risk_level: i32,
    total_risk_level: i32,
}

impl PartialEq for Cell {
    fn eq(&self, other: &Self) -> bool {
        self.x == other.x && self.y == other.y && self.risk_level == other.risk_level
    }
}

impl Hash for Cell {
    fn hash<H: std::hash::Hasher>(&self, state: &mut H) {
        self.x.hash(state);
        self.y.hash(state);
        self.risk_level.hash(state);
    }
}
