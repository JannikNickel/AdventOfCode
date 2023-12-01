class Input:
    def __init__(self, path: str):
        with open(path, "r") as file:
            self._raw = file.read()
            file.seek(0)
            self._lines = file.readlines()

    @property
    def raw(self) -> str:
        return self._raw
    
    @property
    def lines(self) -> [str]:
        return self._lines
