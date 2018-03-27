#coding:utf-8
from struct import *
import socket
import time


HOST = '127.0.0.1'    # The remote host
PORT = 9112              # The same port as used by the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))


# open db
key = "qicheren11"
value = "qcherenaa"
fmstr = "i256s256s"
content = pack(fmstr, 101, key, value)
s.sendall(content)
time.sleep(2)

# write into db
key = "changanqiche"
value = "000625"
fmstr = "i256s256s"
content = pack(fmstr, 0, key, value)
s.sendall(content)

key = "changchengqiche"
value = "601633"
fmstr = "i256s256s"
content = pack(fmstr, 0, key, value)
s.sendall(content)

for i in range(0, 1600000, 1):
    print i
    key = str(i) + str(i) + "jiajiajieshishi" + str(i)
    value = "YAGAOchang1" + str(i)
    fmstr = "i256s256s"
    content = pack(fmstr, 0, key, value)
    s.sendall(content)
    key = str(i) + str(i) + "gaogaolujiejie" + str(i)
    value = "YAGAOchang2" + str(i)
    fmstr = "i256s256s"
    content = pack(fmstr, 0, key, value)
    s.sendall(content)




s.close()

