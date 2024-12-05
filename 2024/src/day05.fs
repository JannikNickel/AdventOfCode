module Day05
open Input
open Common

[<Literal>]
let name = "Print Queue"

let parseInput (input: Input) = 
    let parts = input.lines |> List.split ((=) "")
    let rules = parts[0] |> List.map Parsing.ints |> List.map (function [a; b] -> (a, b) | _ -> (0, 0))
    let updates = parts[1] |> List.map Parsing.ints
    rules, updates

let isOrdered (rules: (int * int) list) (pages: int list) = 
    rules
    |> List.forall (fun rule -> 
        let i = pages |> List.tryFindIndex ((=) (fst rule))
        let k = pages |> List.tryFindIndex ((=) (snd rule))
        i <= k || ((i = None) <> (k = None))
    )

let topologicalSort (dependencies: Map<int, int list>) = 
    let rec dfs (order: int list) (closed: Set<int>) (element: int) = 
        if not (closed |> Set.contains element) then
            let closed = closed |> Set.add element
            let next = dependencies |> Map.tryFind element |> Option.defaultValue []
            let order, closed = next |> List.fold (fun (order, closed) n -> dfs order closed n) (order, closed)
            element :: order, closed
        else
            order, closed

    dependencies.Keys
    |> Seq.fold (fun (stack, closed) element -> dfs stack closed element) ([], Set.empty)
    |> fst

let sortList (rules: (int * int) list) (pages: int list) = 
    let sMap = pages |> List.map (fun n -> (n, [])) |> Map.ofList
    let rMap = 
        rules
        |> List.fold (fun acc (before, after) -> 
            acc |> Map.change before (function Some n -> Some (after :: n) | _ -> None)
        ) sMap
        |> topologicalSort
        |> List.mapi (fun i n -> (n, i))
        |> Map.ofList

    pages
    |> List.sortBy (fun n -> rMap |> Map.find n)

let middle (lst: 'a list) = lst |> List.item (List.length lst / 2)

let solveFirst (input: Input) = 
    let (rules, updates) = parseInput input
    updates
    |> List.filter (isOrdered rules)
    |> List.sumBy middle

let solveSecond (input: Input) = 
    let (rules, updates) = parseInput input
    updates
    |> List.filter (fun l -> not (l |> isOrdered rules))
    |> List.map (sortList rules)
    |> List.sumBy middle
