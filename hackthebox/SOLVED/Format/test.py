#!/usr/bin/env python3

execve = 0x7f6a87ed1a03


ret_shorts = list()
and_x = 0xffff000000000000

for shift in list(range(1, 5)):
    short = (execve & and_x) >> ( 16 * ( 4 - shift) )

    ret_shorts.append( ( shift-1, short ) )

    and_x = and_x >> 16

print(hex(execve))

print(ret_shorts.sorted()

for x in ret_shorts:
    print(x[0], hex(x[1]))
