#!/usr/bin/env python3

import select
import socket
import importlib
import traceback
from struct import pack, unpack


import parser

def Main(proxy_server_addr: str, remote_server_addr: str, num_servers: int, start_port: int) -> None:
    proxy_sockets = list()

    for x in range(num_servers):
        proxy = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        addr = (proxy_server_addr, start_port + x)
        proxy.bind((proxy_server_addr, start_port + x))
        proxy.listen(1)
        proxy_sockets.append(proxy)
        print(f"started server at {proxy.getsockname()}")

    all_sockets = proxy_sockets.copy()

    proxied_socket_map = dict()
    client_queue = dict()

    while True:
        readable, _, _ = select.select(all_sockets, [], [])

        for conn in readable:
            if conn in proxy_sockets:
                client, addr = conn.accept()

                print(f"New connection from {addr} on {conn.getsockname()}")

                _, port = conn.getsockname()

                forward = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                forward.connect((remote_server_addr, port))

                print(f"connected to {forward.getpeername()}")

                proxied_socket_map[client] = forward

                # make them selectable
                all_sockets.append(client)
                all_sockets.append(forward)

                client_queue[client] = bytes()

            else:
                if conn in proxied_socket_map:
                    matched_conn = proxied_socket_map[conn]
                else:
                    matched_conn = GetClientOfForwardSocket(conn, proxied_socket_map)

                try:
                    data = conn.recv(4096)
                except Exception as e:
                    print(f"error '{e}' reading from {conn.getpeername()}")
                    CleanupSockets(conn, matched_conn, proxied_socket_map, all_sockets)
                    continue

                # if conn is a key in the dict, it is a client
                if conn in proxied_socket_map:
                    origin = parser.ORIGIN_CLIENT
                    client = conn
                    server = matched_conn
                else:
                    origin = parser.ORIGIN_SERVER
                    client = matched_conn
                    server = conn

                to_client = None
                to_server = None

                try:
                    importlib.reload(parser)
                    to_client, to_server = parser.Parse(data, origin)

                except Exception as e:
                    print(traceback.format_exc())

                if origin == parser.ORIGIN_SERVER:
                    if to_client:
                        data += to_client

                    if len(client_queue[client]):
                        data += client_queue[client]

                        client_queue[client] = bytes()

                    data = parser.PackServerUpdate(data[2:])

                elif to_client:
                    client_queue[client] += to_client

                matched_conn.sendall(data)

                if to_server:
                    server.sendall(to_server)

def GetClientOfForwardSocket(forward, proxy_socks):
    for client in proxy_socks:
        if proxy_socks[client] is forward:
            return client

def CleanupSockets(s1, s2, proxied_socket_map, all_sockets):
    all_sockets.remove(s1)
    all_sockets.remove(s2)

    if s1 in proxied_socket_map:
        del proxied_socket_map[s1]
    if s2 in proxied_socket_map:
        del proxied_socket_map[s2]

    s1.close()
    s2.close()

if __name__ == "__main__":
    Main("192.168.56.1", "127.0.0.1", 5, 4201)
