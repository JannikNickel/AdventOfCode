module Day14
open Input
open Common

[<Literal>]
let name = "Restroom Redoubt"

let gridSize = Vec2(101, 103)

let parseInput (input: Input) = 
    input.lines
    |> List.map Parsing.ints
    |> List.map (function [px; py; vx; vy] -> (Vec2(px, py), Vec2(vx, vy)) | _ -> failwith "")

// Just to find the kind of shape to look for
let appendMapToFile (file: string) (robots: Vec2 list) (iteration: int) = 
    let grid = Array.init gridSize.y (fun _ -> Array.init gridSize.x (fun _ -> '.'))
    robots |> List.iter (fun pos -> grid[pos.y][pos.x] <- '#')
    let rows =
        grid
        |> Array.map (fun row -> row |> System.String)
        |> List.ofArray
    let rows = (sprintf "Iteration: %d" iteration) :: rows
    System.IO.File.AppendAllLines (file, rows)

let wrap (size: int) (value: int) = (value % size + size) % size

let moveRobot (turns: int) (robot: Vec2 * Vec2) = 
    let (pos, vel) = robot
    let pos = pos + vel * turns
    Vec2(pos.x |> wrap gridSize.x, pos.y |> wrap gridSize.y), vel

let quadrants (robots: Vec2 list) = 
    let xDiv = gridSize.x / 2
    let yDiv = gridSize.y / 2
    robots
    |> List.groupBy (fun pos -> 
        match (pos.x, pos.y) with
        | (x, y) when x < xDiv && y < yDiv -> 0
        | (x, y) when x > xDiv && y < yDiv -> 1
        | (x, y) when x < xDiv && y > yDiv -> 2
        | (x, y) when x > xDiv && y > yDiv -> 3
        | _ -> -1
    )
    |> List.filter (fun n -> fst n >= 0)
    |> List.map snd

let isTreePart (positions: Set<Vec2>) (start: Vec2) = 
    seq {
        for i in 1 .. 5 do
            yield! seq { for k in [ -i .. i ] -> Vec2(k, i) }
    }
    |> Seq.map ((+) start)
    |> Seq.forall (fun n -> positions |> Set.contains n)

let isTree (robots: Vec2 list) = 
    let set = robots |> Set.ofList
    robots |> List.exists (fun n -> (isTreePart set n))

let rec findTree (robots: (Vec2 * Vec2) list) (iteration: int) = 
    if isTree (robots |> List.map fst) then
        iteration
    else
        let robots = robots |> List.map (moveRobot 1)
        findTree robots (iteration + 1)

let solveFirst (input: Input) = 
    input
    |> parseInput
    |> List.map ((moveRobot 100) >> fst)
    |> quadrants
    |> List.map List.length
    |> List.reduce (*)

let solveSecond (input: Input) = 
    findTree (input |> parseInput) 0
