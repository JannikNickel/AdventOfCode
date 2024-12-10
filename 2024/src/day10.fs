module Day10
open Input
open Common

[<Literal>]
let name = "Hoof It"

let dirs = [ Vec2(-1, 0); Vec2(0, 1); Vec2(1, 0); Vec2(0, -1) ]

let parseInput (input: Input) = 
    let map = input.lines |> Parsing.map2d Vec2.ofTpl (fun n -> int n - int '0') |> Map.ofSeq
    let starts = map |> Map.keys |> Seq.filter (fun n -> map |> Map.find n = 0) |> List.ofSeq
    map, starts

let findPaths (map: Map<Vec2, int>) (start: Vec2) = 
    let size = map |> Map.keys |> Seq.max |> (+) Vec2.one
    let rec dfs (curr: Vec2) (closed: Set<Vec2>) = 
        if (map |> Map.find curr) = 9 then [curr]
        elif closed |> Set.contains curr then []
        else
            let value = map |> Map.find curr
            let closed = closed |> Set.add curr
            dirs
            |> List.map (fun dir -> curr + dir)
            |> List.filter (fun n -> Vec2.inBounds size n)
            |> List.filter (fun n -> map |> Map.find n = (value + 1))
            |> List.collect (fun n -> dfs n closed)

    dfs start Set.empty

let findDestinations map start =
    findPaths map start |> List.distinct

let solve fn input = 
    let map, starts = input |> parseInput
    starts |> List.sumBy (fun start -> fn map start |> List.length)

let solveFirst (input: Input) = input |> solve findDestinations
let solveSecond (input: Input) = input |> solve findPaths
