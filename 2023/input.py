class Input:
    def __init__(self, path: str):
        with open(path, "r") as file:
            self._raw = file.read()
            file.seek(0)
            self._lines = [line.rstrip() for line in file.readlines()]

    @property
    def raw(self) -> str:
        return self._raw
    
    @property
    def lines(self) -> list[str]:
        return self._lines
