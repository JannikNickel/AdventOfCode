module Day08
open Input
open Common

[<Literal>]
let name = "Playground"

let parseInput (input: Input) = 
    input.lines |> List.map (Parsing.numbers int64 >> Vec3.ofLst)

let findSets (cond: Set<Vec3> list * int -> bool) (nodes: Vec3 list) = 
    let allEdges = nodes |> List.uniquePairs |> List.sortBy ((<||) Vec3.dst)

    let rec loop (sets: Set<Vec3> list) (edges: (Vec3 * Vec3) list) (iteration: int) (prevEdge: (Vec3 * Vec3) option) =
        match edges with
        | edge :: remaining -> 
            if cond (sets, iteration) then
                (sets, prevEdge)
            else
                let aSet = sets |> List.find (Set.contains (fst edge))
                let bSet = sets |> List.find (Set.contains (snd edge))
                let newSets = 
                    if aSet <> bSet then
                        (Set.union aSet bSet) :: (sets |> List.filter (fun s -> s <> aSet && s <> bSet))
                    else
                        sets
                loop newSets remaining (iteration + 1) (Some edge)
        | _ -> (sets, prevEdge)

    loop (nodes |> List.map Set.singleton) allEdges 0 None

let solveFirst (input: Input) = 
    input
    |> parseInput
    |> (findSets (snd >> (=) 1000) >> fst)
    |> List.map Set.count
    |> List.sortDescending
    |> List.take 3
    |> List.reduce (*)

let solveSecond (input: Input) = 
    input
    |> parseInput
    |> (findSets (fst >> List.length >> (=) 1) >> snd)
    |> function Some (a, b) -> a.x * b.x | _ -> 0L
