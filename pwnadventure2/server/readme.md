### get started with:

```
podman build -f containerfile -t pwn2
podman run -d --init --name pwn2 -p "4200-4201:4200-4201" --mount type=bind,src=./server.ini,dst=/opt/pwnadventure2_server/GameServer/server.ini,ro=true pwn2
```

### info
I removed code from `start_servers.py` that would have attempted to determine the server's external IP address.
You'll need to set it in `server.ini`
The password in `server.ini` is the default for the master server DB. You could change it with `mono MasterServer.exe --create-server-account`