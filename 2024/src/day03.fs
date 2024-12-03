module Day03
open Input
open Common

[<Literal>]
let name = "Mull It Over"

let (|Prefix|_|) (p: string) (s: string) = if s.StartsWith p then Some () else None

let solveFirst (input: Input) = 
    input.raw
    |> Parsing.matches @"mul\(\d+,\d+\)"
    |> List.map Parsing.ints
    |> List.sumBy (function [a; b] -> a * b | _ -> 0)

let solveSecond (input: Input) = 
    input.raw
    |> Parsing.matches @"mul\(\d+,\d+\)|do(n't)?\(\)"
    |> List.fold (fun (on, sum) op -> 
        match op, on with
        | Prefix "mul", true -> (on, sum + (op |> Parsing.ints |> List.reduce (*)))
        | Prefix "don't", _ -> (false, sum)
        | Prefix "do", _ -> (true, sum)
        | _ -> (on, sum)
    ) (true, 0)
    |> snd
