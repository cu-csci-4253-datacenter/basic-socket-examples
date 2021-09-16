#!/usr/bin/env python3
##
## Corresponds to tcp-2-client.c
##
import socket
import sys,os


##
## Sample TCP client - sends msg, waits for response
## Pair with tcp-2-server.py
##

if len(sys.argv) < 3:
    print("usage:", sys.argv[0], "<network> <port>")
    sys.exit(1)

network = sys.argv[1]
port = sys.argv[2]
me = "client"


def print_socket_endpoints(name, sockfd):
    print("%s: Socket %s is connecting %s(me) and %s(them)" % (
        me, name, sockfd.getsockname(), sockfd.getpeername() ))

##
## Create a socket, bind to any interface, specific port
##
sockfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sendAddress = (network, int(port))
sockfd.connect(sendAddress)

print_socket_endpoints("socket", sockfd)

line = sys.stdin.readline()
while True:
    line = line.rstrip()
    if line == '.':
        break
    sockfd.send(line.encode('utf-8'))
    line = sockfd.recv(8192)
    print("%s: got %s" % ( me, line.decode('utf-8') ))
    line = sys.stdin.readline()
print("%s: User signals end of input, exit" % (me))
