# Advent of Code
Solutions for [Advent of Code](https://adventofcode.com/).

## Progress
| Year | Stars | Language |
|------|-------|----------|
| [2025](/2025) | In progress  | F#       |
| [2024](/2024) | 50 / 50 ⭐  | F#       |
| [2023](/2023) | 50 / 50 ⭐  | Python   |
| [2022](/2022) | 50 / 50 ⭐  | C#       |
| [2021](/2021) | 50 / 50 ⭐  | Rust     |
| [2020](/2020) | 50 / 50 ⭐  | C++      |
| [2019](/2019) | 50 / 50 ⭐  | C        |

## Input downloading
To download the puzzle inputs of an individual user, the `input_dl.py` script can be used. It is configured to work for all solutions and years in this repository.
```bash
python input_dl.py --year {2019..2024} [--day {1..25}] [--update] [--session SESSION]

--year <YEAR>       # The year of the input to download.
--day <DAY>         # The day of the input to download. Leave empty to download the whole year
--update            # Update/redownload existing input files
--session <SESSION> # Set the session cookie to log into the AOC website to download input
```

> [!NOTE]
> Advent of Code puzzle inputs differ by user. To download the correct input, the user has to be logged into the AOC website. To do that, the application will use the session cookie from the browser. If the session cookie is missing, the application will prompt the user to input it with instructions on how to retrieve it.
