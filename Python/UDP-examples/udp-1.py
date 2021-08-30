#!/usr/bin/env python3
##
## Corresponds to udp-1.c in C files.
## Not really needed in practice in Python since socket
## calls take presentation format arguments anyway via
## dynamic typing.
##

##
## Example: ./python3 udp-1.py 127.0.0.1
##

import socket
import sys,os

print("Argument is ", sys.argv[1])

asNetwork = socket.inet_pton(socket.AF_INET, sys.argv[1])
print("As a network address, it looks like", asNetwork, ":", end='')
sep = ""
for byte in asNetwork:
    print("%s%03d" % (sep, byte), end='')
    sep = "."
print("")

asPresentation = socket.inet_ntop(socket.AF_INET, asNetwork)
print("As a presentation address, it looks like", asPresentation)

