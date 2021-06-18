import dev
import ram
import debug

class EMU10:
    def __init__(self, prog, socket=None):
        self.pc = 0

        self.ram = ram.RAM()

        self.program = prog
        self.prog_len = self.program.getlen()

        self.debugger = debug.Debug()

        self.condition = False

        self.serial = dev.Serial(socket)
        self.clock = dev.Clock()

        self.devices = {
            0 : self.serial.incoming,
            1 : self.serial.read,
            2 : self.serial.write,
            3 : self.clock.low,
            4 : self.clock.high
        }

    def run(self, debug=False):
        while True:
            if debug:
                self.debugger.debug(self)

            instruction = self.program.getinstruction( self.pc )

            instruction.exe(self)

    def getpc(self):
        return self.pc

    def step(self):
        self.pc += 1

        if self.pc == self.prog_len:
            self.pc = 0

        return self.pc

    def stepback(self):
        self.pc -= 1

        if self.pc == -1:
            self.pc = self.prog_len - 1

        return self.pc

    def getcondition(self):
        return self.condition

    def setcondition(self, state):
        self.condition = state

    def getram(self):
        return self.ram

    def getprogram(self):
        return self.program

    def getdevice(self, dev):
        if not dev in self.devices:
            return None

        return self.devices[dev]
