#!/usr/bin/python
##
## Corresponds to udp-2-client.c
##
import socket
import sys,os


##
## Sample UDP client that expects echoed messages. This example
## shows how you can use "connect" to connect the remote endpoint
## of a socket to a specific IP/port combination. The local endpoint
## is still assigned by the local O/S and uses an empheral port.
##
## Since the remote endpoint is assigned, we can use the write or send
## system calls rather than sendto.
##
## Furthermore, we do not need to use recvfrom, because the ONLY datagrams
## accepted by a connected socket are those that come from the specified
## remote endpoint; however, those messages can arrive on any
## interface and will be accepted, since the socket has not been bound
## a specific interface/port on this system.
##
## Lastly, errors are returned to the process for a connected UDP socket.
##
## This example also shows the use of "getpeername" to retrieve information
## about the "peer" (remote endpoint in this case), including the IP
## adddress and port.
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
sockfd.connect(sendAddress)

line = sys.stdin.readline()
while len(line) > 0:
    line = line.rstrip()
    sockfd.send(line.encode('utf-8'))
    line = sockfd.recv(8192)
    addr = sockfd.getpeername()
    print(f"Server received from {addr[0]} port {addr[1]}: {line.decode('utf-8').rstrip()}")
    line = sys.stdin.readline()
print("Done")
