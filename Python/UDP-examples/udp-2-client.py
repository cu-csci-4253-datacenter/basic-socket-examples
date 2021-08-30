#!/usr/bin/env python3
##
## Corresponds to udp-2-client.c
##
import socket
import sys,os


##
## Sample UDP client - simply sends messages, doesn't receive.
## Pair with udp-2-server.c
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
while len(line) != 0:
    sockfd.sendto(line.encode('utf-8'), sendAddress)
    line = sys.stdin.readline()
print("Done")
