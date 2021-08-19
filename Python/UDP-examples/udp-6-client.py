#!/usr/bin/python
##
## Corresponds to udp-2-client.c
##
import socket
import sys,os


##
## Extend UDP-4 with bind, which sets a specific interface
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

sockfd.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

line = sys.stdin.readline()
while line != '':
    line = line.rstrip()
    sockfd.send(line)
    line = sockfd.recv(8192)
    addr = sockfd.getpeername()
    print("Client received from %s (port %d): %s" % (addr[0], addr[1], line))
    line = sys.stdin.readline()
print("Done")
