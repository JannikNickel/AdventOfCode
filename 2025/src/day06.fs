module Day06
open Input
open Common

[<Literal>]
let name = "Trash Compactor"

let parseInput<'T> (numMapping: string -> 'T) (input: Input) = 
    let nums = input.lines |> List.take (List.length input.lines - 1) |> List.map numMapping
    let ops = input.lines |> List.last |> Seq.filter ((<>) ' ') |> List.ofSeq
    (nums, ops)

let charsToNum (chars: char list) = 
    chars
    |> List.filter ((<>) ' ')
    |> List.map (fun c -> int64 c - int64 '0') 
    |> List.fold (fun acc n -> acc * 10L + n) 0L

let solve (nums: int64 list list) (ops: char list) = 
    (List.zip nums ops)
    |> List.sumBy (fun (nums, op) ->
        match op with
        | '+' -> List.sum nums
        | '*' -> List.reduce (*) nums
        | _ -> 0L
    )

let solveFirst (input: Input) = 
    let (nums, ops) = parseInput (Parsing.numbers int64) input
    solve (nums |> List.transpose) ops

let solveSecond (input: Input) = 
    let (nums, ops) = parseInput List.ofSeq input
    nums
    |> List.transpose
    |> List.split (List.forall ((=) ' '))
    |> List.map (List.map charsToNum)
    |> fun groups -> solve groups ops
