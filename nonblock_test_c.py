import socket
import time

HOST = ''                 # Symbolic name meaning all available interfaces
PORT = 9112             # Arbitrary non-privileged port
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
time.sleep(15)
#s.sendall("recv data\n")
s.close()
