import time

class ProgressBar(object):
    _empty: str
    _fill: str
    _left: str
    _length: int
    _right: int
    _progress: int = 0

    def __init__(self, length = 16, left = "[", fill = "#", empty = " ", right = "]"):
        self._length = length
        self._left = left
        self._fill = fill
        self._empty = empty
        self._right = right

        fill_string: str = ""
        for i in range(0, length):
            fill_string += fill if i < self._progress else empty
        print(left + fill_string + right)
    
    def set_progress(self, progress: int):
        self._progress = progress
        fill_string: str = "\033[F" + self._left
        for i in range(0, self._length):
            fill_string += self._fill if i < self._progress else self._empty
        print(fill_string + self._right)

def __main__():
    debug_bar = ProgressBar(64, "<", "=", "-", ">")
    for i in range(0, 65):
        debug_bar.set_progress(i)
        time.sleep(0.1)
    print("Done!")


if __name__ == "__main__":
    __main__()