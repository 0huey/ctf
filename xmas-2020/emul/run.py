#!/usr/bin/env python3

import rom
import cpu

from sys import argv

def Main(rom_path, socket=None):
    program = rom.Load(rom_path)

    emu = cpu.EMU10(program, socket)

    emu.run(True)


if __name__ == '__main__':
    if len(argv) == 2:
        Main(argv[1])

    elif len(argv) == 4:
        path = argv[1]
        host = argv[2]
        port = argv[3]

        Main(path, (host, int(port)))

    else:
        print("Usage:", argv[0], "ROM_PATH [HOST PORT]")
