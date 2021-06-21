#!/usr/bin/env python3

# ~ _ZN7salsa204core13Core$LT$R$GT$3new17h06163fbcdf79ba51E (
   # ~ $rdi = 0x00007ffe4b57cc70 → 0x0000000000000000,
   # ~ $rsi = 0x00007ffe4b57cd00 → "ef39f4f20e76e33bd25f4db338e81b10", ; key
   # ~ $rdx = 0x00007ffe4b57cd30 → 0x3361303732633464 ; iv
# ~ )

# ~ 53 @ 0000660e  int128_t* alloc_0x20 = rax_4
# ~ 54 @ 00006611  int128_t zmm0 = [data_39cc0].o
# ~ 55 @ 00006618  [rax_4].o = zmm0
# ~ 56 @ 0000661b  int128_t zmm0 = [data_39cd0].o
# ~ 57 @ 00006622  [rax_4 + 0x10].o = zmm0

from Crypto.Cipher import Salsa20  # pycryptodome
from pwn import p64

key = b"ef39f4f20e76e33bd25f4db338e81b10"
iv  = p64(0x3361303732633464)

crypted_pw = b"\x05\x05_\xb1\xa3)\xa8\xd5X\xd9\xf5V\xa6\xcb1\xf3$C*1\xc9\x9d\xecr\xe3>\xb6ob\xad\x1b\xf9"

cipher = Salsa20.new(key=key, nonce=iv)

print(cipher.decrypt(crypted_pw))
