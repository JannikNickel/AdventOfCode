module Day06
open Input
open Common

[<Literal>]
let name = "Guard Gallivant"

let rotate (pos: Vec2) = Vec2(-pos.y, pos.x)

let rec simulate (map: Map<Vec2, char>) (pos: Vec2) (dir: Vec2) (path: (Vec2 * Vec2) Set) =
    if path |> Set.contains (pos, dir) then
        (path, true)
    else
        let nextPos = pos + dir
        let path = path |> Set.add (pos, dir)
        let ahead = map |> Map.tryFind nextPos
        match ahead with
        | Some '#' -> simulate map pos (dir |> rotate) path
        | Some '.' | Some '^' -> simulate map nextPos dir path
        | _ -> (path, false)

let solvePath (input: Input) = 
    let map = input.lines |> Parsing.map2d Vec2.ofTpl id |> Map.ofSeq
    let start = map |> Map.findKey (fun _ value -> value = '^')
    let res, _ = simulate map start (Vec2(0, -1)) Set.empty
    (map, start, res |> Seq.distinctBy fst)

let solveFirst (input: Input) = 
    let _, _, res = solvePath input
    res |> Seq.length

let solveSecond (input: Input) = 
    let map, start, res = solvePath input
    res
    |> Seq.filter (fun (pos, _) -> pos <> start)
    |> Array.ofSeq
    |> Array.Parallel.map (fun (pos, _) ->
        let nMap = map |> Map.change pos (function Some _ -> Some '#' | _ -> None)
        simulate nMap start (Vec2(0, -1)) Set.empty |> snd
    )
    |> Seq.filter ((=) true)
    |> Seq.length
