import random

line: list = []

def set_width(width: int):
    line.clear()

    for i in range(0, width):
        line.append(0)

def effect(rept: int = 1):
    LINE_LOOKUP: list = ['.', ',', '-', '=', '#', '░', '▒', '▓', '█']
    STREAM_CHANCE: float = 0.05

    for each in range(0, rept):
        result: str = ""
        for i in range(0, len(line)):
            if line[i] == 0:
                if random.random() <= STREAM_CHANCE:
                    line[i] = random.randint(len(LINE_LOOKUP) // 2, len(LINE_LOOKUP) - 1)
                result += ' '
            else:
                result += LINE_LOOKUP[line[i]]
                line[i] -= 1
        print("\033[92;49m" + result)

set_width(80)

if __name__ == "__main__":
    import time
    for i in range(36): effect(); time.sleep(0.05)