module Day01
open Input
open Common

[<Literal>]
let name = "Secret Entrance"

let (%!) a b = if a < 0 then (a % b + b) % b else a % b

let parseInput (input: Input) = 
    input.lines
    |> List.map (fun n -> (int n.[1..]) * (if n.[0] = 'L' then -1 else 1))

let solveFirst (input: Input) = 
    input
    |> parseInput
    |> List.scan (fun acc n -> (acc + n) %! 100) 50
    |> List.filter ((=) 0)
    |> List.length

let solveSecond (input: Input) = 
    let rec move curr count = function
        | head :: tail -> 
            let next = curr + head
            let crosses = abs (next / 100) + (if (sign next) <> (sign curr) && (sign curr) <> 0 then 1 else 0)
            move (next %! 100) (count + crosses) tail
        | [] -> count

    move 50 0 (parseInput input)
