open System
open System.Diagnostics
open Input

module Messages =
    [<Literal>]
    let errExpectedValue = "Expected value after \"%s\"!"
    [<Literal>]
    let errInvalidInt = "Invalid integer value for \"%s\"!"
    [<Literal>]
    let errUnknownParam = "Unknown parameter: \"%s\"!"
    [<Literal>]
    let errDayOutOfRange = "Day %d is not implemented!"

module Program =
    [<Literal>]
    let defColor = ConsoleColor.DarkGray
    [<Literal>]
    let resColor = ConsoleColor.White

    type CliOptions = {
        all: bool
        days: int list }
    with
        static member Default = {
            all = false
            days = [] }

    let parseCli argv =
        let matchNum param (tail: string list) =
            match tail with
            | [] -> Error (sprintf Messages.errExpectedValue param)
            | head :: _ ->
                match (Int32.TryParse head) with
                | (true, value) -> Ok value
                | (false, _) -> Error (sprintf Messages.errInvalidInt param)

        let rec parseArg args options = 
            match args with
            | ("--all" | "-a") :: tail -> parseArg tail { options with all = true }
            | (("--day" | "-d") as dayParam) :: tail ->
                match matchNum dayParam tail with
                | Ok value ->
                    match value with
                    | v when v <= 0 || v > Solutions.count -> failwith (sprintf Messages.errDayOutOfRange value)
                    | v -> parseArg tail[1..] { options with days = ( options.days @ [ v ] ) }
                | Error err -> failwith err
            | head :: _ -> failwith (sprintf Messages.errUnknownParam head)
            | [] -> options

        parseArg argv CliOptions.Default

    let printfnc color fmt = 
        Printf.kprintf (fun message -> 
            let original = Console.ForegroundColor
            Console.ForegroundColor <- color
            Console.WriteLine message
            Console.ForegroundColor <- original
        ) fmt

    let fnTimed fn = 
        let stopwatch = Stopwatch.StartNew ()
        let result = fn ()
        stopwatch.Stop()
        (result, stopwatch.Elapsed.TotalMilliseconds)

    let solvePart (fn: Input -> obj) input part = 
        printfnc defColor "Solving part %d..." part
        let (result, duration) = fnTimed (fun () -> fn input)
        printfnc defColor "Solved Part %d (%.2fms)\nResult:" part duration
        printfnc resColor "%A\n" result

    let runSolution day = 
        let input = Input.load (sprintf "./input/%02d.txt" day)
        let solution = Solutions.getSolution day
        printfnc defColor "Preparing Solution %d - %s" day solution.name
        solvePart solution.solveFirst input 1
        solvePart solution.solveSecond input 2

    [<EntryPoint>]
    let main argv = 
        let solutions = 
            try
                let options = parseCli (List.ofArray argv)
                match options with
                | { all = true } -> [1..Solutions.count]
                | { days = head :: tail } -> head :: tail
                | _ -> [ Solutions.count ]
            with
            | ex -> eprintfn "%s" ex.Message; []

        solutions |> function
        | [] -> 1
        | solutions -> solutions |> List.iter runSolution; 0
