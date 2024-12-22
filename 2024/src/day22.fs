module Day22
open Input
open Common

[<Literal>]
let name = "Monkey Market"

let nextSecret (secret: uint64) = 
    let secret = ((secret * 64UL) ^^^ secret) % 16777216UL
    let secret = ((secret / 32UL) ^^^ secret) % 16777216UL
    let secret = ((secret * 2048UL) ^^^ secret) % 16777216UL
    secret

let findSecrets (iterations: int) (initial: uint64) = 
    List.range iterations
    |> List.fold (fun (secrets) _ -> 
        (secrets |> List.head |> nextSecret) :: secrets
    ) [initial]
    |> List.rev

let packBytes (a: uint8) (b: uint8) (c: uint8) (d: uint8) = 
    ((uint32 a) <<< 24) ||| ((uint32 b) <<< 16) ||| ((uint32 c) <<< 8) ||| (uint32 d)

let allSeqs (prices: int list) = 
    prices
    |> List.windowed 5
    |> List.map (fun n -> 
        let diffs = n |> List.windowed 2 |> List.map (function [a; b] -> (b - a) |> uint8 | _ -> 0uy)
        let diffPack = packBytes diffs[0] diffs[1] diffs[2] diffs[3]
        diffPack, n |> List.last
    )
    |> List.distinctBy fst

let solveFirst (input: Input) = 
    input.lines
    |> List.map uint64
    |> List.map (findSecrets 2000)
    |> List.sumBy (List.rev >> List.head)

let solveSecond (input: Input) = 
    input.lines
    |> List.map uint64
    |> List.map (findSecrets 2000)
    |> List.map (List.map (fun n -> n % 10UL |> int))
    |> List.map allSeqs
    |> Seq.concat
    |> Seq.groupBy fst
    |> Seq.map (snd >> Seq.sumBy snd)
    |> Seq.max
