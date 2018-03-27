from struct import *
import socket
import time


HOST = '127.0.0.1'    # The remote host
PORT = 9112              # The same port as used by the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

#keys = "abcdefghijklmnopqrstuvwxyz"
keys = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
i = 100000
fmstr = "i256s256s"
while (i < 250000):
    index = i % 25
    key = str(i) + keys[index]
    value = str(i)
    content = pack(fmstr, 0, key, value)
    print key
    s.sendall(content)
    i += 1
    time.sleep(0.0005)
    if i % 5000 == 0:
        content = pack(fmstr, 15, key, value)
        s.sendall(content)


s.close()

