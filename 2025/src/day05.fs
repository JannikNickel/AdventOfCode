module Day05
open Input
open Common

[<Literal>]
let name = "Cafeteria"

let parseInput (input: Input) =
    input.lines
    |> List.split ((=) "")
    |> function
        | [ fresh; ingredients ] -> 
            let fresh = fresh |> List.map (Parsing.uNumbers uint64 >> (function [a; b] -> (a, b) | _ -> failwith ""))
            (fresh, ingredients |> List.map uint64)
        | _ -> failwith ""

let solveFirst (input: Input) = 
    let (fresh, ingredients) = parseInput input
    ingredients
    |> List.filter (fun n -> fresh |> List.exists (fun (a, b) -> a <= n && n <= b))
    |> List.length

let solveSecond (input: Input) = 
    let sortedFresh = input |> parseInput |> fst |> List.sortBy fst
    let (fstStart, fstEnd) = sortedFresh |> List.head

    sortedFresh
    |> List.fold (fun (currStart, currEnd, result) (first, last) ->
        if first <= currEnd then
            (currStart, max currEnd last, result)
        else
            (first, last, (currStart, currEnd) :: result)
    ) (fstStart, fstEnd, [])
    |> fun (first, last, result) -> (first, last) :: result
    |> List.sumBy (fun (a, b) -> b - a + 1UL)
