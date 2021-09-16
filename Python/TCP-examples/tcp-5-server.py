#!/usr/bin/env python3
##
## Corresponds to tcp-4-server.c. I omit tcp-3-server.py.
## Pair this with tcp-2-client
##
import socket
import sys,os,select


##
## Sample TCP server. This example does not fork a child for concurrency.
## Instead, it uses the "select" system call to wait for input
## on one of many possible sockets, including the listen socket.
## This allows a single threaded server to handle multiple clients concurrently
## in a single process with the shared state that implies.
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
## Create a socket, bind to any interface, specific port
##
listenfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sendAddress = (network, int(port))
listenfd.bind(sendAddress)
listenfd.listen(100)

##
## Accept connections forever
##
readList = [ listenfd ]
while True:
    print(f"{me}: wait for select")
    ##
    ## Select takes 3 lists of sockets - the read, write & other lists.
    ## The process will pause until it's possible to read, write or
    ## receive other notifications (e.g. a closed socket) on those
    ## sockets.
    ##
    ## In the example below, we are simply waiting for any client socket
    ## to have available data.
    ##
    rfd, wfd, ofd = select.select(readList, [], [])

    for fd in rfd:
        if fd == listenfd:
            ## Listen to a new connection?
            print(f"{me}: Listen for connection")
            acceptfd, theirAddr = listenfd.accept()
            print_socket_endpoints("listen", acceptfd)            
            readList.append( acceptfd )
        else:
            ## process a client command
            line = fd.recv(8192)
            if len(line) == 0:
                print("End of input, exit")
                fd.close()
                readList.remove( fd )
            else:
                print(f"{me}: received {line.decode('utf-8')}")
                fd.send(line)
