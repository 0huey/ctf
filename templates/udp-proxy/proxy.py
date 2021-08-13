#!/usr/bin/env python3

import select
import socket
import sys
import importlib
import traceback

import gamemessages

def Main(proxy_server_addr, remote_server_addr):
    server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server.bind(proxy_server_addr)

    all_sockets = [server]

    proxy_socks = dict()

    while True:
        try:
            importlib.reload(gamemessages)
        except Exception as e:
            print(traceback.format_exc())

        readable, _, _ = select.select(all_sockets, [], [])

        for sock in readable:
            try:
                data, origin = sock.recvfrom(4096) # data, (remhost,remport)

            #Probably wont even get a response unless the remote in on your LAN
            except ConnectionRefusedError:
                print(f"Connection refused from {sock.getpeername()}")

                all_sockets.remove(sock)
                del proxy_socks[GetClientAddrOfRemoteSock(sock, proxy_socks)]
                sock.close()

                continue


            # client -> proxy
            if sock is server:

                if origin in proxy_socks:
                    # get sock linked to client addr for proxy -> real server
                    forwarder = proxy_socks[origin]

                else:
                    # new client connection, create connection to real server
                    print("New connection from", origin)

                    forwarder = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

                    forwarder.connect(remote_server_addr)

                    proxy_socks[origin] = forwarder

                    # make it "selectable"
                    all_sockets.append(forwarder)

                try:
                    new_data = gamemessages.Handle(data, "client")

                except Exception as e:
                    print(traceback.format_exc())
                    forwarder.sendall(data)

                else:
                    forwarder.sendall(new_data)


            # real_server -> proxy
            else:
                client_addr = GetClientAddrOfRemoteSock(sock, proxy_socks)

                try:
                    new_data = gamemessages.Handle(data, "server")

                except Exception as e:
                    print(traceback.format_exc())
                    server.sendto(data, client_addr)

                else:
                    server.sendto(new_data, client_addr)


def GetClientAddrOfRemoteSock(sock, proxy_socks):
    for addr in proxy_socks:
        if proxy_socks[addr] is sock:
            return addr


if __name__ == "__main__":
    try:

        local_port = int(sys.argv[1])
        remote_host = sys.argv[2]

        if len(sys.argv) >= 4:
            remote_port = int(sys.argv[3])

        else:
            remote_port = local_port

    except:
        print("Usage: proxy.py local_port remote_host [remote_port]")
        exit(1)

    Main( ("0.0.0.0", local_port), (remote_host, remote_port) )
