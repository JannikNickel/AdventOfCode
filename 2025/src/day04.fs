module Day04
open Input
open Common

[<Literal>]
let name = "Printing Department"

let dirs = Vec2.dirs @ Vec2.dirsDiag

let parseInput (input: Input) = input.lines |> Parsing.map2d Vec2.ofTpl id |> Map.ofSeq

let countPaperRolls (map: Map<Vec2, char>) =
    map
    |> Map.values
    |> Seq.filter ((=) '@')
    |> Seq.length

let countNeighbors (map: Map<Vec2, char>) (pos: Vec2) (c: char) = 
    dirs |> List.sumBy (fun dir ->
        if map |> Map.valueOrDefault (pos + dir) ' ' = c then 1 else 0
    )

let removeFromMap (map: Map<Vec2, char>) (nLimit: int) = 
    map |> Map.map (fun pos v ->
        if v = '@' && countNeighbors map pos '@' < nLimit then '.' else v
    )

let simulateStep (map: Map<Vec2, char>) =
    let nextMap = removeFromMap map 4
    let removed = countPaperRolls map - countPaperRolls nextMap
    if removed > 0 then Some (removed, nextMap) else None

let solveFirst (input: Input) = 
    input
    |> parseInput
    |> simulateStep
    |> Option.map fst
    |> Option.defaultValue 0

let solveSecond (input: Input) = 
    input
    |> parseInput
    |> Seq.unfold simulateStep
    |> Seq.sum
