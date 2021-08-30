#!/usr/bin/python
##
## Corresponds to udp-8-client.c
##
import socket
import sys,os


##
## An example that sets the send and receive buffer sizes for a given socket.
##


if len(sys.argv) < 5:
    print("usage:", sys.argv[0], "<network> <port> <mynetwork> <myport>")
    sys.exit(1)

network = sys.argv[1]
port = sys.argv[2]

myNetwork = sys.argv[3]
myPort = sys.argv[4]

##
## Create a socket, bind to any interface, specific port
##
sockfd = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sendAddress = (network, int(port))
myAddress = (myNetwork, int(myPort))
sockfd.bind(myAddress)       # Note: you need to BIND before you CONNECT
sockfd.connect(sendAddress)

bufsize = 32000

sockfd.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, bufsize)
sockfd.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, bufsize)

line = sys.stdin.readline()
while len(line) > 0:
    line = line.rstrip()
    sockfd.send(line.encode('utf-8'))
    line = sockfd.recv(8192)
    addr = sockfd.getpeername()
    print(f"Server received from {addr[0]} port {addr[1]}: {line.decode('utf-8').rstrip()}")
    line = sys.stdin.readline()
print("Done")
