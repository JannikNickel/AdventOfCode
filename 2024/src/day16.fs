module Day16
open Input
open Common
open System.Collections.Generic

[<Literal>]
let name = "Reindeer Maze"

type State = {
    pos: Vec2
    dir: Vec2
    cost: int
    prev: State option }

let parseMap (input: Input) = 
    let map = input.lines |> Parsing.map2d Vec2.ofTpl id |> Map.ofSeq
    let start = map |> Map.findKey (fun _ v -> v = 'S')
    let dest = map |> Map.findKey (fun _ v -> v = 'E')
    let map = 
        map
        |> Map.change start (fun _ -> Some '.')
        |> Map.change dest (fun _ -> Some '.')
    (map, start, dest)

let search (start: Vec2 * Vec2) (dest: Vec2) (all: bool) (limit: int) (map: Map<Vec2, char>) = 
    let cmp = if all then (<) else (<=)
    let rec dijkstra (queue: PriorityQueue<State, int>) (stateStorage: Map<Vec2 * Vec2, int>) (solutions: State list) = 
        if queue.Count > 0 then
            let state = queue.Dequeue ()
            let { pos = sPos; dir = sDir; cost = sCost } = state

            match stateStorage |> Map.tryFind (sPos, sDir) with
            | Some cost when (cmp cost sCost) -> dijkstra queue stateStorage solutions
            | _ when sCost > limit -> dijkstra queue stateStorage solutions
            | _ -> 
                let stateStorage = stateStorage |> Map.add (sPos, sDir) sCost
                if sPos = dest then
                    if all then
                        let solutions = 
                            match solutions with
                            | [] -> [state]
                            | head :: _ when sCost < head.cost -> [state]
                            | head :: _ when sCost = head.cost -> state :: solutions
                            | _ -> solutions
                        dijkstra queue stateStorage solutions
                    else
                        [state]
                else
                    let ahead = map |> Map.find (sPos + sDir)
                    let rDir = sDir |> Vec2.rotRight
                    let lDir = sDir |> Vec2.rotLeft
                    let newStates = seq {
                        if map |> Map.find (sPos + rDir) <> '#' then
                            yield { state with pos = sPos + rDir; dir = rDir; cost = sCost + 1001; prev = Some state }
                        if map |> Map.find (sPos + lDir) <> '#' then
                            yield { state with pos = sPos + lDir; dir = lDir; cost = sCost + 1001; prev = Some state }
                        if ahead <> '#' then
                            yield { state with pos = sPos + sDir; cost = sCost + 1; prev = Some state }
                    }
                    queue.EnqueueRange (newStates |> Seq.map (fun n -> (n, n.cost)))
                    dijkstra queue stateStorage solutions
        else solutions

    let prio = PriorityQueue<State, int> ()
    prio.Enqueue ({ pos = start |> fst; dir = start |> snd; cost = 0; prev = None }, 0)
    dijkstra prio Map.empty []

let rec reconstruct (path: Vec2 list) (state: State) = 
    let path = state.pos :: path
    match state.prev with
    | Some state -> reconstruct path state
    | _ -> path |> List.rev

let shortestPath (map: Map<Vec2, char>) (start: Vec2) (dest: Vec2) = 
    match (map |> search (start, Vec2.right) dest false 0xFFFFFF) with
    | head :: _ -> head.cost
    | _ -> failwith ""

let solveFirst (input: Input) =
    input |> parseMap |||> shortestPath

let solveSecond (input: Input) = 
    let map, start, dest = input |> parseMap
    let best = shortestPath map start dest
    map
    |> search (start, Vec2.right) dest true best
    |> List.collect (reconstruct [])
    |> List.distinct
    |> List.length
