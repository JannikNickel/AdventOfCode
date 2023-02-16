use super::Solution;
use crate::input::Input;
use crate::common::structures::Grid;

pub struct Day11;

impl Solution for Day11 {
    fn day(&self) -> i32 { 11 }
    fn name(&self) -> &'static str { "Dumbo Octopus" }

    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let mut grid = parse_input(input);
        let res: i32 = (0..100).into_iter().map(|_| simulate(&mut grid)).sum();
        Some(Box::new(res))
    }

    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let mut grid = parse_input(input);
        let octopus_count = grid.width() * grid.height();
        let mut i = 0;
        while simulate(&mut grid) as usize != octopus_count {
            i += 1;
        }
        Some(Box::new(i + 1))
    }
}

fn simulate(grid: &mut Grid<i32>) -> i32 {
    grid.iter_mut().for_each(|n| n.value += 1);

    let mut flashes = 0;
    for x in 0..grid.width() {
        for y in 0..grid.height() {
            if *grid.get(x, y) == 10 {
                grid.set(x, y, 11);
                flashes += 1;
                flashes += propagate(grid, x as i32, y as i32);
            }
        }
    }

    grid.iter_mut().for_each(|n| {
        if n.value > 9 {
            n.value = 0;
        }
    });
    flashes
}

fn propagate(grid: &mut Grid<i32>, x: i32, y: i32) -> i32 {
    let mut sum = 0;
    for i in (-1)..=1 {
        for k in (-1)..=1 {
            let nx = x + i;
            let ny = y + k;
            if nx < 0 || ny < 0 || nx as usize >= grid.width() || ny as usize >= grid.height() {
                continue;
            }

            let mut cell = *grid.get(nx as usize, ny as usize);
            if cell <= 9 {
                cell += 1;
                grid.set(nx as usize, ny as usize, cell);
                if cell == 10 {
                    grid.set(nx as usize, ny as usize, 11);
                    sum += 1;
                    sum += propagate(grid, nx, ny);
                }
            }
        }
    }
    sum
}

fn parse_input(input: &Input) -> Grid<i32> {
    let mut grid = Grid::new(input.lines[0].chars().count(), input.lines.len(), &0);

    for (i, line) in input.lines.iter().enumerate() {
        for k in 0..grid.width() {
            grid.set(k, i, (line.chars().nth(k).unwrap() as i32 - '0' as i32) as i32)
        }
    }

    grid
}
