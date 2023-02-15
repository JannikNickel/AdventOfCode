use super::Solution;
use crate::input::Input;

pub struct Day08;

impl Solution for Day08 {
    fn day(&self) -> i32 { 8 }
    fn name(&self) -> &'static str { "Seven Segment Search" }

    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let notes = parse_input(input);
        let result: usize = notes.iter()
            .map(|n| n.output.iter().filter(|n| n.len() != 5 && n.len() != 6).count())
            .sum();
        Some(Box::new(result))
    }

    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let notes = parse_input(input);        
        let result: i32 = notes.iter()
            .map(|n| {
                let map = solve_note(n);
                map[0] * 1000 + map[1] * 100 + map[2] * 10 + map[3]
            })
            .sum();
        Some(Box::new(result))
    }
}

fn parse_input(input: &Input) -> Vec<Note> {
    input.lines.iter()
        .map(|n| n.split(" | ").collect::<Vec<&str>>())
        .map(|n| Note {
            pattern: n[0].split(' ').collect(),
            output: n[1].split(' ').collect()
        }).collect()
}

fn solve_note<'a>(note: &'a Note) -> Vec<i32> {
    let d1 = find(note, &|n| n.len() == 2);
    let d4 = find(note, &|n| n.len() == 4);
    note.output.iter()
        .map(|output| match output.len() {
            2 => 1,
            3 => 7,
            4 => 4,
            7 => 8,
            5 => {
                if overlap(output, d1) == 2 {
                    3
                } else if overlap(output, d4) == 2 {
                    2
                } else {
                    5
                }
            },
            6 => {
                if overlap(output, d4) == 4 {
                    9
                } else if overlap(output, d1) == 2 {
                    0
                } else {
                    6
                }
            },
            _ => panic!()
        }).collect()
}

fn overlap(signal: &str, other: &str) -> usize {
    signal.chars().filter(|&n| other.contains(n)).count()
}

fn find<'a>(note: &'a Note, predicate: &dyn Fn(&str) -> bool) -> &'a str {
    note.pattern.iter().find(|n| predicate(n)).unwrap()
}

struct Note<'a> {
    pattern: Vec<&'a str>,
    output: Vec<&'a str>
}
