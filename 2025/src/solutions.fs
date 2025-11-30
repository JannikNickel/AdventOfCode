module Solutions
open Input
open System.Reflection

type Solution = {
    name: string
    solveFirst: Input -> obj
    solveSecond: Input -> obj }

let private solutions = 
    let assembly = Assembly.GetExecutingAssembly ()
    assembly.GetTypes ()
    |> Array.filter (fun t -> t.IsClass && t.Namespace = null && t.Name.StartsWith "Day")
    |> Array.sortBy (fun t -> t.Name)
    |> Array.map (fun t -> 
        let name = (t.GetField ("name", BindingFlags.Public ||| BindingFlags.Static)).GetValue null :?> string
        let sFirst = t.GetMethod ("solveFirst", BindingFlags.Public ||| BindingFlags.Static)
        let solveFirst = fun (input: Input) -> sFirst.Invoke (null, [| input |])
        let sSecond = t.GetMethod ("solveSecond", BindingFlags.Public ||| BindingFlags.Static)
        let solveSecond = fun (input: Input) -> sSecond.Invoke (null, [| input |])

        { name = name; solveFirst = solveFirst; solveSecond = solveSecond }
    )
    |> List.ofArray

let count = solutions.Length
let getSolution day = solutions[day - 1]
