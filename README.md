# Sample TCP/UDP tutorials

## Purpose

This code illustrates basic client/server code for TCP and UDP messages in C and Python.

The goal is to illustrate the structure of TCP/UDP applications.

The following is a guide to the Python examples. The C examples are largely similar.

## TCP Examples

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

##

