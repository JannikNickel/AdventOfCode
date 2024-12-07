module Day04
open Input
open Common

[<Literal>]
let name = "Ceres Search"

let patterns = [|
    [| ((-1, 1), 'M'); ((1, 1), 'M'); ((-1, -1), 'S'); ((1, -1), 'S') |]
    [| ((-1, 1), 'M'); ((1, 1), 'S'); ((-1, -1), 'M'); ((1, -1), 'S') |]
    [| ((-1, 1), 'S'); ((1, 1), 'S'); ((-1, -1), 'M'); ((1, -1), 'M') |]
    [| ((-1, 1), 'S'); ((1, 1), 'M'); ((-1, -1), 'S'); ((1, -1), 'M') |] |]

let rotate90 matrix = matrix |> List.rev |> List.transpose
let rotate180 = rotate90 >> rotate90
let rotate270 = rotate90 >> rotate90 >> rotate90

let diagonals (list: char list list) = 
    let rows = List.length list
    let cols = List.length list[0]
    let diag row col = 
        let limit = min (rows - row) (cols - col)
        let rLst = [ row .. (rows - 1) ] |> List.truncate limit
        let cLst = [ col .. (cols - 1) ] |> List.truncate limit
        List.map2 (fun r c -> list[r][c]) rLst cLst

    seq {
        for i in [ (rows - 1) .. -1 .. 0 ] -> diag i 0
        for i in [ 1 .. (cols - 1) ] -> diag 0 i
    } |> List.ofSeq

let transforms (source: char list list) = 
    [ source; source |> rotate90; source |> rotate180; source |> rotate270 ]
    |> Seq.collect (fun n -> [n; n |> diagonals])

let rec countSubstring (sub: string) (acc: int) (source: string) = 
    match source.IndexOf sub with
    | -1 -> acc
    | n -> countSubstring sub (acc + 1) source[(n + 1)..]

let isXmas ((x, y): int * int) (map: Map<(int * int), char>) = 
    let center = map |> Map.valueOrDefault (x, y) '0'
    center = 'A' && patterns
    |> Array.exists (fun pat -> 
        pat |> Array.forall (fun ((xOff, yOff), target) -> 
            (map |> Map.valueOrDefault (x + xOff, y + yOff) '0') = target
        )
    )

let solveFirst (input: Input) = 
    input.lines
    |> List.map List.ofSeq
    |> transforms
    |> Seq.concat
    |> Seq.sumBy (String.ofList >> (countSubstring "XMAS" 0))

let solveSecond (input: Input) = 
    let map = input.lines |> Parsing.map2d id id |> Map.ofSeq
    map
    |> Map.filter (fun key _ -> isXmas key map)
    |> Map.count
