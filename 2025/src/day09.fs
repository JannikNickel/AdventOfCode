module Day09
open Input
open Common

[<Literal>]
let name = "Movie Theater"

let parseInput (input: Input) =
    input.lines |> List.map (Parsing.ints >> Vec2.ofLst)

let minMax a b = (min a b, max a b)

let rectArea (a: Vec2) (b: Vec2) = 
    let (xMin, xMax) = minMax a.x b.x
    let (yMin, yMax) = minMax a.y b.y
    int64 (xMax - xMin + 1) * int64 (yMax - yMin + 1)

let linesCollide (aFrom: Vec2) (aTo: Vec2) (bFrom: Vec2) (bTo: Vec2) = 
    let (axMin, axMax) = minMax aFrom.x aTo.x
    let (ayMin, ayMax) = minMax aFrom.y aTo.y
    let (bxMin, bxMax) = minMax bFrom.x bTo.x
    let (byMin, byMax) = minMax bFrom.y bTo.y
    not (axMax < bxMin || bxMax < axMin || ayMax < byMin || byMax < ayMin)

let isFilled (edges: (Vec2 * Vec2) list) (a: Vec2) (b: Vec2) = 
    let xMin = min a.x b.x + 1
    let xMax = max a.x b.x - 1
    let yMin = min a.y b.y + 1
    let yMax = max a.y b.y - 1

    let rectOutline = 
        [ (Vec2(xMin, yMin), Vec2(xMax, yMin))
          (Vec2(xMax, yMin), Vec2(xMax, yMax))
          (Vec2(xMax, yMax), Vec2(xMin, yMax))
          (Vec2(xMin, yMax), Vec2(xMin, yMin)) ]

    rectOutline
    |> List.forall (fun (rFrom, rTo) -> 
        edges |> List.forall (fun (eFrom, eTo) -> not (linesCollide rFrom rTo eFrom eTo))
    )

let solveFirst (input: Input) = 
    input
    |> parseInput
    |> List.uniquePairs
    |> List.map ((<||) rectArea)
    |> List.max

let solveSecond (input: Input) = 
    let redTiles = parseInput input
    let redEdges = redTiles |> List.pairwise |> List.append [(List.last redTiles, List.head redTiles)]

    redTiles
    |> List.uniquePairs
    |> List.sortByDescending ((<||) rectArea)
    |> List.tryFind (fun (a, b) -> isFilled redEdges a b)
    |> function 
        | Some (a, b) -> rectArea a b
        | None -> 0L
