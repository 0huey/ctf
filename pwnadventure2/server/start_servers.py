#!/usr/bin/env python
import subprocess
import time
import os
import shutil
import multiprocessing
import sys

# Default options
server_count = multiprocessing.cpu_count()
master_host = "binaryninja.com"
master_port = 4200
server_port_start = None
username = None
password = None
host = None
disable_spectator = False

# Get the working directory for the server
workdir = os.path.dirname(os.path.realpath(__file__))

# Parse settings file if it exists
if os.path.exists(os.path.join(workdir, "server.ini")):
        for line in open(os.path.join(workdir, "server.ini"), "r"):
                if line.find('=') == -1:
                        continue
                setting = line[0:line.find('=')].strip()
                value = line[line.find('=') + 1:].strip()
                if setting == "server_count":
                        server_count = int(value)
                elif setting == "master_host":
                        master_host = value
                elif setting == "master_port":
                        master_port = int(value)
                elif setting == "username":
                        username = value
                elif setting == "password":
                        password = value
                elif setting == "ip":
                        host = value
                elif setting == "port":
                        server_port_start = int(value)
                elif setting == "spectator":
                        if not bool(value):
                                disable_spectator = True

if (username is None) or (password is None):
        print "Username or password for server account not specified"
        print "Create the file 'server.ini' with the settings 'username' and 'password'"
        print "The server account can be created by running 'MasterServer.exe --create-server-account'"
        sys.exit(1)

print "Starting servers"

# Detect IP address of server
if host is None:
        print "Unable to autodetect IP address"
        print "Please provide the correct IP address with the setting 'ip' in 'server.ini'"
        sys.exit(1)

def start_server(server_index):
        params = [os.path.join(workdir, "pwnadventure2_gameserver"), "--master", master_host, str(master_port), "--account", username, password, "--host", host]
        if server_port_start is not None:
                params.append("--port")
                params.append(str(server_port_start + server_index))
        if disable_spectator:
                params.append("--disable-spectator")
        return subprocess.Popen(params, cwd=workdir)

processes = []
for i in xrange(0, server_count):
        processes.append([i, start_server(i)])
        print "Server %d started" % (i + 1)

try:
        print "Servers active"
        while True:
                time.sleep(10)

                # Check for stopped servers
                stopped = []
                for process in processes:
                        if process[1].poll() is not None:
                                print "Server %d has stopped" % (process[0] + 1)
                                stopped.append(process)

                # Restart any crashed servers
                for process in stopped:
                        processes.remove(process)
                        processes.append([process[0], start_server(process[0])])
                        print "Server %d restarted" % (process[0] + 1)
except:
        # On interruption, stop all servers
        print "Stopping servers"
        for process in processes:
                process[1].terminate()
                process[1].wait()

print "Servers terminated"
