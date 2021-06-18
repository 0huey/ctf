#!/usr/bin/env python3

from base64 import b64decode

import disasm

class Load:
    def __init__(self, prog_path):
        rom = DecodeRomFile(prog_path)

        self.program = disasm.Disassemble(rom)

    def getinstruction(self, position):
        return self.program[position]

    def getlen(self):
        return len(self.program)

    def __str__(self):
        s = str()

        for instr in self.program:
            s += str(instr) + '\n'

        return s


def DecodeRom(data):
    """
    Decode the program.
    Instructions are a 6 bit opcode followed by three 6 bit arguments
    written across byte boundries for a length of 3 bytes, each.

    Returns list of instruction tuples -> [(OPC, A, B, C), ... ]
    """

    if not len(data) % 3 == 0:
        raise Exception("Bad ROM length")

    rom = list()
    i = 0

    while i < len(data):
        ## read 24 bit instr
        instr = data[ i : i+3 ]

        ## get binary str of instr
        bits = str()
        for byte in instr:
            bits += format(byte, '08b')

        ## split instr binary into 4 6 bit parts
        OPC = int( bits[:6], 2 )
        A = int( bits[6:12], 2 )
        B = int( bits[12:18], 2 )
        C = int( bits[18:], 2 )

        rom.append( ( OPC, A, B, C ) )

        i += 3

    return rom


def DecodeRomFile(path):
    with open(path) as f:
        data = b64decode(f.read().strip())

    return DecodeRom(data)


if __name__ == '__main__' :
    from sys import argv

    if len(argv) > 1:
        rom_path = argv[1]

        for instr in DecodeRomFile(rom_path):
            print(instr)
