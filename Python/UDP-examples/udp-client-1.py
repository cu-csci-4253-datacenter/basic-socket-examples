#!/usr/bin/python
import socket
import sys,os
import random

if len(sys.argv) < 5:
    print("usage:", sys.argv[0], "<relayport> <svrIP> <svrPort> <dropProb>")
    sys.exit(1)

relayPort = sys.argv[1]
svrIP = sys.argv[2]
svrPort = sys.argv[3]
dropProb = float(sys.argv[4])

print("Receive packets on", relayPort)
print("Send to server at ", svrIP, "port", svrPort, "with drop %", dropProb)

fromClient = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
fromClient.bind(('', int(relayPort)))

serverAddr = (svrIP, int(svrPort))

clientAddr = None

while True:
    print("Prepare to read")
    data,fromAddr = fromClient.recvfrom(1024)
    print("got ", data, "from ", fromAddr)
    if clientAddr == None or fromAddr != serverAddr:
        print("Pick new client addr")
        clientAddr = fromAddr
        if random() < dropProb:
            fromClient.sendto(data, serverAddr)
    else:
        print("Forward data to client")
        if random() < dropProb:
            fromClient.sendto(data, clientAddr)
