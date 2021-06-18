#!/usr/bin/env python3

import instructions


def Disassemble(decoded_rom):
    instructions = list()

    for instr in decoded_rom:
        instructions.append( __Disassemble(instr) )

    return instructions


def __Disassemble(instr):
    """
    Input four six bit ints of tuple:
    (OPC, A, B, C)

    OPC = 21 * cond + op + 1
    """

    OPC, A, B, C = instr

    if OPC == 0:
        return instructions.HALT()

    cond = GetCondition(OPC)

    op = GetOpcode(OPC)

    args = (cond, op, A, B, C)

    if 0 <= op <= 1:
        return instructions.ADD(args)

    elif op == 2:
        return instructions.SUB(args)

    elif op == 3:
        return instructions.CMP(args)

    elif 4 <= op <= 5:
        return instructions.OR(args)

    elif 6 <= op <= 7:
        return instructions.XOR(args)

    elif Octal("010") <= op <= Octal("011"):
         return instructions.AND(args)

    elif op == Octal("012"):
        return instructions.SHIFT(args)

    elif op == Octal("013"):
        return instructions.SHL(args)

    elif op == Octal("014"):
        return instructions.SHR(args)

    elif op == Octal("015"):
        return instructions.LD(args)

    elif op == Octal("016"):
        return instructions.ST(args)

    elif op == Octal("017"):
        return instructions.LBL(args)

    elif op == Octal("020"):
        return instructions.JUP(args)

    elif op == Octal("021"):
        return instructions.JDN(args)

    elif op == Octal("021"):
        return instructions.JDN(args)

    elif op == Octal("022"):
        return instructions.IO(args)

    elif op == Octal("023"):
        return instructions.MUL(args)

    elif op == 20:
        return instructions.HALT()

    else:
        raise ValueError("Bad opcode:", op, "instr:", instr)


def GetCondition(OPC):
    cond = (OPC - 1) // 21

    if cond == 0:
        return None

    elif cond == 1:
        return True

    elif cond == 2:
       return False

    else:
        raise ValueError("Bad Condition?:", OPC, cond)


def GetOpcode(OPC):
    return (OPC - 1) % 21


def Octal(s):
    return int(s, 8)


if __name__ == '__main__':
    from sys import argv
    import rom

    if len(argv) > 1:
        rom_path = argv[1]

        decoded_rom = rom.DecodeRomFile(rom_path)

        for i, instr in enumerate(decoded_rom):
            print(i, __Disassemble(instr), "\t", instr)
