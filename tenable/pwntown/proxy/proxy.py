#!/usr/bin/env python3

import select
import socket
import sys
import importlib

import gamemessages

def Main(proxy_addr, remote_addr):
    server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server.bind(proxy_addr)

    allsockets = [server]

    proxysocks     = dict()
    proxysocks_rev = dict()


    while True:
        try:
            importlib.reload(gamemessages)
        except Exception as e:
            print(e)

        readable, _, _ = select.select(allsockets, [], [])

        for sock in readable:
            data, origin = sock.recvfrom(4096) # data, (remhost,remport)

            # client -> proxy
            if sock == server:

                if origin in proxysocks:
                    # proxy -> real server
                    forwarder = proxysocks[origin]

                else:
                    # new client connection, create connection to real server
                    print("New connection from", origin)

                    forwarder = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

                    forwarder.connect(remote_addr)

                    proxysocks[origin]        = forwarder
                    proxysocks_rev[forwarder] = origin

                    allsockets.append(forwarder)  # make it "selectable"

                try:
                    new_data = gamemessages.Handle(data, "client")

                except Exception as e:
                    print(e)

                if new_data:
                    forwarder.sendall(new_data)

                else:
                    forwarder.sendall(data)


            # real_server -> proxy
            else:
                try:
                    new_data = gamemessages.Handle(data, "server")

                except Exception as e:
                    print(e)

                client_addr = proxysocks_rev[sock]

                if new_data:
                    server.sendto(new_data, client_addr)

                else:
                    server.sendto(data, client_addr)


if __name__ == "__main__":
    try:
        local_port = int(sys.argv[1])
        remote_host = sys.argv[2]
        remote_port = int(sys.argv[3])

    except:
        print("Usage: proxy.py local_port remote_host remote_port")
        exit(1)

    Main( ("0.0.0.0", local_port), (remote_host, remote_port) )
