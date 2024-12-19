module Day19
open Input

[<Literal>]
let name = "Linen Layout"

let parseInput (input: Input) = 
    let towels = input.lines[0].Split ", " |> Array.map List.ofSeq
    let patterns = input.lines |> List.skip 2 |> List.map List.ofSeq
    towels, patterns

let startsWith (prefix: char list) (text: char list) = 
    List.length prefix <= List.length text && 
    [| 0 .. List.length prefix - 1 |] |> Array.forall (fun i -> prefix[i] = text[i])

let countVariants (pattern: char list) (towels: char list array) =
    let rec reduce (remaining: char list) (cache: Map<char list, uint64>) = 
        match cache |> Map.tryFind remaining with
        | Some value -> (value, cache)
        | _ -> 
            if remaining |> List.isEmpty then
                (1UL, cache)
            else
                let value, cache = 
                    towels
                    |> Array.filter (fun n -> remaining |> startsWith n)
                    |> Array.fold (fun (sum, c) n -> 
                        let value, c = reduce (remaining[n |> List.length ..]) c
                        (sum + value, c)
                    ) (0UL, cache)
                let cache = cache |> Map.add remaining value
                (value, cache)

    reduce pattern Map.empty |> fst

let solveFirst (input: Input) = 
    let towels, patterns = input |> parseInput
    patterns
    |> List.filter (fun n -> (countVariants n towels) > 0UL)
    |> List.length

let solveSecond (input: Input) = 
    let towels, patterns = input |> parseInput
    patterns
    |> List.sumBy (fun n -> (countVariants n towels))
