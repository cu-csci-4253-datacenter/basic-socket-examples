#!/usr/bin/python
##
## Corresponds to tcp-6-server.c.
## Pair this with tcp-2-client
##
import socket
import sys,os,select


##
## Sample TCP server. This example does not fork a child for concurrency.
## Instead, it uses the "poll" system call to wait for input
## on one of many possible sockets, including the listen socket.
## This allows a single threaded server to handle multiple clients concurrently.
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

mypoll = select.poll()

mypoll.register( listenfd, select.POLLIN )

while True:
    print("%s: wait for select" % ( me ))

    fds = mypoll.poll()
    ##
    ## Note that 'poll' returns file descriptors, and we need to
    ## translate between socket objects and file descriptors
    ##
    for item in fds:
        fd, event = item[0], item[1]
        if fd == listenfd.fileno():
            ## Listen to a new connection?
            print("%s: Listen for connection" % ( me ))
            acceptfd, theirAddr = listenfd.accept()
            print_socket_endpoints("listen", acceptfd)            
            mypoll.register( acceptfd.fileno(), select.POLLIN )
        else:
            ## process a client command
            line = os.read(fd, 8192)
            if line == '':
                print("End of input, exit")
                os.close(fd)
                mypoll.unregister( fd )
            else:
                print("%s: received %s" % (me, line))
                os.write(fd, line)
