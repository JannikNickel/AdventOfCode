# AdventOfCode2022
My solutions for [Advent Of Code 2020](https://adventofcode.com/2020) in C++20 to complete all puzzles for 50 ⭐.

## Arguments
```
--all #Run all days
--day <number> #Run the puzzle from the selected day
```
If no arguments are used, the application will calculate the last available puzzle.

## Structure
- Each puzzle is stored in a separate file which can be found with the links below
- Part 1 and 2 are implemented in a single `DayX.h + DayX.cpp` file to share code if possible
  - Each solution contains a `SolveFirst` method to solve the first part and `SolveSecond` for the second part
- Inputs are stored in 2020/input/ with the files named `01.txt, 02.txt, ...` for each puzzle
  - The input is loaded into an input struct (`input.h`) which can be accessed as a raw string or line vector in each solution struct

## Solutions
- [Day 1](src/day01.cpp)
- [Day 2](src/day02.cpp)
- [Day 3](src/day03.cpp)
- [Day 4](src/day04.cpp)
- [Day 5](src/day05.cpp)
- [Day 6](src/day06.cpp)
- [Day 7](src/day07.cpp)
- [Day 8](src/day08.cpp)
- [Day 9](src/day09.cpp)
- [Day 10](src/day10.cpp)
- [Day 11](src/day11.cpp)
- [Day 12](src/day12.cpp)
- [Day 13](src/day13.cpp)
- [Day 14](src/day14.cpp)
- [Day 15](src/day15.cpp)
- [Day 16](src/day16.cpp)
- [Day 17](src/day17.cpp)
- [Day 18](src/day18.cpp)
- [Day 19](src/day19.cpp)
- [Day 20](src/day20.cpp)
- [Day 21](src/day21.cpp)
- [Day 22](src/day22.cpp)
- [Day 23](src/day23.cpp)
- [Day 24](src/day24.cpp)
- [Day 25](src/day25.cpp)