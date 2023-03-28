# AdventOfCode2021
My solutions for [Advent Of Code 2021](https://adventofcode.com/2021) in Rust 1.67.1 to complete all puzzles for 50 ⭐.

## Parameters
```
--all #Run all days
--day <number> #Run the puzzle from the selected day
```
If no parameters are used, the application will calculate the last available puzzle.

## Structure
- Each puzzle is stored in a separate file which can be found with the links below
- Part 1 and 2 are implemented in a single file to share code if possible
  - Each solution source file contains a `run1` method to solve the first part and `run2` for the second part
- Inputs are stored in `input/` with the files named `01.txt, 02.txt, ...` for each puzzle
  - The input is loaded into an `Input` struct which can be accessed as a raw string or line array in each solution
- `common/` contains some modules like data structures which are used in various solutions

## Solutions
- [Day 1](src/solutions/day01.rs)
- [Day 2](src/solutions/day02.rs)
- [Day 3](src/solutions/day03.rs)
- [Day 4](src/solutions/day04.rs)
- [Day 5](src/solutions/day05.rs)
- [Day 6](src/solutions/day06.rs)
- [Day 7](src/solutions/day07.rs)
- [Day 8](src/solutions/day08.rs)
- [Day 9](src/solutions/day09.rs)
- [Day 10](src/solutions/day10.rs)
- [Day 11](src/solutions/day11.rs)
- [Day 12](src/solutions/day12.rs)
- [Day 13](src/solutions/day13.rs)
- [Day 14](src/solutions/day14.rs)
- [Day 15](src/solutions/day15.rs)
- [Day 16](src/solutions/day16.rs)
- [Day 17](src/solutions/day17.rs)
- [Day 18](src/solutions/day18.rs)
- [Day 19](src/solutions/day19.rs)
- [Day 20](src/solutions/day20.rs)
- [Day 21](src/solutions/day21.rs)
- [Day 22](src/solutions/day22.rs)
- [Day 23](src/solutions/day23.rs)
- [Day 24](src/solutions/day24.rs)
- [Day 25](src/solutions/day25.rs)