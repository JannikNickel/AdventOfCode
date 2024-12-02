module Day02
open Input
open Common

[<Literal>]
let name = "Red-Nosed Reports"

let isSafePart (op: int -> int -> bool) levels = 
    levels
    |> List.pairwise
    |> List.forall (fun (a, b) -> (op a b) && (abs (a - b) <= 3))

let isSafe levels =
    isSafePart (<) levels || isSafePart (>) levels

let isSafeWithoutOne levels = 
    let len = List.length levels - 1
    [| 0..len |] |> Array.exists (fun i -> isSafe (levels |> List.removeAt i))

let solve (input: Input) filter = 
    input.lines
    |> List.map Parsing.ints
    |> List.filter filter
    |> List.length

let solveFirst (input: Input) = solve input isSafe
let solveSecond (input: Input) = solve input isSafeWithoutOne
