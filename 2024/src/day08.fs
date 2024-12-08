module Day08
open Input
open Common

[<Literal>]
let name = "Resonant Collinearity"

let frequencies = [ '0' .. '9' ] @ [ 'A' .. 'Z' ] @ [ 'a' .. 'z' ]

let parseInput (input: Input) = 
    input.lines
    |> Parsing.map2d Vec2.ofTpl id

let inBounds (size: Vec2) (pos: Vec2) =
    pos.x >= 0 && pos.y >= 0 && pos.x < size.x && pos.y < size.y;

let findAntinodes (steps: int list) (size: Vec2) (antennas: Vec2 list) = 
    antennas
    |> List.pairs
    |> List.filter (fun (a, b) -> a <> b)
    |> List.collect (fun (a, b) -> [ for i in steps -> b + (b - a) * i ])
    |> List.filter (inBounds size)

let solve (harmonics: bool) (input: Input) = 
    let map = input |> parseInput |> List.ofSeq
    let size = (map |> Seq.map fst |> Seq.max) + Vec2.one
    let steps = if harmonics then [ 0 .. (max size.x size.y) ] else [ 1 ]
    frequencies
    |> Seq.map (fun freq ->
        map
        |> List.filter (fun (_, value) -> value = freq)
        |> List.map fst
        |> List.ofSeq
    )
    |> Seq.filter (fun n -> (n |> List.length) > 0)
    |> Seq.collect (findAntinodes steps size)
    |> Seq.distinct
    |> Seq.length

let solveFirst (input: Input) = input |> solve false
let solveSecond (input: Input) = input |> solve true
