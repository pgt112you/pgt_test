import socket
import time

HOST = ''                 # Symbolic name meaning all available interfaces
PORT = 50007              # Arbitrary non-privileged port
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)
conn, addr = s.accept()
print 'Connected by', addr
i = 0
while 1:
    i += 1
    data = conn.recv(1024)
    print "recv data:", data
    #if i > 1:
    #    time.sleep(20)
    conn.sendall("recv data\n")
    print "send data: recv data"
conn.close()
