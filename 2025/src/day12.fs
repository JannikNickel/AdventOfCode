module Day12
open Input
open Common

[<Literal>]
let name = "Christmas Tree Farm"

let parseInput (input: Input) = 
    let lastIdx = input.lines |> List.findIndexBack ((=) "")
    let shapes = 
        input.lines.[.. (lastIdx - 1)]
        |> List.split ((=) "")
        |> List.map (fun lines -> lines.[1..] |> Parsing.map2d Vec2.ofTpl id |> List.ofSeq)
    let areas = 
        input.lines.[(lastIdx + 1) ..]
        |> List.map (fun line -> 
            let nums = line |> Parsing.ints
            Vec2(nums.[0], nums.[1]), nums.[2..]
        )
    shapes, areas

let shapeArea (shape: (Vec2 * char) list) = 
    let width = shape |> List.map (fst >> _.x) |> List.max |> (+) 1
    let height = shape |> List.map (fst >> _.y) |> List.max |> (+) 1
    width * height

let canFit (shapes: (Vec2 * char) list list) (area: (Vec2 * int list)) =
    let areaSize = area |> fst |> fun v -> v.x * v.y
    let requiredSize = area |> snd |> List.mapi (fun i n -> n * (shapeArea shapes.[i])) |> List.sum
    requiredSize <= areaSize

let solveFirst (input: Input) = 
    let (shapes, areas) = parseInput input
    areas
    |> List.filter (canFit shapes)
    |> List.length

let solveSecond (input: Input) = 
    // Day 12 only has 1 puzzle
    null :> obj
