module Day20
open Input
open Common

[<Literal>]
let name = "Race Condition"

let dirs = [ Vec2(-1, 0); Vec2(0, 1); Vec2(1, 0); Vec2(0, -1) ]

type State = {
    pos: Vec2
    depth: int
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

let findPath (map: Map<Vec2, char>) (start: Vec2) (dest: Vec2) =
    let size = map |> Map.keys |> Seq.max |> (+) Vec2.one
    let rec bfs (closed: Set<Vec2>) (queue: State list) = 
        match queue with
        | state :: tail -> 
            let { pos = curr; depth = depth } = state
            if curr = dest then
                state
            else
                let queue = tail
                if closed |> Set.contains curr then
                    bfs closed queue
                else
                    let closed = closed |> Set.add curr
                    let value = map |> Map.find curr
                    let next = 
                        dirs
                        |> List.map (fun n -> { pos = curr + n; depth = depth + 1; prev = Some state })
                        |> List.filter (fun n -> n.pos |> Vec2.inBounds size)
                        |> List.filter (fun n -> map |> Map.find n.pos = value)
                    bfs closed (queue @ next)
        | _ -> failwith ""

    bfs Set.empty [{ pos = start; depth = 0; prev = None }]

let rec reconstruct (path: (Vec2 * int) list) (state: State) = 
    let path = (state.pos, state.depth) :: path
    match state.prev with
    | Some state -> reconstruct path state
    | _ -> path |> List.rev

let uniquePairs (arr: 'a array) = 
    [| 0 .. arr.Length - 1 |]
    |> Seq.collect (fun i -> seq {
        for k in i + 1 .. arr.Length - 1 -> (arr[i], arr[k])
    })

let filterFuncA map (a: Vec2) (b: Vec2) = 
    (a.x = b.x || a.y = b.y) && (Vec2.mhDst a b = 2) && (map |> Map.find (a + Vec2.normDir a b) = '#')

let filterFuncB _ (a: Vec2) (b: Vec2) = 
    Vec2.mhDst a b <= 20

let possibleCheats (map: Map<Vec2, char>) (path: (Vec2 * int) array) filterFunc = 
    path
    |> uniquePairs
    |> Seq.filter (fun ((a, _), (b, _)) -> filterFunc map a b)
    |> Seq.map (fun ((a as (aPos, aDepth)), (b as (bPos, bDepth))) -> 
        (a, b, abs (bDepth - aDepth) - (Vec2.mhDst aPos bPos))
    )

let solve cheatFilter (input: Input) = 
    let map, start, dest = input |> parseMap
    let res = findPath map start dest
    let path = res |> reconstruct [] |> Array.ofList
    possibleCheats map path cheatFilter
    |> Seq.filter (fun (_, _, n) -> n >= 100)
    |> Seq.length

let solveFirst (input: Input) = input |> solve filterFuncA
let solveSecond (input: Input) = input |> solve filterFuncB
