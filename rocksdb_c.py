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

## write into db
#key = "changanqiche"
#value = "000625"
#fmstr = "i256s256s"
#content = pack(fmstr, 0, key, value)
#s.sendall(content)
#
#key = "changchengqiche"
#value = "601633"
#fmstr = "i256s256s"
#content = pack(fmstr, 0, key, value)
#s.sendall(content)

# seek and get
#key = "changb"
#value = "601633"
#fmstr = "i256s256s"
#content = pack(fmstr, 6, key, value)
#s.sendall(content)

#key = "changd"    # 这个会报错，因为changchengqiche是最后一个key了，changd的话，超出rowkey的范围了，rocksdb会seek到一个不存在的位置
#value = "601633"
#fmstr = "i256s256s"
#content = pack(fmstr, 6, key, value)
#s.sendall(content)



## get score
#key = "jiajieshi"
#value = "gaolujie"
#fmstr = "i256s256s"
#content = pack(fmstr, 15, key, value)
#s.sendall(content)



#key = "changanqiche"
#value = "600069a"
#fmstr = "i256s256s"
#content = pack(fmstr, 1, key, value)
#s.sendall(content)
#
#key = "changanqi"
#value = "60006"
#fmstr = "i256s256s"
#content = pack(fmstr, 6, key, value)
#s.sendall(content)
#
key = "changanqi"
value = "60006"
fmstr = "i256s256s"
content = pack(fmstr, 13, key, value)
s.sendall(content)

#key = "changanqi"
#value = "60006"
#fmstr = "i256s256s"
#content = pack(fmstr, 14, key, value)
#s.sendall(content)

#key = "qicheren12"
#value = "qcherenaab"
#fmstr = "i256s256s"
#content = pack(fmstr, 0, key, value)
#s.sendall(content)



#key = "qicheren12"
#value = "qcherenaa"
#fmstr = "i256s256s"
#content = pack(fmstr, 0, key, value)
#s.sendall(content)
#
#
#key = "qicheren1"
#value = "qcherenaa"
#fmstr = "i256s256s"
#content = pack(fmstr, 10, key, value)
#s.sendall(content)
#
#key = "qicheren1a11aiaia1aa1"
#value = "qcherenaa"
#fmstr = "i256s256s"
#content = pack(fmstr, 10, key, value)
#s.sendall(content)
#
#key = "qicheren12"
#value = "qcherenaa"
#fmstr = "i256s256s"
#content = pack(fmstr, 1, key, value)
#s.sendall(content)
#
#key = "qicheren1"
#value = "qcherenaa"
#fmstr = "i256s256s"
#content = pack(fmstr, 1, key, value)
#s.sendall(content)

#for i in range(332121, 490562, 1):
#    print i
#    key = str(i) + "wuyongping" 
#    value = "YAGAOchang1" + str(i)
#    fmstr = "i256s256s"
#    content = pack(fmstr, 0, key, value)
#    s.sendall(content)
#    key = str(i) + "huweiwei" 
#    value = "YAGAOchang2" + str(i)
#    fmstr = "i256s256s"
#    content = pack(fmstr, 0, key, value)
#    s.sendall(content)
#

#key = ""
#value = ""
#fmstr = "i256s256s"
#content = pack(fmstr, 2, key, value)
#s.sendall(content)
#
#fmstr = "i256s256s"
#content = pack(fmstr, 3, key, value)
#s.sendall(content)
#


#key = "jiajieshi"
#value = "1"
#fmstr = "i256s256s"
#content = pack(fmstr, 4, key, value)
#s.sendall(content)

"""
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
"""


#key = "qicheren12"
#value = "qcherenaa"
##fmstr = "i%ds%ds" % (len(key), len(value))
#fmstr = "i256s256s"
#content = pack(fmstr, 6, key, value)
#s.sendall(content)



#key = "aaaq"
#value = "cftest11"
#fmstr = "i256s256s"
#content = pack(fmstr, 7, key, value)
#s.sendall(content)
#key = "aaaq"
#value = "cftest01"
#fmstr = "i256s256s"
#content = pack(fmstr, 7, key, value)
#s.sendall(content)
#key = "aaaq"
#value = "cftest10"
#fmstr = "i256s256s"
#content = pack(fmstr, 7, key, value)
#s.sendall(content)
#
#
#print "===" * 9
#time.sleep(5)
#
#
#
#key = "aaaq"
#value = "cftest21"
#fmstr = "i256s256s"
#content = pack(fmstr, 8, key, value)
#s.sendall(content)
#
#
#key = "aaaq"
#value = "ALL"
#fmstr = "i256s256s"
#content = pack(fmstr, 9, key, value)
#s.sendall(content)

s.close()

