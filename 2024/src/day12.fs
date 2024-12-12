module Day12
open Input
open Common

[<Literal>]
let name = "Garden Groups"

let dirs = [ Vec2(-1, 0); Vec2(0, 1); Vec2(1, 0); Vec2(0, -1) ]

let left (dir: Vec2) = Vec2(-dir.y, dir.x)
let right (dir: Vec2) = Vec2(dir.y, -dir.x)

let findPlots (map: Map<Vec2, char>) = 
    let size = map |> Map.keys |> Seq.max |> (+) Vec2.one
    let rec bfs (closed: Set<Vec2>) (queue: Vec2 list) = 
        match queue with
        | curr :: tail -> 
            let queue = tail
            if closed |> Set.contains curr then
                bfs closed queue
            else
                let closed = closed |> Set.add curr
                let value = map |> Map.find curr
                let next = 
                    dirs
                    |> List.map (fun n -> curr + n)
                    |> List.filter (Vec2.inBounds size)
                    |> List.filter (fun n -> map |> Map.find n = value)
                let queue = queue @ next
                bfs closed queue
        | _ -> closed
        
    map
    |> Map.keys
    |> Seq.fold (fun (plots, closed) n -> 
        if closed |> Set.contains n then
            (plots, closed)
        else
            let plot = bfs Set.empty [n]
            (plot :: plots, closed |> Set.union plot)
    ) ([], Set.empty)
    |> fst

let findPerimeter (map: Map<Vec2, char>) (plot: Set<Vec2>) = 
    let value = map |> Map.find (plot |> Seq.head)
    plot
    |> Seq.sumBy (fun pos -> 
        dirs
        |> Seq.filter (fun dir ->
            map |> Map.tryFind (pos + dir) |> Option.defaultValue '\000' <> value
        )
        |> Seq.length
    )  

let findSides (map: Map<Vec2, char>) (plot: Set<Vec2>) = 
    let size = map |> Map.keys |> Seq.max |> (+) Vec2.one
    let value = map |> Map.find (plot |> Seq.head)
    let edges = 
        plot
        |> Seq.collect (fun pos -> 
            dirs
            |> Seq.map (fun dir ->
                match map |> Map.tryFind (pos + dir) with
                | Some n when n <> value -> Some (pos, dir)
                | None -> Some (pos, dir)
                | _ -> None
            )
        )
        |> Seq.filter (fun n -> n |> Option.isSome)
        |> Seq.map Option.get
        |> Set.ofSeq

    let rec countEdges (closed: Set<Vec2 * Vec2>) (curr: Vec2 * Vec2) = 
        let closed = closed |> Set.add curr
        let pos, dir = fst curr, snd curr
        let next = 
            [ pos + (left (curr |> snd)); pos + (right dir) ]
            |> List.filter (fun n -> n |> Vec2.inBounds size)
            |> List.filter (fun n -> map |> Map.find n = value)
            |> List.map (fun n -> (n, dir))
            |> List.filter (fun n -> not (closed |> Set.contains n))
            |> List.filter (fun n -> edges |> Set.contains n)
        match next with
        | [] -> closed
        | _ -> next |> List.map (countEdges closed) |> Set.unionMany

    edges
    |> Seq.fold (fun (count, closed) n -> 
        if closed |> Set.contains n then
            (count, closed)
        else
            let next = countEdges Set.empty n
            count + 1, closed |> Set.union next
    ) (0, Set.empty)
    |> fst

let solve perimeterFunc (input: Input) = 
    let map = input.lines |> Parsing.map2d Vec2.ofTpl id |> Map.ofSeq
    map
    |> findPlots
    |> List.sumBy (fun n -> (n |> Set.count) * (perimeterFunc map n))

let solveFirst (input: Input) = input |> solve findPerimeter
let solveSecond (input: Input) = input |> solve findSides
