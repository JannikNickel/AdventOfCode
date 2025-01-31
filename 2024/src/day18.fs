﻿module Day18
open Input
open Common

[<Literal>]
let name = "RAM Run"

let size = Vec2(70, 70)
let dirs = [ Vec2(-1, 0); Vec2(0, 1); Vec2(1, 0); Vec2(0, -1) ]

let parseInput (input: Input) = 
    input.lines
    |> List.map (Parsing.ints >> (function [a; b] -> Vec2(a, b) | _ -> Vec2(0, 0)))

let rec bfs (obstacles: Set<Vec2>) (dest: Vec2) (closed: Set<Vec2>) (queue: (Vec2 * int) list) = 
    let bounds = size + Vec2.one
    match queue with
    | (curr, depth) :: tail -> 
        if curr = dest then
            depth
        else
            let queue = tail
            if closed |> Set.contains curr then
                bfs obstacles dest closed queue
            else
                let closed = closed |> Set.add curr
                let next = 
                    dirs
                    |> List.map (fun n -> (curr + n, depth + 1))
                    |> List.filter (fst >> (Vec2.inBounds bounds))
                    |> List.filter (fst >> (fun n -> obstacles |> Set.contains n |> not))
                let queue = queue @ next
                bfs obstacles dest closed queue
    | _ -> -1

let findDepth (bytes: Vec2 list) (dropAmount: int) = 
    let map = bytes[ .. (dropAmount - 1) ] |> Set.ofList
    bfs map size Set.empty [(Vec2(0, 0), 0)]

let rec findMinBlocked (bytes: Vec2 list) (dropRange: int * int) = 
    match dropRange with
    | (low, high) when high - low <= 1 -> 
        if findDepth bytes low = -1 then low else high
    | (low, high) -> 
        let center = low + (high - low) / 2
        if findDepth bytes center = -1 then
            findMinBlocked bytes (low, center)
        else
            findMinBlocked bytes (center, high)

let solveFirst (input: Input) = 
    let bytes = input |> parseInput
    findDepth bytes 1024

let solveSecond (input: Input) = 
    let bytes = input |> parseInput
    let min = findMinBlocked bytes (0, bytes |> List.length)
    sprintf "%d,%d" bytes[min - 1].x bytes[min - 1].y
