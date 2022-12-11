from struct import pack, unpack
from binascii import hexlify, unhexlify

ORIGIN_SERVER = "server"
ORIGIN_CLIENT = "client"
EMPTY_TUPLE = (None, None)

UBYTE = "B"
U16 = "H"
U32 = "I"
FLOAT = "f"

MSG_MOVE = 'MOVE'
MSG_JUMP = 'JUMP'
MSG_ATTACK = 'FIRE'
MSG_CHAT = 'CHAT'
MSG_PLAYER_HEALTH = 'LIFE'
MSG_READY_WEP = "REDY"
MSG_CHAT = "CHAT"

MSG_LOOT_DROP = "DEAD"
MSG_GET_LOOT = "LOOT"
MSG_RECEIVE_ITEM = "ITEM"
MSG_REMOVE_ITEM = "DELE"
MSG_QUERY_LOOT = "UHAZ"

MSG_MOB_POSITION = 'OPOS'
MSG_ANIMATION = 'ANIM'
MSG_MOB_SPAWN = 'SPWN'
MSG_MOB_DAMAGE = 'ODMG'

MSG_LOAD = "LOAD"
MSG_NOOP = ''

last_position = (0.0, 0.0, 0.0)

def Parse(buff: bytes, origin: str) -> tuple:
    MSG_IGNORE = [MSG_JUMP, MSG_PLAYER_HEALTH, MSG_ANIMATION, MSG_QUERY_LOOT, MSG_NOOP, MSG_LOAD]
    MSG_PARSE = {
        MSG_MOVE: Move,
        MSG_MOB_POSITION: MobPosition,
        MSG_PLAYER_HEALTH: Health,
        MSG_ATTACK: Attack,
        MSG_MOB_SPAWN: MobSpawn,
        MSG_LOOT_DROP: LootDrop,
        MSG_RECEIVE_ITEM: FoundItem,
        MSG_REMOVE_ITEM: LostItem,
        MSG_LOAD: OnGameLoad
    }

    if origin == ORIGIN_SERVER:
        #server packet (usually) begins with the total length of all messages
        #client always sends 1 message at a time
        buff = buff[2:]

    for cmd, data in UnpackMessages(buff):

        if cmd in MSG_IGNORE:
            pass

        elif cmd in MSG_PARSE:
            return MSG_PARSE[cmd](data, origin)

        else:
            print(f"[{origin}] [{cmd}] ({len(data)}) {hexlify(data)}")

    return EMPTY_TUPLE


def Move(data: bytes, origin: str) -> tuple:
    x, y, z = unpack(FLOAT*3, data[:12])
    xA, yA, zA = unpack(UBYTE*3, data[12:15])
    xA = round(xA * 360 / 256)
    yA = round(yA * 360 / 256)
    zA = round(zA * 360 / 256)

    #print(f"[{MSG_MOVE}] pos: {x:.2f} {y:.2f} {z:.2f} cam: {xA} {yA} {zA}")

    global last_position
    last_position = (x, y, z)

    return EMPTY_TUPLE

def Health(data: bytes, origin: str) -> tuple:
    health, = unpack(U16, data[:2])
    print(f"[{MSG_PLAYER_HEALTH}] {health}")

    return EMPTY_TUPLE

def Attack(data: bytes, origin: str) -> tuple:
    x, z, y = unpack(FLOAT*3, data[:12])
    data=data[12:]

    weapon, = unpack(U32, data[:4])
    data=data[4:]

    #xA, zA, yA = unpack("fff", data[:12])
    #data=data[12:]

    print(f"[{MSG_ATTACK}] pos: {x:.2f} {y:.2f} {z:.2f} wep?: {weapon} ({len(data)}) {hexlify(data)}")

    return EMPTY_TUPLE

def LootDrop(data: bytes, origin: str) -> tuple:
    id, unknown = unpack(U32*2, data)
    #auto loot
    msg = PackMessage(MSG_GET_LOOT, pack(U32, id))
    return (None, msg)

def FoundItem(data: bytes, origin: str) -> tuple:
    name_len, = unpack(UBYTE, data[:1])
    data=data[1:]

    name = data[:name_len].decode('ascii')
    data = data[name_len:]

    amount, = unpack(U32, data)

    print(f"Found {amount} {name}")

    return EMPTY_TUPLE

def LostItem(data: bytes, origin: str) -> tuple:
    name_len, = unpack(UBYTE, data[:1])
    data=data[1:]

    name = data[:name_len].decode('ascii')
    data = data[name_len:]

    amount, = unpack(U32, data)

    print(f"Lost {amount} {name}")

    return EMPTY_TUPLE

def MobSpawn(data: bytes, origin: str) -> tuple:
    id, = unpack(U32, data[:4])
    data=data[4:]

    name_len, = unpack(UBYTE, data[:1])
    data=data[1:]

    name = data[:name_len].decode('ascii')
    data = data[name_len:]

    print(f"[{MSG_MOB_SPAWN}] ID:{id} {name} {data}")

    return EMPTY_TUPLE

def MobPosition(data: bytes, origin: str) -> tuple:
    id, = unpack(U32, data[:4])
    data=data[4:]

    x, y, z = unpack(FLOAT*3, data[:12])
    data=data[12:]

    # if id == 1:
    #     print(f"[MOB] ID {id} pos: {x:.2f} {y:.2f} {z:.2f} ({len(data)}) {hexlify(data)}")

    # move = pack(FLOAT*3 + UBYTE*3, x, y, z, 0, 0, 0)
    # move = PackMessage(MSG_MOVE, move)

    # ready = PackMessage(MSG_READY_WEP, b'')
    #                                         #static rotation and other unknown data
    # attack = pack(FLOAT*3 + U32, x, y, z, 1) + unhexlify('803be4bca55f79beb930783fffffffff')
    # attack = PackMessage(MSG_ATTACK, attack)

    # x, y, z = last_position
    # move_back = pack(FLOAT*3 + UBYTE*3, x, y, z, 0, 0, 0)
    # move_back = PackMessage(MSG_MOVE, move_back)

    # msg = move + ready + attack + move_back
    # return (None, msg)

    return EMPTY_TUPLE

def OnGameLoad(data: bytes, origin: str) -> tuple:
    # packet = bytes()

    # for item in ['Gold', 'PistolAmmo']:
    #     msg = pack(UBYTE, len(item)) + item.encode('ascii') + pack(U32, 99999)
    #     packet += PackMessage(MSG_RECEIVE_ITEM, msg)

    # return (packet, None)
    return EMPTY_TUPLE


def Chat(data: bytes, origin: str) -> tuple:
    if origin == ORIGIN_CLIENT:
        msg_len = unpack(UBYTE, data[:1])
        data=data[1:]

        msg = data[:msg_len].decode('latin-1')
        msg = msg.split()

        # implement map change
        # if len(msg) == 2:
        #     if msg[0] == "map" and msg[1] in map_list:

    return EMPTY_TUPLE



def UnpackMessages(buff: bytes) -> bytes:
    while len(buff) >= 2:
        # length(2):type(4):data
        msg_len, = unpack(U16, buff[0:2])
        msg = ( buff[2:6].decode('latin-1'), buff[6:msg_len + 2] )
        buff = buff[msg_len + 2:]
        yield msg

def PackMessage(cmd: str, data: bytes) -> bytes:
    msg = cmd.encode('latin-1') + data
    return pack(U16, len(msg)) + msg

def PackServerUpdate(packet: bytes) -> bytes:
    return pack(U16, len(packet)) + packet
