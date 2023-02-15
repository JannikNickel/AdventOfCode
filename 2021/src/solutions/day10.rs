use super::Solution;
use crate::input::Input;

pub struct Day10;

impl Solution for Day10 {
    fn day(&self) -> i32 { 10 }
    fn name(&self) -> &'static str { "Syntax Scoring" }

    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let res: i32 = input.lines.iter().map(|n| error_score(n).0).sum();
        Some(Box::new(res))
    }

    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let mut scores: Vec<u64> = input.lines.iter().map(|n| completion_score(n)).filter(|n| *n > 0).collect();
        scores.sort();
        let res = scores[scores.len() / 2];
        Some(Box::new(res))
    }
}

fn error_score(line: &str) -> (i32, Vec<char>) {
    let mut stack = Vec::new();
    for c in line.chars() {
        if c == '(' || c == '[' || c == '{' || c == '<' {
            stack.push(c);
            continue;
        }
        
        if let Some(v) = lookup_error(c) {
            if let Some(e) = stack.pop() {
                if v.0 == e {
                    continue;
                }
            }
            return (v.1, stack);
        }
    }
    (0, stack)
}

fn completion_score(line: &str) -> u64 {
    let (error, mut remaining) = error_score(line);
    let mut score: u64 = 0;
    if error == 0 {
        while !remaining.is_empty() {
            score *= 5;
            score += lookup_completion(remaining.pop().unwrap()).unwrap().1 as u64;
        }
    }
    score
}

fn lookup_error(c: char) -> Option<(char, i32)> {
    match c {
        ')' => Some(('(', 3)),
        ']' => Some(('[', 57)),
        '}' => Some(('{', 1197)),
        '>' => Some(('<', 25137)),
        _ => None
    }
}

fn lookup_completion(c: char) -> Option<(char, i32)> {
    match c {
        '(' => Some((')', 1)),
        '[' => Some((']', 2)),
        '{' => Some(('}', 3)),
        '<' => Some(('>', 4)),
        _ => None
    }
}
