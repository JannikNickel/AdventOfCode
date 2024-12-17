module Day17
open Input
open Common

[<Literal>]
let name = "Chronospatial Computer"

type Instruction = Adv | Bxl | Bst | Jnz | Bxc | Out | Bdv | Cdv

type CPU = {
    aReg: uint64
    bReg: uint64
    cReg: uint64
    ptr: int
    output: uint64 list
}

let matchInstruction = function
    | 0UL -> Adv
    | 1UL -> Bxl
    | 2UL -> Bst
    | 3UL -> Jnz
    | 4UL -> Bxc
    | 5UL -> Out
    | 6UL -> Bdv
    | 7UL -> Cdv
    | _ -> failwith ""

let parseInput (input: Input) = 
    let aReg = input.lines[0] |> Parsing.numbers uint64 |> List.head
    let bReg = input.lines[1] |> Parsing.numbers uint64 |> List.head
    let cReg = input.lines[2] |> Parsing.numbers uint64 |> List.head
    let instructions = input.lines[4] |> Parsing.numbers uint64 |> Array.ofList
    (instructions, aReg, bReg, cReg)

let combo (literal: uint64) (cpu: CPU) = 
    match literal with
    | n when n <= 3UL -> n
    | 4UL -> cpu.aReg
    | 5UL -> cpu.bReg
    | 6UL -> cpu.cReg
    | _ -> failwith ""

let rec simulate (instructions: uint64 array) (cpu: CPU) = 
    if cpu.ptr >= instructions.Length then
        cpu.output |> List.rev |> Array.ofList
    else
        let inst = instructions |> Array.item cpu.ptr |> matchInstruction
        let literal = instructions |> Array.tryItem (cpu.ptr + 1) |> Option.defaultValue 0UL
        let cpu = { cpu with ptr = cpu.ptr + 2 }
        let cpu = 
            match inst with
            | Adv -> 
                let denum = pown 2UL (int (cpu |> combo literal))
                { cpu with aReg = (cpu.aReg / denum) }
            | Bxl ->
                { cpu with bReg = (cpu.bReg ^^^ literal) }
            | Bst ->
                { cpu with bReg = ((cpu |> combo literal) % 8UL) }
            | Jnz -> 
                if cpu.aReg <> 0UL then { cpu with ptr = int literal }
                else cpu
            | Bxc ->
                { cpu with bReg = (cpu.bReg ^^^ cpu.cReg) }
            | Out ->
                { cpu with output = ((cpu |> combo literal) % 8UL) :: cpu.output }
            | Bdv -> 
                let denum = pown 2UL (int (cpu |> combo literal))
                { cpu with bReg = (cpu.aReg / denum) }
            | Cdv -> 
                let denum = pown 2UL (int (cpu |> combo literal))
                { cpu with cReg = (cpu.aReg / denum) }

        simulate instructions cpu

let findInput (instructions: uint64 array) (bReg: uint64) (cReg: uint64) = 
    // Disassembly shows the whole programs loops while A != 0
    // Before jumping back to the start, A-Register is right-shifted by 3 bits
    let shift = pown 2UL 3
    let inputLength = instructions |> Array.length
    [| 1 .. (instructions |> Array.length) |]
    |> Array.fold (fun possible len -> 
        let comp = instructions[inputLength - len ..]
        possible
        |> Seq.collect (fun start -> 
            [| 0UL .. shift |]
            |> Seq.map (fun off -> 
                let aReg = start * shift + off
                let output = simulate instructions { aReg = aReg; bReg = bReg; cReg = cReg; ptr = 0; output = [] }
                (output, aReg)
            )
            |> Seq.filter (fun n -> fst n = comp)
            |> Seq.map snd
        )
    ) [0UL]

let solveFirst (input: Input) = 
    let instructions, aReg, bReg, cReg = input |> parseInput
    let output = simulate instructions { aReg = aReg; bReg = bReg; cReg = cReg; ptr = 0; output = [] }
    output |> Array.map string |> String.concat ","

let solveSecond (input: Input) = 
    let instructions, _, bReg, cReg = input |> parseInput
    findInput instructions bReg cReg |> Seq.min
