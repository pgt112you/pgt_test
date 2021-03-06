import socket
import sys, os
import time

# Create a UDS socket

# Connect the socket to the port where the server is listening
sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
server_address = '/home/guangtong/pgt_test/test.sock'
try:
    sock.connect(server_address)
except socket.error, msg:
    print os.getpid()
    print sys.stderr, msg
    sys.exit(1)

try:
    # Send data
    message = 'This is the message by %ld . ' % os.getpid()
    message = message * 800 + "\n"
    sock.send(message)
    time.sleep(50)
except:
    pass

finally:
    print >>sys.stderr, 'closing socket'
    sock.close()
