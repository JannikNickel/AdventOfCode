module Day01
open Input
open Common

[<Literal>]
let name = "Historian Hysteria"

let parseInput (input: Input) = 
    input.lines
    |> List.map Parsing.ints
    |> List.map (fun l -> (l[0], l[1]))
    |> List.unzip

let solveFirst (input: Input) = 
    input
    |> parseInput
    |> fun (l, r) -> (l |> List.sort, r |> List.sort)
    |> fun (l, r) -> List.map2 (fun x y -> abs (x - y)) l r
    |> List.sum

let solveSecond (input: Input) = 
    let (l, r) = input |> parseInput
    l |> List.sumBy (fun n -> n * (r |> List.filter ((=) n) |> List.length))
