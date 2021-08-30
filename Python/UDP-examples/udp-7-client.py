#!/usr/bin/python
##
## Corresponds to udp-7-client.c
##
import socket
import sys,os


##
## It is possible to "broadcast" to a network broadcast address.
## (e.g. 192.168.1.255) but doing so requires yet another socket
## option SO_BROADCAST to allow the use of broadcast addresses.
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
sockfd.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

line = sys.stdin.readline()
while len(line) > 0 :
    line = line.rstrip()
    sockfd.send(line.encode('utf-8'))
    line = sockfd.recv(8192)
    addr = sockfd.getpeername()
    print(f"Server received from {addr[0]} port {addr[1]}: {line.decode('utf-8').rstrip()}")
    line = sys.stdin.readline()
print("Done")
