module Day23
open Input
open Common

[<Literal>]
let name = "LAN Party"

let constructNetwork (input: Input) = 
    input.lines
    |> List.fold (fun map conn -> 
        let a, b = conn[0..1], conn[3..4]
        map
        |> Map.change a (function Some lst -> Some (b :: lst) | _ -> Some [b])
        |> Map.change b (function Some lst -> Some (a :: lst) | _ -> Some [a])
    ) Map.empty

let findPairs (map: Map<string, string list>) = 
    map
    |> Map.keys
    |> Seq.collect (fun n -> 
        map
        |> Map.find n
        |> List.map (fun k -> if k < n then (k, n) else (n, k))
    )
    |> Seq.distinct

let areConnected (a: string) (b: string) (map: Map<string, string list>) = 
    map |> Map.find a |> List.contains b

let findTriangles (map: Map<string, string list>) = 
    map
    |> findPairs
    |> Seq.collect (fun (a, b) -> 
        let aSet = map |> Map.find a |> Set.ofList
        let bSet = map |> Map.find b |> Set.ofList
        aSet
        |> Set.intersect bSet
        |> Seq.map (fun n -> [a; b; n] |> List.sort)
    )
    |> Seq.distinct

let connectedSets (map: Map<string, string list>) = 
    map
    |> findPairs
    |> Seq.map (fun (a, b) -> 
        let aSet = map |> Map.find a |> Set.ofList
        let bSet = map |> Map.find b |> Set.ofList
        let union = aSet |> Set.intersect bSet
        a :: b :: (union |> List.ofSeq)
        |> List.sort
    )
    |> Seq.filter (fun n -> 
        n
        |> List.pairs
        |> List.forall (fun (a, b) -> a = b || map |> areConnected a b)
    )
    |> Seq.distinct

let solveFirst (input: Input) = 
    input
    |> constructNetwork
    |> findTriangles
    |> Seq.filter (List.exists (fun n -> n[0] = 't'))
    |> Seq.length

let solveSecond (input: Input) = 
    input
    |> constructNetwork
    |> connectedSets
    |> Seq.maxBy List.length
    |> Array.ofSeq
    |> String.concat ","
