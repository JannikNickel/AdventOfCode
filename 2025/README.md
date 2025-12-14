# AdventOfCode2024
My solutions for [Advent Of Code 2025](https://adventofcode.com/2025) in F# 9/.NET 9 to complete all puzzles for 24 ⭐.

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
