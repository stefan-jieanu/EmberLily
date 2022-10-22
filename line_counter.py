from pathlib import Path


def main():
    lines = 0;

    for path in Path('emberlily/src').rglob('*.cpp'):
        lines += len(open(path).readlines())

    for path in Path('emberlily/src').rglob('*.hpp'):
        lines += len(open(path).readlines())

    print("Wow! Your program is " + str(lines) + " lines of code! °˖✧◝(⁰▿⁰)◜✧˖° ") 

if __name__ == "__main__":
    main();