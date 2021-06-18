class OPCODE:
    def __init__(self, args):
        self.cond, self.op, self.a, self.b, self.c = args

        ### overwrite in subclass
        self.mnemonic = "INV"

    def __str__(self):
        if self.cond == None:
            cond = " "

        elif self.cond:
            cond = '+'

        else:
            cond = '-'

        return "{} {} {}".format(cond, self.mnemonic, self.print_args())

    def print_args(self):
        pass

    def exe(self, cpu):
        if self.cond == None or self.cond == cpu.getcondition():
            self._exe(cpu)

        cpu.step()

    ## each subclass must overwrite
    def _exe(self, cpu):
        raise

    def getcondition(self):
        return self.cond

    ## overwritten in LBL; called by JDN / JUP
    def getlabel(self):
        return None


class ADD(OPCODE):
    def __init__(self, args):
        OPCODE.__init__(self, args)

        self.mnemonic = "add"

        if self.op == 0:
            self.immediate = False
        else:
            self.immediate = True

    def print_args(self):
        dest = 'r' + str(self.a)
        arg1 = 'r' + str(self.b)

        if self.immediate:
            arg2 = hex(self.c)
        else:
            arg2 = 'r' + str(self.c)

        return "{}, {}, {}".format(dest, arg1, arg2)

    def _exe(self, cpu):
        ram = cpu.getram()

        dest = self.a

        arg1 = ram.read( self.b )

        if self.immediate:
            arg2 = self.c
        else:
            arg2 = ram.read(self.c)

        result = (arg1 + arg2) % 64

        ram.write(dest, result)


class SUB(OPCODE):
    def __init__(self, args):
        OPCODE.__init__(self, args)

        self.mnemonic = "sub"

    def print_args(self):
        dest = 'r' + str(self.a)
        arg1 = 'r' + str(self.b)
        arg2 = 'r' + str(self.c)

        return "{}, {}, {}".format(dest, arg1, arg2)

    def _exe(self, cpu):
        ram = cpu.getram()

        dest = self.a
        arg1 = ram.read(self.b)
        arg2 = ram.read(self.c)

        result = arg1 - arg2

        if result < 0:
            result = 64 - abs(result)

        ram.write(dest, result)


class CMP(OPCODE):
    def __init__(self, args):
        OPCODE.__init__(self, args)

        self.mnemonic = "cmp"

        if self.a // 8 == 0:
            self.immediate = None

        elif self.a // 8 == 2:
            self.immediate = 'c'

        elif self.a // 8 == 3:
            self.immediate = 'b'

        else:
            ### docs skipped over a // 8 == 1; but that didnt come up in program
            raise Exception("unknown immediacy in compare:", args)

        self.cc = self.a % 8

    def print_args(self):
        cc = {
            0 : "tr",
            1 : "fa",
            2 : "eq",
            3 : "ne",
            4 : "sl",
            5 : "sg",
            6 : "ul",
            7 : "ug"
        }

        if self.immediate == None:
            b = 'r' + str(self.b)
            c = 'r' + str(self.c)

        elif self.immediate == 'c':
            b = 'r' + str(self.b)
            c = hex(self.c)

        elif self.immediate == 'b':
            b = hex(self.b)
            c = 'r' + str(self.c)

        return "{} {}, {}".format(cc[self.cc], b, c)

    def _exe(self, cpu):
        ram = cpu.getram()

        if self.immediate == None:
            arg1 = ram.read(self.b)
            arg2 = ram.read(self.c)

        elif self.immediate == 'c':
            arg1 = ram.read(self.b)
            arg2 = self.c

        elif self.immediate == 'b':
            arg1 = self.b
            arg2 = ram.read(self.c)

        # its apparently possible to compare nothing,
        # but this didnt come up in the program
        # tr ??
        if self.cc == 0:
            raise Exception("cmp tr unimplemented")

        # fa ??
        elif self.cc == 1:
            raise Exception("cmp fa unimplemented")

        # eq
        elif self.cc == 2:
            if arg1 == arg2:
                cpu.setcondition(True)
            else:
                cpu.setcondition(False)

        # ne
        elif self.cc == 3:
            if arg1 != arg2:
                cpu.setcondition(True)
            else:
                cpu.setcondition(False)

        # signed less than
        elif self.cc == 4:
            arg1 = getsigned(arg1)
            arg2 = getsigned(arg2)

            if arg1 < arg2:
                cpu.setcondition(True)
            else:
                cpu.setcondition(False)

        # signed greater than
        elif self.cc == 5:
            arg1 = getsigned(arg1)
            arg2 = getsigned(arg2)

            if arg1 > arg2:
                cpu.setcondition(True)
            else:
                cpu.setcondition(False)

        # unsigned less than
        elif self.cc == 6:
            if arg1 < arg2:
                cpu.setcondition(True)
            else:
                cpu.setcondition(False)

        # unsigned greater than
        elif self.cc == 7:
            if arg1 > arg2:
                cpu.setcondition(True)
            else:
                cpu.setcondition(False)


class OR(OPCODE):
    def __init__(self, args):
        OPCODE.__init__(self, args)

        self.mnemonic = "or"

        if self.op == 4:
            self.immediate = False
        else:
            self.immediate = True

    def print_args(self):
        dest = 'r' + str(self.a)
        arg1 = 'r' + str(self.b)

        if self.immediate:
            arg2 = hex(self.c)
        else:
            arg2 = 'r' + str(self.c)

        return " {}, {}, {}".format(dest, arg1, arg2)

    def _exe(self, cpu):
        ram = cpu.getram()

        dest = self.a
        arg1 = ram.read(self.b)

        if self.immediate:
            arg2 = self.c
        else:
            arg2 = ram.read(self.c)

        result = arg1 | arg2

        ram.write(dest, result)


class XOR(OPCODE):
    def __init__(self, args):
        OPCODE.__init__(self, args)

        self.mnemonic = "xor"

        if self.op == 6:
            self.immediate = False
        else:
            self.immediate = True

    def print_args(self):
        dest = 'r' + str(self.a)
        arg1 = 'r' + str(self.b)

        if self.immediate:
            arg2 = hex(self.c)
        else:
            arg2 = 'r' + str(self.c)

        return "{}, {}, {}".format(dest, arg1, arg2)

    def _exe(self, cpu):
        ram = cpu.getram()

        dest = self.a
        arg1 = ram.read(self.b)

        if self.immediate:
            arg2 = self.c
        else:
            arg2 = ram.read(self.c)

        result = arg1 ^ arg2

        ram.write(dest, result)


class AND(OPCODE):
    def __init__(self, args):
        OPCODE.__init__(self, args)

        self.mnemonic = "and"

        if self.op == 8:
            self.immediate = False
        else:
            self.immediate = True

    def print_args(self):
        dest = 'r' + str(self.a)
        arg1 = 'r' + str(self.b)

        if self.immediate:
            arg2 = hex(self.c)
        else:
            arg2 = 'r' + str(self.c)

        return "{}, {}, {}".format(dest, arg1, arg2)

    def _exe(self, cpu):
        ram = cpu.getram()

        dest = self.a
        arg1 = ram.read(self.b)

        if self.immediate:
            arg2 = self.c
        else:
            arg2 = ram.read(self.c)

        result = arg1 & arg2

        ram.write(dest, result)


class SHIFT(OPCODE):
    """
    Shift/rotate by immediate
    """

    def __init__(self, args):
        OPCODE.__init__(self, args)

        if self.c // 8 == 0:
             self.mnemonic = "shl"

        elif self.c // 8 == 1:
             self.mnemonic = "shr"

        elif self.c // 8 == 2:
             self.mnemonic = "sar"

        elif self.c // 8 == 3:
             self.mnemonic = "rol"

        self.bits = self.c % 8

    def print_args(self):
        dest = 'r' + str(self.a)
        arg1 = 'r' + str(self.b)
        shift = str(self.bits)

        return "{}, {}, {}".format(dest, arg1, shift)

    def _exe(self, cpu):
        ram = cpu.getram()

        dest = self.a
        arg1 = ram.read(self.b)
        shift = self.bits

        if self.mnemonic == "shl":
            result = (arg1 << shift) & 63

            ram.write(dest, result)

        elif self.mnemonic == "shr":
            result = arg1 >> shift

            ram.write(dest, result)

        elif  self.mnemonic == "sar":
            # ~ #???
            # ~ arg1 = getsign(arg1)
            # ~ result = abs(arg1 >> shift)

            # ~ ram.write(dest, result)
            raise Exception("sar not implemented")

        elif  self.mnemonic == "rol":
            # not in program and im lazy
            raise Exception("rol not implemented")


### opcodes 013 and 014, shift by register value are unused in program
### shl and shr only executed by immediate value in op 012, SHIFT
# ~ class SHL(OPCODE):
    # ~ pass

# ~ class SHR(OPCODE):
    # ~ pass


class LD(OPCODE):
    def __init__(self, args):
        OPCODE.__init__(self, args)

        self.mnemonic = "ld"

    def print_args(self):
        dst = 'r' + str(self.a)
        src = 'r' + str(self.b)
        offset = str(self.c)

        return " {}, [{}+{}] ".format(dst, src, offset)

    def _exe(self, cpu):
        ram = cpu.getram()

        dest = self.a
        src = (self.b + self.c) % 64
        value = ram.read(src)

        ram.write(dest, value)


class ST(OPCODE):
    def __init__(self, args):
        OPCODE.__init__(self, args)
        self.mnemonic = "st"

    def print_args(self):
        value = 'r' + str(self.a)
        dst = 'r' + str(self.b)
        offset = str(self.c)

        return " [{}+{}], {}".format(dst, offset, value)

    def _exe(self, cpu):
        ram = cpu.getram()

        dest = (self.b + self.c) % 64
        value = ram.read(self.a)

        ram.write(dest, value)


class LBL(OPCODE):
    def __init__(self, args):
        OPCODE.__init__(self, args)
        self.mnemonic = "lbl"
        self.label = 64 * self.a + self.b

    def print_args(self):
        label = "{0:#0{1}x}".format(self.label, 6)

        if self.c == 0:
            return label

        else:
            return "{}, {} ; ret target".format(label, hex(self.c))

    def getlabel(self):
        return (self.label, self.c)

    def _exe(self, cpu):
        pass


class JUP(OPCODE):
    def __init__(self, args):
        OPCODE.__init__(self, args)
        self.mnemonic = "jup"
        self.label = 64 * self.a + self.b

    def print_args(self):
        label = "{0:#0{1}x}".format(self.label, 6)

        if self.c == 0:
            return label

        else:
            register = 'r' + str(self.c)
            return "{}, {} ; ret".format(label, register)

    def _exe(self, cpu):
        ram = cpu.getram()
        program = cpu.getprogram()

        search = ( self.label, ram.read(self.c) )

        start = cpu.getpc()

        cpu.stepback()

        while cpu.getpc() != start:
            instr = program.getinstruction( cpu.getpc() )

            if instr.getlabel() == search:
                if instr.getcondition() == None or instr.getcondition() == cpu.getcondition():
                    cpu.stepback()
                    break

            cpu.stepback()

        else:
            raise Exception("Looped without finding label")


class JDN(OPCODE):
    def __init__(self, args):
        OPCODE.__init__(self, args)
        self.mnemonic = "jdn"
        self.label = 64 * self.a + self.b

    def print_args(self):
        label = "{0:#0{1}x}".format(self.label, 6)

        if self.c == 0:
            return label

        else:
            register = 'r' + str(self.c)
            return "{}, {} ; ret".format(label, register)

    def _exe(self, cpu):
        ram = cpu.getram()
        program = cpu.getprogram()

        search = ( self.label, ram.read(self.c) )

        start = cpu.getpc()

        cpu.step()

        while cpu.getpc() != start:
            instr = program.getinstruction( cpu.getpc() )

            if instr.getlabel() == search:
                if instr.getcondition() == None or instr.getcondition() == cpu.getcondition():
                    cpu.stepback()
                    break

            cpu.step()

        else:
            raise Exception("Looped without finding label")


class IO(OPCODE):
    def __init__(self, args):
        OPCODE.__init__(self, args)
        self.mnemonic = "io"

    def print_args(self):
        recv = 'r' + str(self.a)
        dev = str(self.b)
        send = 'r' + str(self.c)

        if self.a == 0 and self.c == 0:
            return " {}".format(dev)

        elif self.a == 0:
            return " {}, {} ; put {} to dev {}".format(dev, send, send, dev)

        elif self.c == 0:
            return " {}, {} ; read dev {} to {}".format(recv, dev, dev, recv)

        else:
            return " {}, {}, {} ; send {} to dev {}, recv to {}".format(recv, dev, send, send, dev, recv)

    def _exe(self, cpu):
        ram = cpu.getram()

        ## will return device method ptr
        dev = cpu.getdevice(self.b)

        if dev == None:
            print("Get dev {} returned none; exiting..".format(self.b))
            exit(1)

        dest = self.a

        data = ram.read(self.c)

        result = dev(data)

        ## if were writing to dev, dest will be r0 which can't be written to
        ram.write(dest, result)


class MUL(OPCODE):
    def __init__(self, args):
        OPCODE.__init__(self, args)

        if self.c // 16 == 0:
            self.mnemonic = "fmu"

        elif self.c // 16 == 1:
            self.mnemonic = "fms"

        self.pr = self.c % 16

    def print_args(self):
        rdst = 'r' + str(self.a)
        b = 'r' + str(self.b)
        pr = str(self.pr)

        return "/{} {}, {}".format(pr, rdst, b)

    def _exe(self, cpu):
        ram = cpu.getram()

        dest = self.a
        arg1 = ram.read(self.a)
        arg2 = ram.read(self.b)

        if self.mnemonic == "fms":
            arg1 = getsigned(arg1)
            arg2 = getsigned(arg2)

        result = (arg1 * arg2) >> self.pr

        result = result & 63

        result = getunsigned(result)

        ram.write(dest, result)


class HALT(OPCODE):
    def __init__(self):
        self.mnemonic = "halt"
        self.cond = None

    def print_args(self):
        return "; invalid instruction"

    def _exe(self, cpu):
        print("Reached halt instruction")
        exit()


def getsigned(x):
    if x & 32:
        return (x & 31) * -1

    else:
        return x

def getunsigned(x):
    if x < 0:
        return 32 + (x & 31)
    else:
        return x
