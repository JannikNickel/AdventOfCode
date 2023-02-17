use super::Solution;
use crate::input::Input;
use std::collections::HashMap;

pub struct Day14;

impl Solution for Day14 {
    fn day(&self) -> i32 { 14 }
    fn name(&self) -> &'static str { "Extended Polymerization" }

    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let (mut sequence, rules) = parse_input(input);
        for _ in 0..10 {
            polymerize(&mut sequence, &rules);
        }
        let res = most_common(&sequence) - least_common(&sequence);
        Some(Box::new(res))
    }

    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let (sequence, rules) = parse_input(input);
        
        let mut pairs: HashMap<(char, char), u64> = HashMap::new();
        for i in 0..(sequence.len() - 1) {
            *pairs.entry((sequence[i], sequence[i + 1])).or_default() += 1;
        }
        for _ in 0..40 {
            let mut new_pairs = HashMap::new();
            for ((l, r), count) in pairs {
                if let Some(n) = rules.get(&(l, r)) {
                    *new_pairs.entry((l, *n)).or_default() += count;
                    *new_pairs.entry((*n, r)).or_default() += count;
                }
            }
            pairs = new_pairs;
        }

        let mut m: HashMap<char, u64> = HashMap::new();
        *m.entry(sequence[0]).or_default() += 1;
        for pair in pairs {
            *m.entry(pair.0.1).or_default() += pair.1;
        }
        let most = m.iter().max_by_key(|(_, n)| *n).map(|(_, c)| c).unwrap();
        let least = m.iter().min_by_key(|(_, n)| *n).map(|(_, c)| c).unwrap();

        Some(Box::new(most - least))
    }
}

fn most_common(seq: &Vec<char>) -> usize {    
    count_elements(seq).into_iter().max_by_key(|(_, n)| *n).map(|(_, c)| c).unwrap()
}

fn least_common(seq: &Vec<char>) -> usize {
    count_elements(seq).into_iter().min_by_key(|(_, n)| *n).map(|(_, c)| c).unwrap()
}

fn count_elements(seq: &Vec<char>) -> HashMap<char, usize> {
    let mut m: HashMap<char, usize> = HashMap::new();
    for c in seq {
        *m.entry(*c).or_default() += 1;
    }
    m
}

fn polymerize(sequence: &mut Vec<char>, rules: &HashMap<(char, char), char>) {
    let mut new = Vec::with_capacity(sequence.len() * 2 - 1);

    for pair in sequence.windows(2) {
        if let [l, r] = pair {
            new.push(*l);
            if let Some(n) = rules.get(&(*l, *r)) {
                new.push(*n);
            }
        }
    }
    new.push(*sequence.last().unwrap());

    *sequence = new;
}

fn parse_input(input: &Input) -> (Vec<char>, HashMap<(char, char), char>) {
    let mut rules = HashMap::new();
    for line in input.lines.iter().skip(2) {
        if let [l, r] = line.split(" -> ").collect::<Vec<&str>>()[..] {
            rules.insert((l.chars().nth(0).unwrap(), l.chars().nth(1).unwrap()), r.chars().nth(0).unwrap());
        }
    }
    (input.lines[0].chars().collect(), rules)
}
