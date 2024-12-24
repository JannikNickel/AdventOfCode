module Day24
open Input
open Common

[<Literal>]
let name = "Crossed Wires"

type GateInput = Const of string | Complex of string

type Gate = {
    x: GateInput
    y: GateInput
    op: (int -> int -> int) }

type State = {
    gates: Map<string, Gate>
    swaps: List<string * string>
    start: int }

let matchOp = function
    | "AND" -> (&&&)
    | "OR" -> (|||)
    | "XOR" -> (^^^)
    | _ -> failwith ""

let parseGateInp (consts: Map<string, int>) (inp: string) = 
    if consts |> Map.containsKey inp then Const inp else Complex inp

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
    (gates, initial)

let bitsToInt (bits: int seq) = 
    bits |> Seq.fold (fun acc bit -> (acc <<< 1) ||| (bit |> uint64)) 0UL

let rec intToBits (n: uint64) = 
    if n = 0UL then []
    else
        (intToBits (n >>> 1)) @ [(int (n &&& 1UL))]

let bitsToStr bits = 
    bits |> Array.ofList |> Array.map (fun x -> '0' + (char x)) |> System.String

let uniquePairs (arr: 'a array) = 
    [| 0 .. arr.Length - 1 |]
    |> Seq.collect (fun i -> seq {
        for k in i + 1 .. arr.Length - 1 -> (arr[i], arr[k])
    })

let rec solveGate (gates: Map<string, Gate>) (constants: Map<string, int>) (gate: string) (closed: Set<string>) = 
    let findInp (closed: Set<string>) (inp: GateInput) = 
        match inp with
        | Const c -> constants |> Map.find c
        | Complex c -> solveGate gates constants c closed

    if closed |> Set.contains gate then
        failwith "loop"
    else
        let closed = closed |> Set.add gate
        let g = gates |> Map.find gate
        let x = g.x |> findInp closed
        let y = g.y |> findInp closed
        g.op x y

let solveGates (constants: Map<string, int>) (gates: Map<string, Gate>) = 
    gates
    |> Map.keys
    |> Seq.filter (fun n -> n[0] = 'z')
    |> Seq.sortDescending
    |> Seq.map (fun n -> solveGate gates constants n Set.empty)

let findGates (gates: Map<string, Gate>) (start: string) (depth: int) = 
    let rec loop (curr: string) (depth: int) (res: string list) = 
        if depth = 0 then
            res
        else
            let gate = gates |> Map.find curr
            let parents = 
                match gate.x, gate.y with
                | Complex sx, Complex sy -> [sx; sy]
                | _, Complex sy -> [sy]
                | Complex sx, _ -> [sx]
                | _ -> []
            res @ parents @ (parents |> List.collect (fun p -> loop p (depth - 1) res))

    loop start depth []

let swapGates (a: string) (b: string) (gates: Map<string, Gate>) = 
    let aVal = gates |> Map.find a
    let bVal = gates |> Map.find b
    gates
    |> Map.add a bVal
    |> Map.add b aVal

let testGate (constants: Map<string, int>) (gates: Map<string, Gate>) (expected: int list) (len: int) = 
    let test = solveGates constants gates |> List.ofSeq |> List.rev
    test[..len] = expected[..len]

let findExpected (constants: Map<string, int>) = 
    let xInput = constants |> Map.toSeq |> Seq.filter (fun (k, _) -> k[0] = 'x') |> Seq.sortDescending |> Seq.map snd |> bitsToInt
    let yInput = constants |> Map.toSeq |> Seq.filter (fun (k, _) -> k[0] = 'y') |> Seq.sortDescending |> Seq.map snd |> bitsToInt
    let expected = xInput + yInput
    let expectedBits = intToBits expected
    expectedBits |> List.rev

let findWorkingSwaps (constants: Map<string, int>) (gates: Map<string, Gate>) (possible: string list) (expected: int list) (len: int) = 
    possible
    |> Array.ofList
    |> uniquePairs
    |> Seq.filter (fun (a, b) -> 
        let gates = gates |> swapGates a b
        try
            let test = testGate constants gates expected len
            let rnd = System.Random ()
            let tests = 
                [0..100] |> List.forall (fun _ -> 
                    let constants = constants |> Map.map (fun _ _ -> if rnd.NextDouble() > 0.5 then 1 else 0)
                    let expected = constants |> findExpected
                    testGate constants gates expected len
                )
            test && tests
        with
        | _ -> false
    )
    |> List.ofSeq

let processState (constants: Map<string, int>) (expected: int list) (state: State) = 
    let rec findError i nextStates = 
        if i >= (expected |> List.length) then
            nextStates
        else
            if not (testGate constants state.gates expected i) then
                let zCurr = sprintf "z%02d" i
                let zStart = sprintf "z%02d" (i + 1)
                let possibleWrong = zCurr :: (findGates state.gates zCurr 2) @ (findGates state.gates zStart 2)
                let workingSwaps = findWorkingSwaps constants state.gates possibleWrong expected i
                let next =
                    workingSwaps
                    |> List.map (fun (a, b) -> {
                        gates = state.gates |> swapGates a b
                        swaps = (a, b) :: state.swaps
                        start = i + 1
                    })
                nextStates @ next
            else
                findError (i + 1) nextStates

    findError state.start []

let rec findSwappedGates (constants: Map<string, int>) (expected: int list) (states: State list) = 
    match states with
    | state :: tail -> 
        let states = tail |> List.filter (fun n -> n.swaps |> List.length <= 4)
        if testGate constants state.gates expected (expected |> List.length) then
            state.swaps
            |> List.collect (fun (a, b) -> [a; b])
            |> List.sort
        else
            let next = processState constants expected state
            findSwappedGates constants expected (states @ next)
    | _ -> failwith ""

let solveFirst (input: Input) = 
    let gates, constants = input |> parseInput
    gates
    |> solveGates constants
    |> bitsToInt

let solveSecond (input: Input) = 
    let gates, constants = input |> parseInput
    let expected = constants |> findExpected
    let initial = { gates = gates; swaps = []; start = 0 }
    findSwappedGates constants expected [initial]
    |> Array.ofList
    |> String.concat ","
