use super::Solution;
use crate::input::Input;

pub struct Day03;

#[derive(PartialEq)]
enum Op {
    Greater,
    Less
}

impl Solution for Day03 {
    fn day(&self) -> i32 { 3 }
    fn name(&self) -> &'static str { "Binary Diagnostic" }

    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let line_amount = input.lines.len();
        let line_size = input.lines[0].len();
        let mut gamma = 0;
        let mut epsilon = 0;
        for i in 0..line_size {
            let bits = count_set(input.lines.iter().map(|n| n.as_str()), line_size - i - 1);
            if bits as f64 / line_amount as f64 > 0.5 {
                gamma |= 1 << i;
            } else {
                epsilon |= 1 << i;
            }
        }
        Some(Box::new(gamma * epsilon))
    }

    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let line_size = input.lines[0].len();
        let oxy = find_rating(&input.lines, line_size, Op::Greater);
        let co2 = find_rating(&input.lines, line_size, Op::Less);

        Some(Box::new(oxy * co2))
    }
}

fn find_rating(numbers: &Vec<String>, line_size: usize, op: Op) -> i32 {
    let mut open: Vec<usize> = (0..numbers.len()).into_iter().collect();
    let mut res = 0;

    for i in 0..line_size {
        let set = count_set(open.iter().map(|n| numbers[*n].as_str()), i);
        let unset = open.len() - set;

        let mut target = match op {
            Op::Greater => '0',
            Op::Less => '1'
        };
        if set >= unset {
            target = match op {
                Op::Greater => '1',
                Op::Less => '0'
            }
        }

        for k in (0..open.len()).rev() {
            let x = numbers[open[k]].chars().nth(i).unwrap();
            if x != target {
                open.remove(k);
            }
        }
        if open.len() == 1 {
            res = i32::from_str_radix(&numbers[open[0]], 2).unwrap();
        }
    }

    res
}

fn count_set<'a, I>(values: I, index: usize) -> usize where I: Iterator<Item = &'a str> {
    let mut c = 0;
    for i in values {
        if i.chars().nth(index).unwrap() == '1' {
            c += 1;
        }
    }
    c
}
