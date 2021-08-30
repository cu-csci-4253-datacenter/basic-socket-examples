#!/usr/bin/env python3
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
    line, addr = sockfd.recvfrom(8192)
    print(f"Server received from {addr[0]} port {addr[1]}: {line.decode('utf-8').rstrip()}")
    sockfd.sendto(line, addr)
print("Done")
