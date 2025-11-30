module Input
open System.IO

type Input = {
    raw: string
    lines: string list }
with
    static member load path = {
        raw = File.ReadAllText path
        lines = File.ReadAllLines path |> List.ofArray }
