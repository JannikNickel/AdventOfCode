use super::Solution;
use crate::input::Input;
use crate::common::structures::Grid;

pub struct Day25;

impl Solution for Day25 {
    fn day(&self) -> i32 { 25 }
    fn name(&self) -> &'static str { "Sea Cucumber" }

    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let mut grid = parse_input(input);
        let mut steps = 1;
        while simulate(&mut grid, Cell::East) + simulate(&mut grid, Cell::South) != 0 {
            steps += 1;
        }
        Some(Box::new(steps))
    }

    fn run2(&self, _: &Input) -> Option<Box<dyn std::fmt::Display>> {
        //Day 25 only has 1 puzzle
        None
    }
}

fn simulate(grid: &mut Grid<Cell>, dir: Cell) -> usize {
    let mut moves = Vec::new();
    let x_off = (dir == Cell::East) as usize;
    let y_off = (dir == Cell::South) as usize;
    for p in grid.iter() {
        let nx = (p.x + x_off) % grid.width();
        let ny = (p.y + y_off) % grid.height();
        if *grid.get(p.x, p.y) == dir && *grid.get(nx, ny) == Cell::Empty {
            moves.push(((p.x, p.y), (nx, ny)));
        }
    }
    for ((fx, fy), (tx, ty)) in &moves {
        grid.set(*fx, *fy, Cell::Empty);
        grid.set(*tx, *ty, dir);
    }
    moves.len()
}

#[derive(Debug, Clone, Copy, PartialEq)]
enum Cell {
    Empty,
    East,
    South
}

fn parse_input(input: &Input) -> Grid<Cell> {
    let mut grid = Grid::new(input.lines[0].len(), input.lines.len(), &Cell::Empty);
    for (i, line) in input.lines.iter().enumerate() {
        for (k, c) in line.chars().enumerate() {
            grid.set(k, i, match c {
                '.' => Cell::Empty,
                '>' => Cell::East,
                'v' => Cell::South,
                _ => panic!()
            });
        }
    }
    grid
}
