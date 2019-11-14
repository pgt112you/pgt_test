#coding:utf-8
from struct import *
import socket
import time


HOST = '127.0.0.1'    # The remote host
PORT = 9122              # The same port as used by the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))


# open db
key = "qicheren11"
value = "qcherenaa"
fmstr = "i256s256s"
content = pack(fmstr, 101, key, value)
s.sendall(content)
print s.recv(1000)

s.close()

