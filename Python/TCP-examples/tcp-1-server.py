#!/usr/bin/python
##
## Corresponds to tcp-1-server.c
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
me = "server"

##
## Print socket information
##
def print_socket_endpoints(name, sockfd):
    print("%s: Socket %s is connecting %s(me) and %s(them)" % (
        me, name, sockfd.getsockname(), sockfd.getpeername() ))

##
## Handle an interacton with a client
##
def handle_client(sockfd):
    while True:
        line = acceptfd.recv(8192)
        if line == '':
            print("End of input, exit")
            break
        print("%s: received %s" % (me, line))
    

##
## Create a socket, bind to any interface, specific port
##
listenfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sendAddress = (network, int(port))
listenfd.bind(sendAddress)
listenfd.listen(100)

##
## Accept connections forever
##
while True:
    acceptfd, theirAddr = listenfd.accept()
    print_socket_endpoints("listen", acceptfd)
    handle_client(acceptfd)
    acceptfd.close()
print("Done")
