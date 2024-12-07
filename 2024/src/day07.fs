module Day07
open Input
open Common

[<Literal>]
let name = "Bridge Repair"

let parseInput (input: Input) = 
    input.lines
    |> List.map (Parsing.numbers uint64)
    |> List.map (function head :: tail -> (head, tail) | _ -> (0UL, []))

let permsWithRep (len: int) (items: 'a list) = 
    let rec accumulate n acc = 
        match n with
        | 0 -> seq { yield acc }
        | _ -> seq { for i in items do yield! accumulate (n - 1) (i :: acc) }
    accumulate len []

let concatDigits a b = 
    let digits = int (System.Math.Log10 (float b) + 1.0)
    a * (pown 10UL digits) + b

let solveEquation (ops: (uint64 -> uint64 -> uint64) list) (values: uint64 list) = 
    match values with
    | first :: tail -> List.fold2 (fun acc n op -> op acc n) first tail ops
    | _ -> 0UL

let canSolveEquation ops (result: uint64, values: uint64 list) = 
    ops
    |> permsWithRep (values.Length - 1)
    |> Seq.exists (fun ops -> (values |> solveEquation ops) = result)

let solve ops (input: Input) = 
    input
    |> parseInput
    |> List.filter (canSolveEquation ops)
    |> List.sumBy fst

let solveFirst (input: Input) = input |> solve [(+); (*)]
let solveSecond (input: Input) = input |> solve [(+); (*); concatDigits]