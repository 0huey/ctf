import charset

from pwn import *

import time
import math

class Serial:
    def __init__(self, socket):

        self.buff = Buffer()

        if socket == None:
            self.socket = None

        else:
            host, port = socket
            self.socket = remote(host, port)

    def incoming(self, data):
        self.checksocket()

        length = len(self.buff)

        if length > 63:
            length = 63

        return length


    def read(self, data):
        self.checksocket()

        if len(self.buff) == 0:
            return 63

        else:
            char = self.buff.get(1).decode()
            return charset.FromAscii(char)


    def write(self, data):
        if self.socket:
            self.socket.write( charset.ToAscii(data) )

        else:
            print(charset.ToAscii(data), end='')


    def checksocket(self):
        if self.socket:
            data = self.socket.read(timeout=0.05)
            self.buff.add(data)


class Clock:
    def __init__(self):
        self.ref_time = time.monotonic()

    def low(self, data):
        if data != 0:
            self.ref_time = time.monotonic()

        time_diff = time.monotonic() - self.ref_time

        ## convert % of second to 6 bit int
        perc_sec = time_diff - math.floor(time_diff)

        return math.floor( perc_sec * 64 )

    def high(self, data):
        if data != 0:
            self.ref_time = time.monotonic()

        time_diff = math.floor(time.monotonic() - self.ref_time)

        ## max 6 bits; never overflows
        if time_diff > 63:
            return 63

        else:
            return time_diff



