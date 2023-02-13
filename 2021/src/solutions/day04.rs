use super::Solution;
use crate::input::Input;

pub struct Day04;

impl Solution for Day04 {
    fn day(&self) -> i32 { 4 }
    fn name(&self) -> &'static str { "Giant Squid" }

    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let (numbers, mut boards) = parse_input(input);
        for n in numbers {
            for board in &mut boards {
                if board.simulate(n) {
                    return Some(Box::new(board.score(n)));
                }
            }
        }
        None
    }

    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let (numbers, mut boards) = parse_input(input);
        for n in numbers {
            for i in (0..boards.len()).rev() {
                if boards[i].simulate(n) {
                    if boards.len() == 1 {
                        return Some(Box::new(boards[i].score(n)));
                    }
                    boards.remove(i);
                }
            }
        }
        None
    }
}

fn parse_input(input: &Input) -> (Vec<i32>, Vec<Board>) {
    let numbers: Vec<i32> = input.lines[0].split(',').map(|n| n.parse::<i32>().unwrap()).collect();
    let boards = input.source.replace('\r', "").split("\n\n").skip(1).map(|s| Board::parse(s)).collect::<Vec<Board>>();
    (numbers, boards)
}

#[derive(Clone, Copy)]
enum Cell {
    Set(i32),
    Unset(i32)
}

struct Board {
    cells: [[Cell; 5]; 5]
}

impl Board {
    fn parse(s: &str) -> Board {
        let mut cells = [[Cell::Unset(0); 5]; 5];
        for (i, line) in s.lines().enumerate() {
            for (k, value) in line.split(' ').filter(|n| !n.trim().is_empty()).enumerate() {
                cells[i][k] = Cell::Unset(value.trim().parse::<i32>().unwrap());
            }
        }        
        Board { cells }
    }

    fn simulate(&mut self, n: i32) -> bool {
        'outer: for i in 0..5 {
            for k in 0..5 {
                if match self.cells[i][k] { Cell::Set(v) => v == n, Cell::Unset(v) => v == n } {
                    self.cells[i][k] = Cell::Set(n);
                    break 'outer;
                }
            }
        }
        self.is_bingo()
    }

    fn is_bingo(&self) -> bool {
        for i in 0..5 {
            if self.cells[i].iter().all(|n| matches!(n, Cell::Set(_))) {
                return true;
            }
        }
        for k in 0..5 {
            if (0..5).all(|n| matches!(self.cells[n][k], Cell::Set(_))) {
                return true;
            }
        }
        false
    }

    fn score(&self, last: i32) -> i32 {
        let mut score = 0;
        for i in 0..5 {
            for k in 0..5 {
                if let Cell::Unset(v) = self.cells[i][k] {
                    score += v;
                }
            }
        }
        score * last
    }
}
