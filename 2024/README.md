# AdventOfCode2024
My solutions for [Advent Of Code 2024](https://adventofcode.com/2024) in F# 8/.NET 8 to complete all puzzles for 50 ⭐.

## Parameters
```
-a, --all #Run all days
-d, --day <number> #Run the puzzle of the selected day (can be used multiple times to run more than one solution)
```
If no parameters are used, the application will calculate the last available puzzle.

## Structure
- Each puzzle is solved in a separate file which can be found with the links before (or in the `src/` folder)
- Part 1 and 2 are implemented in a single `dayX.fs` file to share code if possible
    - Each solution contains a `solveFirst` function to solve the first part and `solveSecond` for the second part
- Inputs are stored in `input/` with the files named `01.txt, 02.txt, ...` for each puzzle
    - The input is loaded in an input class (`input.fs`) which can be accessed as raw string or line list in each solution function

## Solutions
- [Day 1](src/day01.fs)
- [Day 2](src/day02.fs)
- [Day 3](src/day03.fs)
- [Day 4](src/day04.fs)
- [Day 5](src/day05.fs)
- [Day 6](src/day06.fs)
- [Day 7](src/day07.fs)
- [Day 8](src/day08.fs)
- [Day 9](src/day09.fs)
- [Day 10](src/day10.fs)
- [Day 11](src/day11.fs)
- [Day 12](src/day12.fs)
- [Day 13](src/day13.fs)
- [Day 14](src/day14.fs)
- [Day 15](src/day15.fs)
- [Day 16](src/day16.fs)
- [Day 17](src/day17.fs)
- [Day 18](src/day18.fs)
- [Day 19](src/day19.fs)
- [Day 20](src/day20.fs)
- [Day 21](src/day21.fs)
- [Day 22](src/day22.fs)
- [Day 23](src/day23.fs)
- [Day 24](src/day24.fs)
- [Day 25](src/day25.fs)
