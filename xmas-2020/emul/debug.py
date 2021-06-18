class Debug:
    def __init__(self):
        self.last_cmd = None
        self.breakpoints = set()

    def debug(self, cpu):
        if self.last_cmd == 'c' and not cpu.getpc() in self.breakpoints:
            return

        self.print_info(cpu)

        while True:
            i = input("dbg> ").strip()

            if i == "":
                i = self.last_cmd

            if i.startswith("ni") or i.startswith("c"):
                self.last_cmd = i
                break

            elif i.startswith('br'):
                i = i.split()

                if len(i) > 1 and i[1].isdecimal():
                    self.breakpoints.add( int(i[1]) )

            elif i.startswith('info br'):
                print(self.breakpoints)

    def print_info(self, cpu):
        pc =  cpu.getpc()
        print("\nCPU - pc:", pc, "\tcondition:", cpu.getcondition())

        print(cpu.getram())

        prog = cpu.getprogram()

        for x in list(range(10)):
            if pc+x < prog.getlen():
                print(pc + x, "\t", prog.getinstruction(pc + x))
