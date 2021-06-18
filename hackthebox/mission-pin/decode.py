#!/usr/bin/env python3

from sys import argv


def Main(binary):
    Parse(binary, 0x0001b985, 0x00092c40)


def Parse(binary, start, end):
    ch1 = " "
    ch2 = " "

    while start < end:
        channel = binary[start]
        char = chr(binary[start + 10])

        if channel == 1 and ch1[-1] != char:
            ch1 += char

        elif channel == 2 and ch2[-1] != char:
            ch2 += char

        start += 0x20

    print("ch1:", ch1)
    print("ch2:", ch2)


if __name__ == '__main__':
    if len(argv) == 2:
        with open(argv[1], 'rb') as f:
            Main(f.read())
