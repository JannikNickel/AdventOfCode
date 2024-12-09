module Day09
open Input
open Common

[<Literal>]
let name = "Disk Fragmenter"

type Block = 
| File of int
| Empty

type Sector = 
| FileSector of int * int
| EmptySector of int

let inline charToInt c = int c - int '0'

let expandDiskMap (map: char seq) = 
    let files, empty = map |> List.ofSeq |> List.map charToInt |> List.alternate
    let empty = if files.Length <> empty.Length then (empty @ [0]) else empty
    (files, empty)
    ||> List.mapi2 (fun i f e -> 
        [ for _ in [ 0 .. (f - 1) ] -> File i ] @ [ for _ in [ 0 .. (e - 1) ] -> Empty ]
    )
    |> List.concat
    |> Array.ofList

let parseSectors (map: char seq) = 
    let files, empty = map |> List.ofSeq |> List.map charToInt |> List.alternate
    let empty = if files.Length <> empty.Length then (empty @ [0]) else empty
    (files, empty)
    ||> List.mapi2 (fun i f e -> 
        [ FileSector(f, i); EmptySector(e) ]
    )
    |> List.concat
    |> Array.ofList

let findLastIdx predicate (lastIdx: int) (disk: Block array) = 
    let mutable res = -1
    for i in lastIdx .. -1 .. 0 do
        if res = -1 && predicate disk[i] then
            res <- i
    res

let compress (disk: Block array) = 
    let fCount = (disk |> Array.filter (fun n -> n <> Empty) |> Array.length)
    let mutable last = disk.Length - 1
    disk
    |> Array.mapi (fun i n -> 
        if i >= fCount then
            Empty
        else
            match n with
            | File _ -> n
            | Empty -> 
                let lastIdx = disk |> findLastIdx (fun n -> n <> Empty) last
                last <- lastIdx - 1
                disk[lastIdx]
    )

let findSector (fileSecs: Sector array) (maxSize: int) = 
    let idx = 
        fileSecs
        |> Array.tryFindIndex (fun n -> 
            match n with
            | FileSector(len, _) -> len <= maxSize
            | _ -> false
        )
    match idx with
    | Some i -> Some(i, fileSecs[i])
    | _ -> None

let secSize (sec: Sector) = 
    match sec with
    | FileSector(len, _) -> len
    | EmptySector(len) -> len

let findSpace (disk: Sector array) (minSpace: int) = 
    let idx = disk |> Array.tryFindIndex (fun n -> n.IsEmptySector && secSize n >= minSpace)
    match idx with
    | Some i -> Some(i, disk[i])
    | _ -> None
    
let solve (file: Sector) (disk: Sector array): Sector array = 
    match file with
    | EmptySector(_) -> failwith ""
    | FileSector(len, _id) -> 
        let sec = findSpace disk len
        match sec with
        | Some (secIdx, sec) -> 
            let fIdx = disk |> Array.findIndex (fun n ->
                match n with
                | FileSector(_, fId) -> fId = _id
                | _ -> false
            )

            if secIdx <= fIdx then
                let lIdx = fIdx - 1
                let rIdx = fIdx + 1
                let validL = lIdx >= 0 && disk[lIdx].IsEmptySector
                let validR = rIdx < disk.Length && disk[rIdx].IsEmptySector
                let space =
                    (secSize disk[fIdx])
                    //+ if validL then (disk[lIdx] |> secSize) else 0
                    //+ if validR then (disk[rIdx] |> secSize) else 0
                let disk =
                    disk
                    |> Array.removeAt fIdx
                    |> Array.insertAt fIdx (EmptySector(space))
                    //|> if validR then (Array.removeAt rIdx) else id
                    //|> if validL then (Array.removeAt lIdx) else id

                let secSize = secSize sec
                let diff = secSize - len
                let disk = 
                    disk
                    |> Array.removeAt secIdx
                    |> Array.insertAt secIdx file
                    |> if diff > 0 then Array.insertAt (secIdx + 1) (EmptySector(diff)) else id
                disk
            else
                disk
        | _ -> disk

let compressSectors (disk: Sector array): Sector list = 
    let disk = disk |> Array.filter (fun n -> n |> secSize > 0)
    let files =
        disk
        |> Array.filter (fun n -> match n with FileSector(_, _) -> true | _ -> false)
        |> Array.rev

    files
    |> Array.fold (fun acc n -> 
        solve n acc
    ) disk
    |> List.ofArray

let checksum disk = 
    disk
    |> Array.mapi (fun i n -> 
        i * (match n with | File id -> id | _ -> 0)
    )

let expandSectors disk = 
    disk
    |> Seq.filter (fun n -> n |> secSize > 0)
    |> Seq.collect (fun n -> 
        match n with
        | FileSector(len, id) -> [for _ in [ 0 .. (len - 1)] -> File(id) ]
        | EmptySector(len) -> [for _ in [ 0 .. (len - 1)] -> Empty ]
    )
    |> Array.ofSeq

let dbgPrint (list: Sector list) = 
    list
    |> List.iter (fun n -> 
        let s =
            match n with
            | FileSector(len, id) -> [ for _ in [0..(len - 1)] -> sprintf "%d" id ]
            | EmptySector(len) -> [ for _ in [0..(len - 1)] -> sprintf "." ]
        printf "%A" s
    )

let solveFirst (input: Input) = 
    let disk = input.raw |> expandDiskMap |> compress |> checksum
    let mutable sum = 0UL
    for v in disk do
        sum <- sum + (v |> uint64)
    sum

let solveSecond (input: Input) = 
    let disk = input.raw |> parseSectors
    let disk = disk |> compressSectors |> List.filter (fun n -> n |> secSize > 0)
    let disk = disk |> expandSectors
    let disk = disk |> checksum
    let mutable sum = 0UL
    for v in disk do
        sum <- sum + (v |> uint64)
    sum
