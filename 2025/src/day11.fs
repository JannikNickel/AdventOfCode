module Day11
open Input
open Common

[<Literal>]
let name = "Reactor"

let parseInput (input: Input) =
    input.lines
    |> List.fold (fun acc line -> 
        let matches = Parsing.matches "[a-z]{3}" line
        acc |> Map.add matches.[0] matches.[1..]
    ) Map.empty

let countPaths (start: string) (target: string) (graph: Map<string, string list>) =
    let rec count (start: string) (target: string) (cache: Map<string * string, uint64>) =
        match cache |> Map.tryFind (start, target) with
        | Some cached -> cached, cache
        | None -> 
            let res, nextCache = 
                if start = target then
                    1UL, cache
                else
                    graph
                    |> Map.valueOrDefault start []
                    |> List.fold (fun (acc, currCache) n -> 
                        let pathAmount, updatedCache = count n target currCache
                        acc + pathAmount, updatedCache
                    ) (0UL, cache)
            res, (nextCache |> Map.add (start, target) res)

    (count start target Map.empty) |> fst

let solveFirst (input: Input) = 
    input
    |> parseInput
    |> countPaths "you" "out"

let solveSecond (input: Input) = 
    let graph = parseInput input
    let paths = [
        [ "svr"; "dac"; "fft"; "out" ];
        [ "svr"; "fft"; "dac"; "out" ] ]
        
    paths
    |> List.sumBy (fun path -> 
        path
        |> List.pairwise
        |> List.map (fun (a, b) -> countPaths a b graph)
        |> List.reduce (*)
    )
