module Day15
open Input
open Common

[<Literal>]
let name = "Warehouse Woes"

type Cell = Start | Empty | Wall | Crate | LCrate | RCrate

let matchDir = function
    | '^' -> Vec2(0, -1)
    | 'v' -> Vec2(0, 1)
    | '<' -> Vec2(-1, 0)
    | '>' -> Vec2(1, 0)
    | _ -> failwith ""

let matchCell = function
    | '@' -> Start
    | '.' -> Empty
    | '#' -> Wall
    | 'O' -> Crate
    | '[' -> LCrate
    | ']' -> RCrate
    | _ -> failwith ""

let expandLine (line: string) = 
    line
    |> Seq.collect (function
        | '#' -> seq { '#'; '#' }
        | 'O' -> seq { '['; ']' }
        | '.' -> seq { '.'; '.' }
        | '@' -> seq { '@'; '.' }
        | _ -> failwith ""
    )
    |> Array.ofSeq
    |> System.String

let parseMap (lines: string list) expand = 
    lines
    |> List.map (if expand then expandLine else id)
    |> Parsing.map2d Vec2.ofTpl matchCell
    |> Map.ofSeq

let parseInput (expand: bool) (input: Input) = 
    let parts = input.lines |> List.split ((=) "")
    let map = parseMap parts[0] expand
    let start = map |> Map.findKey (fun _ v -> v = Start)
    let map = map |> Map.change start (fun _ -> Some Empty)
    let moves = parts[1] |> Seq.collect (Seq.map matchDir)
    (map, start, moves)

let findEmptyOrWall (map: Map<Vec2, Cell>) (start: Vec2) (dir: Vec2) = 
    let rec search (pos: Vec2) = 
        match map |> Map.find pos with
        | Empty -> (pos, Empty)
        | Wall -> (pos, Wall)
        | _ -> search (pos + dir)
    search (start + dir)

let move (map: Map<Vec2, Cell>) (start: Vec2) (dir: Vec2) = 
    let neighbour = map |> Map.find (start + dir)
    match neighbour with
    | Empty -> (map, start + dir)
    | Crate -> 
        let (pos, cell) = findEmptyOrWall map start dir
        if cell = Wall then
            (map, start)
        else
            let map =
                map
                |> Map.change (start + dir) (fun _ -> Some Empty)
                |> Map.change pos (fun _ -> Some Crate)
            (map, start + dir)
    | _ -> (map, start)

let findConnectedCrates (map: Map<Vec2, Cell>) (start: Vec2) (dir: Vec2) = 
    let rec search (set: Vec2 list) (closed: Set<Vec2>) = 
        match set with
        | pos :: tail -> 
            match map |> Map.find pos with
            | LCrate | RCrate as c -> 
                let next = [ pos; (pos + (if c = LCrate then Vec2.right else Vec2.left)) ]
                let closed = closed |> Set.union (next |> Set.ofList)
                let set = tail @ (next |> List.map ((+) dir))
                search set closed
            | _ -> search tail closed
        | _ -> closed

    search [start + dir] Set.empty

let shiftHorizontal (map: Map<Vec2, Cell>) (start: Vec2) (dest: Vec2) (dir: Vec2) = 
    let range = List.fromTo dest.x (start.x + dir.x)
    let map = 
        range
        |> List.fold (fun acc x -> 
            acc |> Map.change (Vec2(x, start.y)) (fun _ -> Some (map |> Map.find (Vec2(x - dir.x, start.y))))
        ) map
        |> Map.change (start + dir) (fun _ -> Some Empty)
    (map, start + dir)

let shiftVertical (map: Map<Vec2, Cell>) (crates: Set<Vec2>) (start: Vec2) (dir: Vec2) = 
    let sortTarget = start.y + dir.y * 10000
    let order = 
        crates
        |> List.ofSeq
        |> List.sortBy (fun n -> abs (n.y - sortTarget))
    let map = 
        order
        |> List.fold (fun acc pos -> 
            let nextPos = pos + dir
            let nextValue = acc |> Map.find nextPos
            let value = acc |> Map.find pos
            acc
            |> Map.change nextPos (fun _ -> Some value)
            |> Map.change pos (fun _ -> Some nextValue)
        ) map
    (map, start + dir)

let moveWide (map: Map<Vec2, Cell>) (start: Vec2) (dir: Vec2) = 
    let neighbour = map |> Map.find (start + dir)
    match neighbour with
    | Empty -> (map, start + dir)
    | LCrate | RCrate -> 
        if abs dir.x <> 0 then
            let (pos, cell) = findEmptyOrWall map start dir
            if cell = Wall then
                (map, start)
            else
                shiftHorizontal map start pos dir
        else
            let crates = findConnectedCrates map start dir
            let free = not (crates |> Seq.exists (fun pos -> map |> Map.find (pos + dir) = Wall))
            if not free then
                (map, start)
            else
                shiftVertical map crates start dir
    | _ -> (map, start)

let solve wide (input: Input) = 
    let map, start, moves = input |> parseInput wide
    let moveFn = if wide then moveWide else move
    moves
    |> Seq.fold (fun (map, start) dir -> moveFn map start dir) (map, start)
    |> fst
    |> Map.filter (fun _ value -> value = Crate || value = LCrate)
    |> Map.fold (fun acc pos _ -> acc + pos.y * 100 + pos.x) 0

let solveFirst (input: Input) = input |> solve false
let solveSecond (input: Input) = input |> solve true
