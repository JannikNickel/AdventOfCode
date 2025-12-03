module Day03
open Input
open Common

[<Literal>]
let name = "Lobby"

let parseInput (input: Input) = 
    input.lines
    |> List.map Seq.toList
    |> List.map (List.map (fun n -> uint64 (n - '0')))

let find (size: int) (digits: uint64 list) = 
    let rec findMaxDigit digits count = 
        match count with
        | 0 -> 0UL
        | _ -> 
            let limitIdx = List.length digits - count
            let max = digits.[..limitIdx] |> List.max
            let leftIdx = digits |> List.findIndex ((=) max)
            let shift = pown 10UL (count - 1)
            max * shift + (findMaxDigit (digits.[(leftIdx + 1) ..]) (count - 1))

    findMaxDigit digits size

let solve (selector: uint64 list -> uint64) (input: Input) = 
    input
    |> parseInput
    |> List.map selector
    |> List.sum

let solveFirst (input: Input) = input |> solve (find 2)
let solveSecond (input: Input) = input |> solve (find 12)
