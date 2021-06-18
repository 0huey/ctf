import struct



## Q 001a00000094b455020000070000005d1f35db090000000700414141414141  - say AAAAAA
## Q 001e00000094b47b020000070000005d1f35db0d0000000b0042424242424242424242   - say CCCCCCCCCC

## Q 001a00000094b4550200001a0000005511148509000000ff9f20c2b61ef9c2  - pos 1
## Q 001a00000094b4550200001a0000005511148509000000000046c2001cf9c2  - move short left
## Q 001a00000094b4550200001a00000055111485090000000100e0c0b61ef9c2  - move long right

## Q 001e00000094b47b02000017000000da96099c0d00000001000000000080bf00000000 - use slot 2
#serverupdate? 5a42f9020000120000001a0c000000730dd2c03ef28fc200004040

msg_subtypes = {
  "94b4" : "client update",
  "5a42" : "server update"
}

def Handle(data, origin):
    header = data[:20]
    body = data[20:]

    msg_type = chr(header[4]) # Q or R -- R seems to just be echo?

    if msg_type != "Q":
        return data

    msg_len = struct.unpack("I", body[:4])[0]

    msg = body[4:4+msg_len]

    # ~ extra_msg = body[4+msg_len:]

    if msg_type == "Q" and len(msg) > 2:

        msg_subtype = msg[:2].hex()
        if msg_subtype in msg_subtypes:
            msg_subtype = msg_subtypes[msg_subtype]

        if msg_subtype == "client update":
            return HandleClientUpdate(header, msg_len, msg)

        elif msg_subtype == "server update":
            HandleServerUpdate(header, msg)


client_updates = {
    "1a" : "pos",
    "17" : "use slot",
}

def HandleClientUpdate(header, msg_len, msg):
    update_type = bytes([msg[6]]).hex()

    if update_type in client_updates:
        update_type = client_updates[update_type]

    else:
        print("unknown client update", update_type, msg)
        return

    if update_type == "pos":
        return HandlePositionData(header, msg_len, msg)


def HandlePositionData(header, msg_len, msg):
    static_data = msg[:18]

    pos_data = msg[18:]

    x, y = struct.unpack("2f", pos_data)

    print("client pos", msg.hex())
    # ~ print("client pos", round(x), round(y), pos_data.hex()[:8], pos_data.hex()[8:])

    x = 70.0
    y = -62.0

    return
    pos_data = struct.pack("f", x) + struct.pack("f", y)

    new_msg = static_data + pos_data

    return ReassemblePacket(header, msg_len, new_msg)


server_updates = {
    "12" : "pos"
}

def HandleServerUpdate(header, msg):
    update_type = bytes([msg[6]]).hex()

    if update_type in server_updates:
        update_type = server_updates[update_type]

    else:
        # ~ print("unknown server update:", update_type, msg)
        return

    if update_type == "pos":
        print("server pos", msg.hex())

def ReassemblePacket(header, msg_len, msg):
    msg_len = struct.pack("I", len(msg))

    return header + msg_len + msg
