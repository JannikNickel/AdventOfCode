module Day25
open Input
open Common

[<Literal>]
let name = "Code Chronicle"

type Schematic = Lock | Key

let parseInput (input: Input) = 
    input.lines
    |> List.split ((=) "")
    |> List.map (fun n -> 
        let lock = n |> List.head |> Seq.forall ((=) '#')
        let ref = if lock then '.' else '#'
        let depths = 
            n
            |> List.map List.ofSeq
            |> List.transpose
            |> List.map (List.filter ((=) ref) >> List.length)
        if lock then (Lock, depths) else (Key, depths)
    )

let matches (lock: int list) (key: int list) = 
    (lock, key)
    ||> List.zip
    |> List.forall (fun (lDepth, kDepth) -> lDepth >= kDepth)

let solveFirst (input: Input) = 
    let schematics = input |> parseInput
    let locks = schematics |> List.filter (fst >> (=) Lock) |> List.map snd
    let keys = schematics |> List.filter (fst >> (=) Key) |> List.map snd
    (locks, keys)
    ||> List.allPairs
    |> List.filter (fun (lock, key) -> matches lock key)
    |> List.length

let solveSecond (input: Input) = 
    //Day 25 only has 1 puzzle
    null :> obj
