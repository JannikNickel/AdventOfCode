module Day11
open Input
open Common

[<Literal>]
let name = "Plutonian Pebbles"

let digits (n: uint64) =
    int (floor (log10 (float n)) + 1.0)

let splitNum (n: uint64) = 
    let len = n |> digits
    let a = n / (pown 10UL (len / 2))
    let b = n - a * (pown 10UL (len / 2))
    a, b

let step (n: uint64) = 
    match n with
    | n when n = 0UL -> [1UL]
    | n when (n |> digits) % 2 = 0 -> 
        let l, r = splitNum n
        [l; r]
    | n -> [n * 2024UL]
    |> Seq.groupBy id
    |> Seq.map (fun (k, t) -> (k, t |> Seq.length |> uint64))

let rec simSteps (steps: int) (mappings: Map<uint64, uint64>) = 
    if steps <= 0 then mappings
    else
        mappings
        |> Map.map (fun key value -> step key |> Seq.map (fun (k, v) -> (k, v * value)))
        |> Map.toSeq
        |> Seq.collect snd
        |> Seq.fold (fun acc (k, v) -> 
            acc |> Map.change k (function
                | Some n -> Some (n + v)
                | None -> Some v
            )
        ) Map.empty
        |> simSteps (steps - 1)

let solve steps (input: Input) = 
    input.raw
    |> Parsing.ints
    |> List.map (fun n -> (uint64 n, 1UL))
    |> Map.ofSeq
    |> simSteps steps
    |> Map.values
    |> Seq.sum

let solveFirst (input: Input) = input |> solve 25
let solveSecond (input: Input) = input |> solve 75
