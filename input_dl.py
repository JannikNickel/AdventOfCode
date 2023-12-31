import json
import os
from argparse import ArgumentParser
from shutil import get_terminal_size
from enum import Enum
from collections import defaultdict
from http.client import HTTPSConnection

FILE_DIR = os.path.dirname(__file__)
SESSION_FILE = os.path.join(FILE_DIR, ".session")
SESSION_KEY = "session"

AOC_DOMAIN = "adventofcode.com"
AOC_SESSION_COOKIE_NAME = "session"

PATHS = {
    2019: "2019/input/",
    2020: "2020/input/",
    2021: "2021/input/",
    2022: "2022/input/",
    2023: "2023/input/"
}

ANSI_ESC_INFO = "\x1B[90m"
ANSI_ESC_CLOSE = "\033[0m"

class DownloadResult(Enum):
    NONE = 0
    SUCCESS = 1
    NOT_AVAILABLE = 2
    INVALID_SESSION = 3
    SKIPPED = 4

def main():
    parser = ArgumentParser()
    parser.add_argument("--session", type = str, required = False, help = "The session cookie to use to fetch input")
    parser.add_argument("--year", type = int, required = True, choices = PATHS.keys(), help = "The year of the input to download")
    parser.add_argument("--day", type = int, required = False, choices = range(1, 25 + 1), help = "The day of the input to download")
    parser.add_argument("--update", action = "store_true", required = False, help = "Flag to redownload input data even if it exists")
    args = parser.parse_args()

    session = read_session()
    if args.session:
        session[SESSION_KEY] = args.session
        save_session(session)

    if SESSION_KEY not in session:
        update_session(session)

    pairs = []
    if args.day:
        pairs.append((args.year, args.day))
    else:
        pairs += [(args.year, i) for i in range(1, 25 + 1)]

    download_res = defaultdict(int)
    for i, (year, day) in enumerate(pairs):
        res = download_input(session, year, day, args.update)
        download_res[res] += 1
        p = (i + 1) / len(pairs)
        display_progress("Downloading input", p)
    print()

    for r in download_res.keys():
        print(f"> {r.name.replace("_", " ").title()}: {download_res[r]}")

def download_input(session: dict[str, object], year: int, day: int, update: bool) -> DownloadResult:
    path = f"{PATHS[year]}{day:02d}.txt"
    url = f"/{year}/day/{day}/input"

    if not update and os.path.exists(path):
        return DownloadResult.SKIPPED
    
    conn = HTTPSConnection(AOC_DOMAIN)
    headers = {
        "Cookie": f"{AOC_SESSION_COOKIE_NAME}={session[SESSION_KEY]}",
        "User-Agent": "https://github.com/JannikNickel/AdventOfCode"
    }
    conn.request("GET", url, headers = headers)
    response = conn.getresponse()
    if response.status == 200:
        data = response.read().decode("utf-8").rstrip("\n\r")
        with open(path, "w") as file:
            file.write(data)
        return DownloadResult.SUCCESS
    else:
        return DownloadResult.NOT_AVAILABLE if response.status == 404 else DownloadResult.INVALID_SESSION
    
def display_progress(message: str, progress: float):
    terminal_width = get_terminal_size().columns
    bar_width = min(terminal_width, 100) - len(f"{message} [] 100% ")
    fill_count = int(progress * bar_width)
    bar = f"[{"=" * fill_count}{" " * (bar_width - fill_count)}] {int(progress * 100)}%"
    print(f"\r{message} {bar}", end = "", flush = True)

def update_session(config: dict[str, object]):
    print("Advent of Code uses personalized input. To download your input, the session cookie from your browser is required!")
    print(f"{ANSI_ESC_INFO} > Log into Advent Of Code website\n > Open dev tools\n   Firefox:\n    > Select Storage tab and expand cookies\n   Chrome:\n    > Select Application tab\n    > Expand Cookies in the Storage category\n > Select https://adventofcode.com\n > Copy the \"session\" cookie value{ANSI_ESC_CLOSE}")
    session = input("Enter session token: ")
    config[SESSION_KEY] = session
    save_session(config)

def read_session() -> dict[str, object]:
    if os.path.exists(SESSION_FILE):
        with open(SESSION_FILE, "r") as file:
            return json.load(file)
    return {}

def save_session(config: dict[str, object]):
    with open(SESSION_FILE, "w") as file:
        json.dump(config, file, indent = True)

if __name__ == "__main__":
    main()
