use std::{path::Path, process, time::Instant};
use clap::Parser;

use input::Input;
use solutions::*;

use colored::Colorize;

pub mod common;
pub mod input;
pub mod solutions;

fn main() {
    let args = Args::parse();

    match args {
        Args { all: true, day: _ } => {
            all_solutions().iter().for_each(|n| run_solution(n))
        },
        Args { all: _, day: Some(actual_day) } => {
            run_solution(&all_solutions().get((actual_day - 1) as usize).unwrap_or_else(|| {
                eprintln!("Solution for day {} doesnt exist!", actual_day);
                process::exit(1);
            }))
        },
        _ => {
            run_solution(&all_solutions().last().unwrap_or_else(|| {
                eprintln!("No solutions!");
                process::exit(1);
            }))
        }
    }
}

fn print_light(text: &str) {
    println!("{}", text.truecolor(100, 100, 100));
}

fn print(text: &str) {
    println!("{}", text);
}

fn day_to_formatted(day: i32) -> String {
    if day < 10 {
        return "0".to_string() + &day.to_string();
    }
    day.to_string()
}

fn run_solution(solution: &Box<dyn Solution>) {
    print_light(&format!("Preparing Solution {} - {}", day_to_formatted(solution.day()), solution.name()));

    let s_path = format!("input/{}.txt", day_to_formatted(solution.day() - 1));
    let path = Path::new(&s_path);
    let input = match Input::build(path) {
        Ok(result) => result,
        Err(err) => {
            eprintln!("{:#?}", err);
            process::exit(1);
        }
    };

    run_part(1, solution, &input);
    run_part(2, solution, &input);
}

fn run_part(part: i32, solution: &Box<dyn Solution>, input: &Input) {
    print_light(&format!("Solving Part {}...", part));
    let start = Instant::now();

    let result = match part {
        1 => solution.run1(input),
        2 => solution.run2(input),
        _ => {
            eprintln!("Could not solve! Expected part to be 1 or 2!");
            return;
        }
    };    
    let duration = start.elapsed();
    print_light(&format!("Solved Part {} ({}ns)", part, duration.as_nanos()));

    print_light("Result:");
    print(&format!("{}", result.unwrap_or(Box::new("-"))));
    println!();
}

#[derive(Parser, Debug)]
struct Args {
    #[arg(short, long)]
    day: Option<i32>,

    #[arg(short, long, action)]
    all: bool
}
