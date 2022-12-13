#!/usr/bin/env python3

import select
import socket
import sys
import importlib
import traceback

import gamemessages

def Main(proxy_server_addr, remote_server_addr):
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(proxy_server_addr)
    server.listen()

    all_sockets = [server]

    proxied_socket_map = dict()

    while True:
        readable, _, _ = select.select(all_sockets, [], [])

        for conn in readable:
            if conn is server:
                client, addr = server.accept()

                print(f"New connection from {addr}")

                forward = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                forward.connect(remote_server_addr)

                proxied_socket_map[client] = forward

                # make them "selectable"
                all_sockets.append(client)
                all_sockets.append(forward)

            else:
                if conn in proxied_socket_map:
                    matched_conn = proxied_socket_map[conn]
                else:
                    matched_conn = GetClientOfForwardSocket(conn, proxied_socket_map)

                try:
                    data = conn.recv(4096)
                except ConnectionResetError as e:
                    print(f"error reading from {conn.getpeername()}")
                    CleanupSockets(conn, matched_conn, proxied_socket_map, all_sockets)
                    continue

                try:
                    importlib.reload(gamemessages)

                    if conn.getpeername() == remote_server_addr:
                        origin = "server"
                    else:
                        origin = "client"

                    new_data = gamemessages.Handle(data, origin)

                except Exception as e:
                    print(traceback.format_exc())

                    matched_conn.sendall(data)

                else:
                    matched_conn.sendall(new_data)


def GetClientOfForwardSocket(forward, proxy_socks):
    for client in proxy_socks:
        if proxy_socks[client] is forward:
            return client

def CleanupSockets(s1, s2, proxied_socket_map, all_sockets):
    all_sockets.remove(s1)
    all_sockets.remove(s2)

    if s1 in proxied_socket_map:
        del proxied_socket_map[s1]
    elif s2 in proxied_socket_map:
        del proxied_socket_map[s2]

if __name__ == "__main__":
    local  = ("192.168.56.1", 4201)
    remote = ("127.0.0.1", 4201)

    Main(local, remote)
