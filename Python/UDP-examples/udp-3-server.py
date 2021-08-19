#!/usr/bin/python
##
## Corresponds to udp-3-server.c
##
import socket
import sys,os


##
## Sample UDP server - receive message, return it to client
## Pair with udp-3-server.c
##

if len(sys.argv) < 3:
    print("usage:", sys.argv[0], "<network> <port>")
    sys.exit(1)

network = sys.argv[1]
port = sys.argv[2]

##
## Create a socket, bind to any interface, specific port
##
sockfd = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sendAddress = (network, int(port))
sockfd.bind(sendAddress)

while True:
    line, clientAddress = sockfd.recvfrom(8192)
    print("Server received from %s (port %d): %s" % (clientAddress[0], clientAddress[1], line))
    sockfd.sendto(line, clientAddress)
print("Done")
