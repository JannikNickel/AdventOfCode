module Day10
open Input
open Common

[<Literal>]
let name = "Factory"

type Machine = 
    { Target: char list
      Buttons: int list list
      Joltages: int list }

let parseMachine (line: string) = 
    let targets = line.[ (line.IndexOf '[' + 1) .. (line.IndexOf ']' - 1) ] |> Seq.toList
    let buttons = Parsing.matches "\((.*?)\)" line |> List.map Parsing.ints
    let joltages = line.[ (line.IndexOf '{' + 1) .. (line.IndexOf '}' - 1) ] |> Parsing.ints
    { Target = targets; Buttons = buttons; Joltages = joltages }

let parseInput (input: Input) =
    input.lines |> List.map parseMachine

let startMachine (machine: Machine) = 
    let targetMask =
        machine.Target
        |> List.mapi (fun i c -> if c = '#' then (1 <<< i) else 0)
        |> List.sum

    let buttonMasks = 
        machine.Buttons
        |> List.map (fun btn -> btn |> List.fold (fun acc i -> acc ||| (1 <<< i)) 0)

    let rec bfs visited queue = 
        match Queue.dequeue queue with
        | None -> -1
        | Some ((state, presses), remainingQueue) -> 
            if state = targetMask then
                presses
            else
                let nextStates = 
                    buttonMasks
                    |> List.map (fun n -> state ^^^ n)
                    |> List.filter (fun s -> not (Set.contains s visited))

                let newVisited = List.fold (fun acc s -> Set.add s acc) visited nextStates
                let newQueue = List.fold (fun acc s -> Queue.enqueue (s, presses + 1) acc) remainingQueue nextStates

                bfs newVisited newQueue
    
    bfs (Set.singleton 0) (Queue.enqueue (0, 0) Queue.empty)

let rec distribute (buckets: int) (items: int) = 
    seq {
        if buckets = 1 then
            yield [ items ]
        else
            for i in 0 .. items do
                for prev in distribute (buckets - 1) (items - i) do
                    yield i :: prev
    }

let nextPresses (state: int array) (target: int array) (buttons: int list list) (pressAmounts: int list) = 
    let presses = List.zip buttons pressAmounts
    let simulatePresses idx curr = 
        let added = presses |> List.sumBy (fun (btn, count) -> if btn |> List.contains idx then count else 0)
        let newValue = curr + added
        if newValue <= target.[idx] then Some newValue else None

    let rec mapState i acc = 
        if i >= Array.length state then
            Some (List.rev acc |> List.toArray)
        else
            match simulatePresses i state.[i] with
            | Some v -> mapState (i + 1) (v :: acc)
            | _ -> None

    mapState 0 []

let rec findMinPresses (state: int array) (target: int array) (buttons: int list list) = 
    if state = target then
        Some 0
    else
        let bestCandidate = 
            state
            |> Array.mapi (fun idx value -> idx, target.[idx] - value)
            |> Array.filter (fun (_, rem) -> rem > 0)
            |> Array.map (fun (idx, rem) -> 
                let matchingCount = buttons |> List.filter (fun btn -> btn |> List.contains idx) |> List.length
                (idx, rem, matchingCount)
            )
            |> Array.sortBy (fun (_, _, c) -> c)
            |> Array.tryHead

        match bestCandidate with
        | None -> None
        | Some (idx, pressAmounts, _) ->
            let relevant, remaining = buttons |> List.partition (fun btn -> btn |> List.contains idx)

            if List.isEmpty relevant then
                None
            else
                distribute (List.length relevant) pressAmounts
                |> Seq.map (fun presses -> 
                    match nextPresses state target relevant presses with
                    | Some newState -> 
                        match findMinPresses newState target remaining with
                        | Some minPresses -> Some (minPresses + pressAmounts)
                        | _ -> None
                    | _ -> None
                )
                |> Seq.choose id
                |> Seq.sort
                |> Seq.tryHead

let configureJoltage (machine: Machine) =
    let machineSize = List.length machine.Joltages
    let initialState = Array.zeroCreate machineSize
    let targetState = machine.Joltages |> List.toArray
    let enabledButtons = machine.Buttons
    findMinPresses initialState targetState enabledButtons

let solveFirst (input: Input) = 
    input
    |> parseInput
    |> List.map startMachine
    |> List.sum

let solveSecond (input: Input) = 
    input
    |> parseInput
    |> Array.ofList
    |> Array.Parallel.sumBy (configureJoltage >> Option.defaultValue 0)
