module Common

[<Struct>]
type Vec2 = Vec2 of struct(int * int)
with
    member inline this.x = match this with Vec2(x, _) -> x
    member inline this.y = match this with Vec2(_, y) -> y

    static member one = Vec2(1, 1)
    static member left = Vec2(-1, 0)
    static member right = Vec2(1, 0)
    static member dirs = [ Vec2(-1, 0); Vec2(0, -1); Vec2(1, 0); Vec2(0, 1) ]
    static member dirsDiag = [ Vec2(-1, -1); Vec2(1, -1); Vec2(1, 1); Vec2(-1, 1) ]

    static member inline (+) (v1: Vec2, v2: Vec2) = Vec2(v1.x + v2.x, v1.y + v2.y)
    static member inline (-) (v1: Vec2, v2: Vec2) = Vec2(v1.x - v2.x, v1.y - v2.y)
    static member inline (*) (v1: Vec2, s: int) = Vec2(v1.x * s, v1.y * s)
    static member inline (/) (v1: Vec2, s: int) = Vec2(v1.x / s, v1.y / s)

module Vec2 = 
    let ofTpl (tpl: int * int) = Vec2(fst tpl, snd tpl)
    let rotRight (dir: Vec2) = Vec2(dir.y, -dir.x)
    let rotLeft (dir: Vec2) = Vec2(-dir.y, dir.x)
    let mhDst (a: Vec2) (b: Vec2) = abs (b.x - a.x) + abs (b.y - a.y)
    let normDir (a: Vec2) (b: Vec2) = (b - a) / (mhDst a b)

    let inBounds (size: Vec2) (pos: Vec2) =
        pos.x >= 0 && pos.y >= 0 && pos.x < size.x && pos.y < size.y

module List = 
    let split (pred: 'a -> bool) (list: 'a list) = 
        list
        |> List.fold (fun (acc, curr) n -> if pred n then (acc @ [curr], []) else (acc, n :: curr)) ([], [])
        |> fun pair -> fst pair @ [snd pair]
        |> List.map (fun n -> List.rev n)

    let windowedi (windowSize: int) (list: 'a list) = 
        list
        |> List.mapi (fun i _ -> i, list |> List.skip i |> List.truncate windowSize)
        |> List.filter (fun (_, wnd) -> wnd.Length = windowSize)

    let pairs (list: 'a list) = List.allPairs list list

    let alternate (list: 'a list) = 
        let rec split odd even list = 
            match list with
            | a :: b :: tail -> split (a :: odd) (b :: even) tail
            | a :: tail -> split (a :: odd) even tail
            | [] -> List.rev odd, List.rev even
        split [] [] list

    let fromTo start dest = 
        if start < dest then
            [ start .. dest ]
        else
            [ start .. -1 .. dest ]

    let range len = fromTo 0 (len - 1)

module Map = 
    let valueOrDefault key defaultValue = 
        Map.tryFind key >> Option.defaultValue defaultValue

module String = 
    let ofList = Array.ofList >> System.String

module Parsing = 
    open System.Text.RegularExpressions

    let numbers (parse: string -> 'T) (str: string) = 
        let matches = Regex.Matches (str, @"-?\d+")
        [ for m in matches -> parse m.Value ]

    let uNumbers (parse: string -> 'T) (str: string) = 
        let matches = Regex.Matches (str, @"\d+")
        [ for m in matches -> parse m.Value ]

    let ints (str: string) = numbers int str

    let matches (regex: string) (source: string) = 
        let matches = Regex.Matches (source, regex)
        [ for m in matches -> m.Value ]

    let map2d (keyMapping: (int * int) -> 'T1) (mapping: char -> 'T2) (lines: string list) : ('T1 * 'T2) seq = 
        seq {
            for x in 0 .. lines[0].Length - 1 do
                for y in 0 .. lines.Length - 1 do
                    yield keyMapping (x, y), mapping lines[y].[x]
        }
