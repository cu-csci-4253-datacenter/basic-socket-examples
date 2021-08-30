# Sample TCP/UDP tutorials

## Purpose

This code illustrates basic client/server code for TCP and UDP messages in C and Python.

The goal is to illustrate the structure of TCP/UDP applications.

The following is a guide to the Python examples. The C examples are largely similar.

## Python TCP Examples

* tcp-1-client.py / tcp-1-server.py -- This is a simple client/server where in the client connects to the server and sends messages (lines of text).
* tcp-2-client.py / tcp-2-server.py -- This extends the prior examples to have the
client both send to the server and then receive a reply from the server (the same message).
In this example, the server can only interact with a single client.
* tcp-2-client.py / tcp-4-server.py -- This extends the tcp-2-server.py example
by having the TCP server be able to accept connections from multiple clients
by "forking" the server process.
* tcp-2-client.py / tcp-5-server.py -- This extends the tcp-2-server.py example
by having the TCP server be able to accept connections from multiple clients
by using the `select()` system call to check for available data from the clients

## Python UDP Examples
* udp-1.py - this program simply accepts a network address and then prints it out using
"dotted quad" notation and then as a presentation address.
* udp-2-client.py / udp-2-server.py - The client send a message to the server, which doesn't repond
* udp-3-client.py / udp-3-server.py - The client send a message to the server, and then waits for a response from the server
* udp-4-client.py / udp-3-server.py - The client send a message to the server, and then waits for a response from the server. Unlike the udp-3-client.py example, here the client uses `connect()` to specify the server address as a property of the socket rather than specifying that address using `sendto` each time.
* udp-5-client.py / udp-3-server.py - This extends udp-c-client.py to explicit specify the **client** UDP port using the `bind()` system call
* udp-6-client.py / udp-3-server.py - This example shows how to use the `SO_REUSEADDR` socket option to allow multiple sockets to use the same client port address. This can be useful for multi-threaded UDP code.
* udp-7-client.py / udp-3-server.py - This example shows how to use the `SO_BROADCAST` option to enable broadcast packets
* udp-8-client.py / udp-3-server.py - This example shows how to use the `SO_SNDBUF` and `SO_RCVBUF` options to explicitly set the send and receive buffer sizes.


