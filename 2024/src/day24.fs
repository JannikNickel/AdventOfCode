module Day24
open Input
open Common

[<Literal>]
let name = "Crossed Wires"

type GateInput = Const of int | Complex of string

type Gate = {
    x: GateInput
    y: GateInput
    op: (int -> int -> int) }

let matchOp = function
    | "AND" -> (&&&)
    | "OR" -> (|||)
    | "XOR" -> (^^^)
    | _ -> failwith ""

let parseGateInp (consts: Map<string, int>) (inp: string) = 
    match consts |> Map.tryFind inp with
    | Some value -> Const value
    | _ -> Complex inp

let parseInput (input: Input) = 
    let parts = input.lines |> List.split ((=) "")
    let initial = parts[0] |> List.map (fun n -> (n.Split ":") |> fun k -> (k[0], int k[1])) |> Map.ofList
    let gates = 
        parts[1]
        |> List.fold (fun acc n -> 
            let split = n.Split ([| " -> "; " " |], System.StringSplitOptions.None)
            let gate = { x = (split[0] |> parseGateInp initial); y = (split[2] |> parseGateInp initial); op = (matchOp split[1]) }
            acc |> Map.add split[3] gate
        ) Map.empty<string, Gate>
    (gates, initial |> Map.toSeq)

let rec solveGate (gates: Map<string, Gate>) (gate: string) = 
    let g = gates |> Map.find gate
    let x =
        match g.x with
        | Const c -> c
        | Complex c -> solveGate gates c
    let y =
        match g.y with
        | Const c -> c
        | Complex c -> solveGate gates c
    g.op x y

let bitsToInt (bits: int seq) = 
    bits |> Seq.fold (fun acc bit -> (acc <<< 1) ||| (bit |> uint64)) 0UL

let rec intToBits (n: uint64) = 
    if n = 0UL then []
    else
        (intToBits (n >>> 1)) @ [(int (n &&& 1UL))]

let bitsToStr bits = 
    bits |> Array.ofList |> Array.map (fun x -> '0' + (char x)) |> System.String

let solveFirst (input: Input) = 
    let gates, _ = input |> parseInput
    gates
    |> Map.keys
    |> Seq.filter (fun n -> n[0] = 'z')
    |> Seq.sortDescending
    |> Seq.map (fun n -> solveGate gates n)
    |> bitsToInt

let solveGates (gates: Map<string, Gate>) = 
    gates
    |> Map.keys
    |> Seq.filter (fun n -> n[0] = 'z')
    |> Seq.sortDescending
    |> Seq.map (fun n -> solveGate gates n)
    //|> bitsToInt

let findGates (gates: Map<string, Gate>) (start: string) (depth: int) = 
    let rec loop (start: string) (depth: int) (res: string list) = 
        if depth = 0 then
            res
        else
            let gate = gates |> Map.find start
            let parents = 
                match gate.x, gate.y with
                | Complex sx, Complex sy -> [sx; sy]
                | _, Complex sy -> [sy]
                | Complex sx, _ -> [sx]
                | _ -> []
            let res = res @ parents
            res @ (parents |> List.collect (fun p -> loop p (depth - 1) res))

    loop start depth []

let uniquePairs (arr: 'a array) = 
    [| 0 .. arr.Length - 1 |]
    |> Seq.collect (fun i -> seq {
        for k in i + 1 .. arr.Length - 1 -> (arr[i], arr[k])
    })

let solveSecond (input: Input) = 
    //Graphviz input
    let parts = input.lines |> List.split ((=) "")
    printfn "digraph {{"
    parts[1]
    |> List.iter (fun n -> 
        let split = n.Split ([| " -> "; " " |], System.StringSplitOptions.None)
        let l, op, r, res = split[0], split[1], split[2], split[3]
        printfn "%s -> %s_%s_%s" l l op r
        printfn "%s -> %s_%s_%s" r l op r
        printfn "%s_%s_%s -> %s" l op r res
    )
    printfn "}}"

    //let gates, initial = input |> parseInput
    //let xInput = initial |> Seq.filter (fun (k, _) -> k[0] = 'x') |> Seq.sortDescending |> Seq.map snd |> bitsToInt
    //let yInput = initial |> Seq.filter (fun (k, _) -> k[0] = 'y') |> Seq.sortDescending |> Seq.map snd |> bitsToInt
    //let expected = xInput + yInput
    //let expectedBits = intToBits expected
    
    //let bitLength = Seq.length initial / 2
    //for i in 0 .. bitLength - 1 do
    //    let bits = solveGates gates |> List.ofSeq |> List.rev
    //    let ref = expectedBits |> List.rev
    //    printfn "%d\n%s\n%s" i (bitsToStr ref) (bitsToStr bits)
    //    if bits[..i] <> ref[..i] then
    //        let zCurr = sprintf "z%02d" i
    //        let zStart = sprintf "z%02d" (i + 1) // + 1 to get all connected nodes starting from the lower adder
    //        let possibleWrong = ((findGates gates zCurr 2) @ (findGates gates zStart 2)) |> List.distinct
    //        let possibleWrong = zCurr :: zStart :: possibleWrong
    //        // TODO find the swap 

    //        printfn "diff at %d %s" i zStart
    //    printfn ""

    null :> obj
