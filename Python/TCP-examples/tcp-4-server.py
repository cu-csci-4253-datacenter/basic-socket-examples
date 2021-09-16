#!/usr/bin/env python3
##
## Corresponds to tcp-4-server.c. I omit tcp-3-server.py.
## Pair this with tcp-2-client
##
import socket
import sys,os


##
## Sample TCP server - receive message, return it to client
## Fork for each connectin client. The only difference with tcp-2-server
## is use of fork in main code which allows the server to handle
## multiple clients simultaneously, although in different processes.
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
        line = sockfd.recv(8192)
        if len(line) == 0:
            print("End of input, exit")
            return
        print(f"{me}: received {line.decode('utf-8')}")
        # Send it back to client
        sockfd.send(line)
    

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
    print("%s: Listen for connection" % ( me ))
    acceptfd, theirAddr = listenfd.accept()
    pid = os.fork()
    if pid == 0:
        print_socket_endpoints("listen", acceptfd)
        handle_client(acceptfd)
        acceptfd.close()
        sys.exit(0)
    else:
        acceptfd.close() ## parent doesn't need this
print("Done")
