class RAM:
    def __init__(self):
        self.ram = [0] * 64

    def read(self, index):
        return self.ram[index]

    def write(self, index, value):
        ## 0 always eq 0 according to doc
        if index == 0:
            return

        self.ram[index] = value

        if not 0 <= value < 64:
            print("ram error?")

    def __str__(self):
        s = " "*6 + "0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n"
        s += "-" * len(s) + '\n'

        for row in list(range(4)):
            s += '{0:0{1}x}'.format(row*16, 2) + " | "

            start = row * 16

            for mem in self.ram[start : start+16]:
                s += '{0:0{1}x}'.format(mem, 2)  + " "

            s += '\n'

        return s
