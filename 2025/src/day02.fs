module Day02
open Input
open Common

[<Literal>]
let name = "Gift Shop"

let parseInput (input: Input) = 
    input.raw
    |> Parsing.numbers int64
    |> List.map abs
    |> List.chunkBySize 2

let isInvalid (num: int64) = 
    let str = string num
    let mid = (String.length str) / 2
    str.[.. (mid - 1)] = str.[mid..]

let isInvalid2 (num: int64) = 
    let str = string num
    let strLen = String.length str
    let rec checkSeq = function
        | 0 -> false
        | len -> 
            let repeats = strLen / len
            if strLen % repeats = 0 && (str.[.. (len - 1)] |> String.replicate repeats) = str then
                true
            else
                checkSeq (len - 1)

    checkSeq (strLen / 2)

let solve (filter: int64 -> bool) (input: Input) = 
    input
    |> parseInput
    |> Seq.collect (function [a; b] -> seq { a..b } | _ -> failwith "")
    |> Seq.filter filter
    |> Seq.sum

let solveFirst (input: Input) = input |> solve isInvalid
let solveSecond (input: Input) = input |> solve isInvalid2
