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

let readSectors (map: char seq) = 
    let files, empty = map |> List.ofSeq |> List.map charToInt |> List.alternate
    let empty = if files.Length <> empty.Length then (empty @ [0]) else empty
    (files, empty)
    ||> List.mapi2 (fun i f e -> [ FileSector(f, i); EmptySector(e) ])
    |> List.concat
    |> Array.ofList

let findLastIdx predicate (lastIdx: int) (disk: Block array) = 
    let rec search i = 
        if i < 0 then -1
        elif predicate disk[i] then i
        else search (i - 1)

    search lastIdx

let compress (disk: Block array) = 
    let fCount = (disk |> Array.filter (fun n -> n.IsFile) |> Array.length)
    disk
    |> Array.fold (fun (i, last, acc) n -> 
        match i, n with
        | i, _ when i >= fCount -> (i + 1, last, Empty :: acc)
        | _, File _ -> (i + 1, last, n :: acc)
        | _, Empty -> 
            let lastIdx = disk |> findLastIdx (fun n -> n.IsFile) last
            (i + 1, lastIdx - 1, disk[lastIdx] :: acc)
    ) (0, disk.Length - 1, [])
    |> (fun (_, _, n) -> n)
    |> List.rev
    |> Array.ofList

let secSize (sec: Sector) = 
    match sec with
    | FileSector(len, _) -> len
    | EmptySector(len) -> len

let findSpace (disk: Sector array) (minSpace: int) = 
    let idx = disk |> Array.tryFindIndex (fun n -> n.IsEmptySector && secSize n >= minSpace)
    match idx with
    | Some i -> Some(i, disk[i])
    | _ -> None
    
let moveFile (file: Sector) (disk: Sector array): Sector array = 
    match file with
    | EmptySector _ -> failwith ""
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
                let space = (secSize disk[fIdx])
                let secSize = secSize sec
                let diff = secSize - len

                disk
                |> Array.removeAt fIdx
                |> Array.insertAt fIdx (EmptySector(space))
                |> Array.removeAt secIdx
                |> Array.insertAt secIdx file
                |> if diff > 0 then Array.insertAt (secIdx + 1) (EmptySector(diff)) else id
            else
                disk
        | _ -> disk

let compressSectors (disk: Sector array) = 
    let disk = disk |> Array.filter (fun n -> n |> secSize > 0)

    disk
    |> Array.filter (fun n -> match n with FileSector(_, _) -> true | _ -> false)
    |> Array.rev
    |> Array.fold (fun acc n -> moveFile n acc) disk

let expandSectors (disk: Sector array) = 
    disk
    |> Seq.filter (fun n -> n |> secSize > 0)
    |> Seq.collect (fun n -> 
        match n with
        | FileSector(len, id) -> [ for _ in [ 0 .. (len - 1)] -> File(id) ]
        | EmptySector(len) -> [ for _ in [ 0 .. (len - 1)] -> Empty ]
    )
    |> Array.ofSeq

let checksum disk = 
    disk |> Array.mapi (fun i n -> 
        (uint64 i) * (match n with | File id -> uint64 id | _ -> 0UL)
    )

let solveFirst (input: Input) = 
    input.raw
    |> expandDiskMap
    |> compress
    |> checksum
    |> Array.sum

let solveSecond (input: Input) = 
    input.raw
    |> readSectors
    |> compressSectors
    |> expandSectors
    |> checksum
    |> Array.sum
