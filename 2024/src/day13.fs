module Day13
open Input
open Common

[<Literal>]
let name = "Claw Contraption"

let pair (str: string) = 
    match str |> Parsing.numbers int64 with
    | [a; b] -> (a, b)
    | _ -> failwith ""

let parseInput (input: Input) = 
    input.lines
    |> List.split ((=) "")
    |> List.map (function
        | [a; b; res] -> (a |> pair, b |> pair, res |> pair)
        | _ -> failwith ""
    )

let det a b =
    (fst a) * (snd b) - (fst b) * (snd a)

let solveLgs (a: int64 * int64, b: int64 * int64, p: int64 * int64) = 
    // a.x * x + b.x * y = p.x
    // a.y * x + b.y * y = p.y
    // Cramer's rule
    let detD = det (fst a, fst b) (snd a, snd b)
    let detX = det (fst p, fst b) (snd p, snd b)
    let detY = det (fst a, fst p) (snd a, snd p)
    let x = detX / detD
    let y = detY / detD
    let xRes = (fst a) * x + (fst b) * y
    let yRes = (snd a) * x + (snd b) * y
    if (xRes, yRes) = p then (x, y) else (0, 0)

let solve (offset: int64) (input: Input) = 
    input
    |> parseInput
    |> List.sumBy (fun (a, b, p) -> 
        let (x, y) = solveLgs (a, b, (fst p + offset, snd p + offset))
        x * 3L + y
    )

let solveFirst (input: Input) = input |> solve 0L
let solveSecond (input: Input) = input |> solve 10000000000000L
