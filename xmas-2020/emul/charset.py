def ToAscii(data):
    return charset[data]

def FromAscii(data):
    for index, char in enumerate(charset):
        if data.upper() == char:
            return index

    else:
        raise("invalid char:", data)

charset = [
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'G',
    'H',
    'I',
    'J',
    'K',
    'L',
    'M',
    'N',
    'O',
    'P',
    'Q',
    'R',
    'S',
    'T',
    'U',
    'V',
    'W',
    'X',
    'Y',
    'Z',
    ' ',
    '+',
    '-',
    '*',
    '/',
    '<',
    '=',
    '>',
    '(',
    ')',
    '[',
    ']',
    '{',
    '}',
    '#',
    '$',
    '_',
    '?',
    '|',
    '^',
    '&',
    '!',
    '~',
    ',',
    '.',
    ':',
    '\n',
    None
]
