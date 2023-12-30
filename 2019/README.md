# AdventOfCode2019
My solutions for [Advent Of Code 2019](https://adventofcode.com/2019) in C to complete all puzzles for 50 ⭐.

## Arguments
```
--all #Run all days
--day <number> #Run the puzzle from the selected day
```
If no arguments are used, the application will calculate the last available puzzle.

## Structure
- Each puzzle is stored in a separate file which can be found with the links below
- Part 1 and 2 are implemented in a single `dayX.c` file to share code if possible
  - Each solution contains a `dayx_part1` function to solve the first part and `dayx_part2` for the second part
- Inputs are stored in `input/` with the files named `01.txt, 02.txt, ...` for each puzzle
  - The input is loaded into an input struct (`input.h`) which can be accessed as a raw string or lines in each solution file

## Solutions
- [Day 1](src/day01.c)
- [Day 2](src/day02.c)
- [Day 3](src/day03.c)
- [Day 4](src/day04.c)
- [Day 5](src/day05.c)
- [Day 6](src/day06.c)
- [Day 7](src/day07.c)
- [Day 8](src/day08.c)
- [Day 9](src/day09.c)
- [Day 10](src/day10.c)
- [Day 11](src/day11.c)
- [Day 12](src/day12.c)
- [Day 13](src/day13.c)
- [Day 14](src/day14.c)
- [Day 15](src/day15.c)
- [Day 16](src/day16.c)
- [Day 17](src/day17.c)
- [Day 18](src/day18.c)
- [Day 19](src/day19.c)
- [Day 20](src/day20.c)
- [Day 21](src/day21.c)
- [Day 22](src/day22.c)
- [Day 23](src/day23.c)
- [Day 24](src/day24.c)
- [Day 25](src/day25.c)