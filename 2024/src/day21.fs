module Day21
open Input
open Common

[<Literal>]
let name = "Keypad Conundrum"

let numpad = Map.ofList [
    (Vec2(0, 0), '7'); (Vec2(1, 0), '8'); (Vec2(2, 0), '9');
    (Vec2(0, 1), '4'); (Vec2(1, 1), '5'); (Vec2(2, 1), '6');
    (Vec2(0, 2), '1'); (Vec2(1, 2), '2'); (Vec2(2, 2), '3');
    (*               *)(Vec2(1, 3), '0'); (Vec2(2, 3), 'A') ]

let dirpad = Map.ofList [
    (*               *)(Vec2(1, 0), '^'); (Vec2(2, 0), 'A');
    (Vec2(0, 1), '<'); (Vec2(1, 1), 'v'); (Vec2(2, 1), '>') ]

let numpadStart = numpad |> Map.findKey (fun _ v -> v = 'A')
let dirpadStart = dirpad |> Map.findKey (fun _ v -> v = 'A')

let moveFromTo (from: Vec2) (dest: Vec2) (num: bool) = 
    let xDiff = dest.x - from.x
    let yDiff = dest.y - from.y
    let xDir = if xDiff < 0 then '<' else '>'
    let yDir = if yDiff < 0 then '^' else 'v'
    let xMoves = [ for _ in 0 .. (abs xDiff - 1) -> xDir ]
    let yMoves = [ for _ in 0 .. (abs yDiff - 1) -> yDir ]

    let keyMap = if num then numpad else dirpad
    let hMoveAllowed = keyMap |> Map.containsKey (Vec2(dest.x, from.y))
    let vMoveAllowed = keyMap |> Map.containsKey (Vec2(from.x, dest.y))

    match hMoveAllowed, vMoveAllowed with
    | true, true when xDir = '<' -> xMoves @ yMoves @ ['A']
    | true, false -> xMoves @ yMoves @ ['A']
    | _ -> yMoves @ xMoves @ ['A']

let solveKeypad (code: char list) (num: bool) (start: Vec2) = 
    let keyMap = if num then numpad else dirpad
    code
    |> List.fold (fun (acc: char list, from: Vec2) n -> 
        let dest = keyMap |> Map.findKey (fun _ v -> v = n)
        let next = acc @ (moveFromTo from dest num)
        next, dest
    ) ([], start)
    |> fst

let rec calcLength (code: char list) (depth: int) (cache: Map<(char list * int), int64>) = 
    let key = (code, depth)
    match cache |> Map.tryFind key with
    | Some value -> (value, cache)
    | _ -> 
        if depth = 0 then
            (code |> List.length |> int64, cache)
        else
            let (res, cache) = 
                code
                |> Seq.zip ('A' :: code)
                |> Seq.fold (fun (acc, cache) (from, dest) -> 
                    let fromPos = dirpad |> Map.findKey (fun _ v -> v = from)
                    let destPos = dirpad |> Map.findKey (fun _ v -> v = dest)
                    let (len, cache) = calcLength (moveFromTo fromPos destPos false) (depth - 1) cache
                    acc + len, cache
                ) (0L, cache)
            (res, cache |> Map.add key res)

let parseNum (code: char list) = 
    code
    |> String.ofList
    |> Parsing.numbers int64
    |> List.head

let solveFirst (input: Input) = 
    let codes = input.lines |> List.map List.ofSeq
    codes
    |> List.sumBy (fun code -> 
        let numpad = solveKeypad code true numpadStart
        let moves = 
            List.range 2
            |> List.fold (fun moves _ -> solveKeypad moves false dirpadStart) numpad
        (parseNum code) * (moves |> List.length |> int64)
    )

let solveSecond (input: Input) = 
    let codes = input.lines |> List.map List.ofSeq
    codes
    |> List.map (fun code -> 
        let numpad = solveKeypad code true numpadStart
        (parseNum code) * fst (calcLength numpad 25 Map.empty)
    )
    |> List.sum
