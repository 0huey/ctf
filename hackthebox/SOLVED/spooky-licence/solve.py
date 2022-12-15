import z3

buff = list()

for x in range(32):
    buff.append(z3.BitVec(f"b{x}", 8))

s = z3.Solver()

s.add(buff[0x1d] == buff[5] - buff[3] + ord('F'))
s.add(buff[2] + buff[0x16] == buff[0xd] + ord('{'))
s.add(buff[0xc] + buff[4] == buff[5] + 0x1c)
s.add(buff[0x19] * buff[0x17] == buff[0] + buff[0x11] + 0x17)
s.add(buff[0x1b] * buff[1] == buff[5] + buff[0x16] + -0x15)
s.add(buff[9] * buff[0xd] == buff[0x1c] * buff[3] + -9)
s.add(buff[9] == ord('p'))
s.add(buff[0x13] + buff[0x15] == buff[6] + -0x80)
s.add(buff[0x10] == buff[0xf] - buff[0xb] + ord('0'))
s.add(buff[7] * buff[0x1b] == buff[1] * buff[0xd] + ord('-'))
s.add(buff[0xd] == buff[0x12] + buff[0xd] + -0x65)
s.add(buff[0x14] - buff[8] == buff[9] + ord('|'))
s.add(buff[0x1f] == buff[8] - buff[0x1f] + -0x79)
s.add(buff[0x14] * buff[0x1f] == buff[0x14] + 0x04)
s.add(buff[0x18] - buff[0x11] == buff[0x15] + buff[8] + -0x17)
s.add(buff[7] + buff[5] == buff[5] + buff[0x1d] + ord(','))
s.add(buff[0xc] * buff[10] == buff[1] - buff[0xb] + -0x24)
s.add(buff[0x1f] * buff[0] == buff[0x1a] + -0x1b)
s.add(buff[1] + buff[0x14] == buff[10] + -0x7d)
s.add(buff[0x12] == buff[0x1b] + buff[0xe] + 0x02)
s.add(buff[0x1e] * buff[0xb] == buff[0x15] + ord('D'))
s.add(buff[5] * buff[0x13] == buff[1] + -0x2c)
s.add(buff[0xd] - buff[0x1a] == buff[0x15] + -0x7f)
s.add(buff[0x17] == buff[0x1d] - buff[0] + ord('X'))
s.add(buff[0x13] == buff[8] * buff[0xd] + -0x17)
s.add(buff[6] + buff[0x16] == buff[3] + ord('S'))
s.add(buff[0xc] == buff[0x1a] + buff[7] + -0x72)
s.add(buff[0x10] == buff[0x12] - buff[5] + ord('3'))
s.add(buff[0x1e] - buff[8] == buff[0x1d] + -0x4d)
s.add(buff[0x14] - buff[0xb] == buff[3] + -0x4c)
s.add(buff[0x10] - buff[7] == buff[0x11] + ord('f'))
s.add(buff[1] + buff[0x15] == buff[0xb] + buff[0x12] + ord('+'))

s.check()
solution = s.model().__str__().strip('[]').replace(',','').splitlines()

output = ['']*32

for x in solution:
    index, num = x.strip(' b').split(' = ')
    output[int(index)] = chr(int(num))

print(''.join(output))
