from struct import *
import socket

HOST = '127.0.0.1'    # The remote host
PORT = 9112              # The same port as used by the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))


key = "qicheren11"
value = "qcherenaa"
#fmstr = "i%ds%ds" % (len(key), len(value))
fmstr = "i256s256s"
content = pack(fmstr, 0, key, value)
s.sendall(content)


"""
key = "1559915599jiajiajieshishi15599"
value = "22world"
fmstr = "i256s256s"
content = pack(fmstr, 1, key, value)
s.sendall(content)
"""

"""
for i in range(16000, 13000, -1):
    print i
    key = str(i) + str(i) + "jiajiajieshishi" + str(i)
    value = "YAGAOCHANG1" + str(i)
    fmstr = "i256s256s"
    content = pack(fmstr, 0, key, value)
    s.sendall(content)
    key = str(i) + str(i) + "gaogaolujiejie" + str(i)
    value = "YAGAOCHANG2" + str(i)
    fmstr = "i256s256s"
    content = pack(fmstr, 0, key, value)
    s.sendall(content)

fmstr = "i256s256s"
content = pack(fmstr, 2, key, value)
s.sendall(content)

fmstr = "i256s256s"
content = pack(fmstr, 3, key, value)
s.sendall(content)
"""

"""
key = "jiajieshi"
value = "17"
fmstr = "i256s256s"
content = pack(fmstr, 4, key, value)
s.sendall(content)

key = "1559915599jiajiajieshishi15599"
value = "22world"
fmstr = "i256s256s"
content = pack(fmstr, 1, key, value)
s.sendall(content)
"""

"""
key = "1"
value = "2"
fmstr = "i256s256s"
content = pack(fmstr, 5, key, value)
s.sendall(content)

key = "1559915599jiajiajieshishi15599"
value = "22world"
fmstr = "i256s256s"
content = pack(fmstr, 1, key, value)
s.sendall(content)
"""

key = "qicheren12"
value = "qcherenaa"
#fmstr = "i%ds%ds" % (len(key), len(value))
fmstr = "i256s256s"
content = pack(fmstr, 6, key, value)
s.sendall(content)

s.close()

