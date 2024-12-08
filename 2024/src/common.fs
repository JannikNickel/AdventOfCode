module Common

[<Struct>]
type Vec2 = Vec2 of struct(int * int)
with
    member inline this.x = match this with Vec2(x, _) -> x
    member inline this.y = match this with Vec2(_, y) -> y

    static member one = Vec2(1, 1)
    static member ofTpl (tpl: int * int) = Vec2(fst tpl, snd tpl)

    static member inline (+) (v1: Vec2, v2: Vec2) = Vec2(v1.x + v2.x, v1.y + v2.y)
    static member inline (-) (v1: Vec2, v2: Vec2) = Vec2(v1.x - v2.x, v1.y - v2.y)
    static member inline (*) (v1: Vec2, s: int) = Vec2(v1.x * s, v1.y * s)

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
