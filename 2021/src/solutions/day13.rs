use std::collections::HashSet;
use std::ops::Deref;

use super::Solution;
use crate::input::Input;
use crate::common::math::Vec2;

pub struct Day13;

impl Solution for Day13 {
    fn day(&self) -> i32 { 13 }
    fn name(&self) -> &'static str { "Transparent Origami" }

    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let (points, folds) = parse_input(input);
        let res = fold(&points, &folds[0]).len();
        Some(Box::new(res))
    }

    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let (mut points, folds) = parse_input(input);
        folds.iter().for_each(|n| points = fold(&points, n));
        let res = points_to_string(&points);
        Some(Box::new(res))
    }
}

fn points_to_string(points: &HashSet<Vec2>) -> String {
    let width = points.iter().map(|n| n.x as usize).max().unwrap() + 1;
    let height = points.iter().map(|n| n.y as usize).max().unwrap() + 1;
    let mut chars = vec![vec![' '; width]; height];

    for p in points {
        chars[p.y as usize][p.x as usize] = '#';
    }
    let mut res = String::new();
    chars.iter().for_each(|n| {
        res += String::from_iter(n).deref();
        res += "\n";
    });

    res
}

fn fold(points: &HashSet<Vec2>, fold: &Fold) -> HashSet<Vec2> {
    match fold {
        Fold::X(v) => wrap_x(points, *v),
        Fold::Y(v) => wrap_y(points, *v)
    }
}

fn wrap_x(points: &HashSet<Vec2>, pos: i32) -> HashSet<Vec2> {
    let mut new = HashSet::new();
    for p in points {
        if p.x > pos {
            let nx = pos - (p.x - pos);
            if nx >= 0 && nx < pos {
                new.insert(Vec2::new(nx, p.y));
            };
        } else {
            new.insert(*p);
        }
    }
    new
}

fn wrap_y(points: &HashSet<Vec2>, pos: i32) -> HashSet<Vec2> {
    let mut new = HashSet::new();
    for p in points {
        if p.y > pos {
            let ny = pos - (p.y - pos);
            if ny >= 0 && ny < pos {
                new.insert(Vec2::new(p.x, ny));
            };
        } else {
            new.insert(*p);
        }
    }
    new
}

#[derive(Debug, Clone, Copy, PartialEq)]
enum Fold {
    X(i32),
    Y(i32)
}

fn parse_input(input: &Input) -> (HashSet<Vec2>, Vec<Fold>) {
    let mut points = HashSet::new();
    let mut folds = Vec::new();

    for line in &input.lines {
        if line.starts_with("fold") {
            if let [axis, pos] = line.replace("fold along ", "").split('=').collect::<Vec<&str>>()[..] {
                folds.push(match (axis, pos) {
                    ("x", p) => Fold::X(p.parse::<i32>().unwrap()),
                    ("y", p) => Fold::Y(p.parse::<i32>().unwrap()),
                    _ => panic!()
                });
            }
        } else if let [x, y] = line.split(',').collect::<Vec<&str>>()[..] {
            points.insert(Vec2::new(x.parse::<i32>().unwrap(), y.parse::<i32>().unwrap()));
        }
    }

    (points, folds)
}
