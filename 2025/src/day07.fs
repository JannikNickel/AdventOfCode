module Day07
open Input
open Common

[<Literal>]
let name = "Laboratories"

let parseInput (input: Input) = 
    let map = input.lines |> Parsing.map2d Vec2.ofTpl id
    let height = map |> Seq.map (fst >> _.y) |> Seq.max |> (+) 1
    let start = map |> Seq.find (snd >> (=) 'S') |> fst
    (map |> Map.ofSeq, height, start)

let rec countSplits (map: Map<Vec2, char>) (mapHeight: int) (beams: Vec2 list) (splits: int) = 
    let currHeight = beams |> List.head |> _.y
    if currHeight >= mapHeight then
        splits
    else
        let newBeams, newSplits = 
            beams
            |> List.fold (fun (accBeams, accSplits) beam -> 
                let below = beam + Vec2(0, 1)
                match map |> Map.valueOrDefault below '.' with
                | '^' -> ((below + Vec2.left) :: (below + Vec2.right) :: accBeams, accSplits + 1)
                | _ -> (below :: accBeams, accSplits)
            ) ([], splits)
        countSplits map mapHeight (newBeams |> List.distinct) newSplits

let rec countTimelines (map: Map<Vec2, char>) (mapHeight: int) (beams: (Vec2 * int64) list) = 
    let currHeight = beams |> List.head |> (fst >> _.y)
    if currHeight >= mapHeight then
        beams |> List.sumBy snd
    else
        let newBeams = 
            beams 
            |> List.collect (fun (pos, timelines) -> 
                let below = pos + Vec2(0, 1)
                match map |> Map.valueOrDefault below '.' with
                | '^' -> [ (below + Vec2.left, timelines); (below + Vec2.right, timelines) ]
                | _ -> [ (below, timelines) ]
            )
            |> List.groupBy fst
            |> List.map (fun (pos, times) -> (pos, times |> List.sumBy snd))
        countTimelines map mapHeight newBeams

let solveFirst (input: Input) = 
    let (map, height, start) = parseInput input
    countSplits map height [ start ] 0

let solveSecond (input: Input) = 
    let (map, height, start) = parseInput input
    countTimelines map height [ (start, 1) ]
