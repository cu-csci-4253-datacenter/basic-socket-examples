#!/usr/bin/python
##
## Corresponds to udp-3-client.c
##
import socket
import sys,os


##
## Sample UDP client - sends messages, receives
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

line = sys.stdin.readline()
while line != '':
    line = line.rstrip()
    sockfd.sendto(line, sendAddress)
    line, addr = sockfd.recvfrom(8192)    
    print("Client received from %s (port %d): %s" % (addr[0], addr[1], line))
    line = sys.stdin.readline()
print("Done")
