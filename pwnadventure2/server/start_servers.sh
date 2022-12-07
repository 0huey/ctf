#!/bin/bash

cd /opt/pwnadventure2_server/MasterServer
mono MasterServer.exe &

cd /opt/pwnadventure2_server/GameServer
python start_servers.py &

wait -n

exit $?
