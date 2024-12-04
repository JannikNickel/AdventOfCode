module Common

module List = 
    let split (pred: 'a -> bool) (list: 'a list) = 
        list
        |> List.fold (fun (acc, curr) n -> if pred n then (acc @ [curr], []) else (acc, n :: curr)) ([], [])
        |> fun pair -> fst pair @ [snd pair]
        |> List.map (fun n -> List.rev n)

    let windowedi (windowSize: int) (list: 'T list) = 
        list
        |> List.mapi (fun i _ -> i, list |> List.skip i |> List.truncate windowSize)
        |> List.filter (fun (_, wnd) -> wnd.Length = windowSize)

module Map = 
    let valueOrDefault key defaultValue = 
        Map.tryFind key >> Option.defaultValue defaultValue

module String = 
    let ofList = Array.ofList >> System.String

module Parsing = 
    open System.Text.RegularExpressions

    let ints (str: string) = 
        let matches = Regex.Matches (str, @"-?\d+")
        [ for m in matches -> int m.Value ]

    let matches (regex: string) (source: string) = 
        let matches = Regex.Matches (source, regex)
        [ for m in matches -> m.Value ]

    let map2d (mapping: char -> 'T) (lines: string list) : ((int * int) * 'T) seq = 
        seq {
            for x in 0 .. lines[0].Length - 1 do
                for y in 0 .. lines.Length - 1 do
                    yield (x, y), mapping lines[y].[x]
        }
