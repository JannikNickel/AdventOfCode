use super::Solution;
use crate::input::Input;
use crate::common::structures::Graph;
use std::collections::{HashSet, VecDeque};

pub struct Day12;

impl Solution for Day12 {
    fn day(&self) -> i32 { 12 }
    fn name(&self) -> &'static str { "Passage Pathing" }

    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let graph = parse_input(input);
        let res = find_paths(&graph, &Cave::Small("start"), &Cave::Small("end"), false).len();
        Some(Box::new(res))
    }

    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let graph = parse_input(input);
        let res = find_paths(&graph, &Cave::Small("start"), &Cave::Small("end"), true).len();
        Some(Box::new(res))
    }
}

fn find_paths<'a>(graph: &Graph<Cave<'a>>, from: &Cave<'a>, to: &Cave<'a>, visit_twice: bool) -> Vec<Vec<Cave<'a>>> {
    let mut paths = Vec::new();
    let mut open = VecDeque::new();
    open.push_back(Path {
        closed: HashSet::from_iter(vec![from.clone()].iter().cloned()),
        path: vec![from.clone()],
        visit_twice: false
    });

    while !open.is_empty() {
        let curr = open.pop_front().unwrap();
        let node = curr.path.last().unwrap();
        
        if node == to {
            paths.push(curr.path);
            continue;
        }

        for neighbour in graph.neighbours(node) {
            if matches!(neighbour, Cave::Small(_)) && curr.closed.contains(neighbour) {
                if visit_twice && !curr.visit_twice && neighbour != from && neighbour != to {
                    let mut new_path = curr.clone();
                    new_path.path.push(neighbour.clone());
                    new_path.closed.insert(neighbour.clone());
                    new_path.visit_twice = true;
                    open.push_back(new_path);
                }
                continue;
            }
            let mut new_path = curr.clone();
            new_path.path.push(neighbour.clone());
            new_path.closed.insert(neighbour.clone());
            open.push_back(new_path);
        }
    }

    paths
}

#[derive(Debug, Clone)]
struct Path<'a> {
    closed: HashSet<Cave<'a>>,
    path: Vec<Cave<'a>>,
    visit_twice: bool
}

#[derive(Debug, Clone, PartialEq, Eq, Hash)]
enum Cave<'a> {
    Small(&'a str),
    Big(&'a str)
}

impl<'a> Cave<'a> {
    fn new(name: &str) -> Cave {
        if name.chars().all(|c| c.is_uppercase()) {
            return Cave::Big(name);
        }
        Cave::Small(name)
    }
}

fn parse_input(input: &Input) -> Graph<Cave> {
    let mut graph = Graph::new();
    for line in input.lines.iter() {
        if let [from, to] = line.split('-').collect::<Vec<&str>>()[..] {
            graph.connect(&Cave::new(from), &Cave::new(to));
        }
    }
    graph
}
