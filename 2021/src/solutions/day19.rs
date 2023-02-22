use super::Solution;
use crate::input::Input;
use crate::common::math::Vec3;
use std::collections::{HashSet, HashMap};

pub struct Day19;

impl Solution for Day19 {
    fn day(&self) -> i32 { 19 }
    fn name(&self) -> &'static str { "Beacon Scanner" }

    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let scanners = parse_input(input);
        let variants: HashMap<usize, Vec<Scanner>> = gen_variants(&scanners);
        let (_, beacons) = solve(&scanners, &variants);
        Some(Box::new(beacons.len()))
    }

    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let scanners = parse_input(input);
        let variants: HashMap<usize, Vec<Scanner>> = gen_variants(&scanners);
        let (origins, _) = solve(&scanners, &variants);
        let res = origins
            .iter()
            .enumerate()
            .flat_map(|(i, a)| origins
                .iter()
                .enumerate()
                .filter(move |(k, _)| *k > i)
                .map(|(_, b)| {
                    i32::abs(a.1.x - b.1.x) + i32::abs(a.1.y - b.1.y) + i32::abs(a.1.z - b.1.z)
                })
            ).max().unwrap();
        Some(Box::new(res))
    }
}

fn gen_variants(scanners: &Vec<Scanner>) -> HashMap<usize, Vec<Scanner>> {
    scanners.iter().skip(1).map(|scanner| {
        (scanner.index, (0..24).into_iter().map(|i| {
            Scanner {
                index: scanner.index,
                beacons: scanner.beacons.iter().map(|n| rotation_variant(i)(*n)).collect::<Vec<Vec3>>()
            }
        }).collect::<Vec<Scanner>>())
    }).collect()
}

fn solve(scanners: &[Scanner], variants: &HashMap<usize, Vec<Scanner>>) -> (HashMap<usize, Vec3>, HashSet<Vec3>) {
    let mut all_beacons: HashSet<Vec3> = HashSet::from_iter(scanners[0].beacons.clone());
    let mut closed: HashSet<usize> = HashSet::from_iter([scanners[0].index]);
    let mut origins = HashMap::new();

    'outer: while closed.len() != scanners.len() {
        for scanner in scanners {
            if closed.contains(&scanner.index) {
                continue;
            }

            for variant in &variants[&scanner.index] {
                let beacons = &variant.beacons;
                for beacon in beacons {
                    for target in &all_beacons {
                        let off = *target - *beacon;
                        if beacons.iter().filter(|n| all_beacons.contains(&(**n + off))).count() >= 12 {
                            for b in beacons {
                                all_beacons.insert(*b + off);
                            }
                            closed.insert(scanner.index);
                            origins.insert(scanner.index, off);
                            continue 'outer;
                        }
                    }
                }
            }
        }
    }
    (origins, all_beacons)
}

fn parse_input(input: &Input) -> Vec<Scanner> {
    let mut scanners = Vec::new();
    let mut beacons = Vec::new();

    for line in &input.lines {
        if line.starts_with("---") {
            if beacons.len() > 0 {
                scanners.push(Scanner { index: scanners.len(), beacons: beacons });
                beacons = Vec::new();
            }
        } else if !line.is_empty() {
            if let [x, y, z] = line.split(',').map(|n| n.parse::<i32>().unwrap()).collect::<Vec<i32>>()[..] {
                beacons.push(Vec3::new(x, y, z));
            }
        }
    }
    scanners.push(Scanner { index: scanners.len(), beacons: beacons });    
    scanners
}

#[derive(Debug, Clone)]
struct Scanner {
    index: usize,
    beacons: Vec<Vec3>,
}

fn rotation_variant(index: i32) -> impl Fn(Vec3) -> Vec3 {
    match index {
        0 => |p: Vec3 | Vec3::new(p.x, p.z, -p.y),
        1 => |p: Vec3 | Vec3::new(-p.z, p.x, -p.y),
        2 => |p: Vec3 | Vec3::new(-p.x, -p.z, -p.y),
        3 => |p: Vec3 | Vec3::new(p.z, -p.x, -p.y),
        4 => |p: Vec3 | Vec3::new(p.z, -p.y, p.x),
        5 => |p: Vec3 | Vec3::new(p.y, p.z, p.x),
        6 => |p: Vec3 | Vec3::new(-p.z, p.y, p.x),
        7 => |p: Vec3 | Vec3::new(-p.y, -p.z, p.x),
        8 => |p: Vec3 | Vec3::new(-p.y, p.x, p.z),
        9 => |p: Vec3 | Vec3::new(-p.x, -p.y, p.z),
        10 => |p: Vec3 | Vec3::new(p.y, -p.x, p.z),
        11 => |p: Vec3 | Vec3::new(p.x, p.y, p.z),
        12 => |p: Vec3 | Vec3::new(-p.z, -p.x, p.y),
        13 => |p: Vec3 | Vec3::new(p.x, -p.z, p.y),
        14 => |p: Vec3 | Vec3::new(p.z, p.x, p.y),
        15 => |p: Vec3 | Vec3::new(-p.x, p.z, p.y),
        16 => |p: Vec3 | Vec3::new(-p.x, p.y, -p.z),
        17 => |p: Vec3 | Vec3::new(-p.y, -p.x, -p.z),
        18 => |p: Vec3 | Vec3::new(p.x, -p.y, -p.z),
        19 => |p: Vec3 | Vec3::new(p.y, p.x, -p.z),
        20 => |p: Vec3 | Vec3::new(p.y, -p.z, -p.x),
        21 => |p: Vec3 | Vec3::new(p.z, p.y, -p.x),
        22 => |p: Vec3 | Vec3::new(-p.y, p.z, -p.x),
        23 => |p: Vec3 | Vec3::new(-p.z, -p.y, -p.x),
        _ => panic!()
    }
}
