CC	=c++
CFLAGS	=-O -g -Wall
LIBS	= support.o
EXE = .exe

OBJ	= udp-1$(EXE) \
	udp-2-server$(EXE) \
	udp-3-server$(EXE) \
	udp-8-server$(EXE) \
	udp-2-client$(EXE) \
	udp-3-client$(EXE) \
	udp-4-client$(EXE) \
	udp-5-client$(EXE) \
	udp-6-client$(EXE) \
	udp-7-client$(EXE) \
	udp-server$(EXE) \
	mcast-recv$(EXE) mcast-send$(EXE) \
	udp-select-example$(EXE) \
	udp-relay$(EXE) \
	udp-struct$(EXE)

default: $(OBJ)

udp-1$(EXE): udp-1.o support.o
	$(CC) $(CFLAGS) -o udp-1$(EXE) udp-1.o support.o
##
## Servers
##
udp-2-server$(EXE): udp-2-server.o support.o
	$(CC) $(CFLAGS) -o udp-2-server$(EXE) udp-2-server.o support.o
udp-3-server$(EXE): udp-3-server.o support.o
	$(CC) $(CFLAGS) -o udp-3-server$(EXE) udp-3-server.o support.o
udp-8-server$(EXE): udp-8-server.o support.o
	$(CC) $(CFLAGS) -o udp-8-server$(EXE) udp-8-server.o support.o

##
## Clients
##
udp-2-client$(EXE): udp-2-client.o support.o
	$(CC) $(CFLAGS) -o udp-2-client$(EXE) udp-2-client.o support.o
udp-3-client$(EXE): udp-3-client.o support.o
	$(CC) $(CFLAGS) -o udp-3-client$(EXE) udp-3-client.o support.o
udp-4-client$(EXE): udp-4-client.o support.o
	$(CC) $(CFLAGS) -o udp-4-client$(EXE) udp-4-client.o support.o
udp-5-client$(EXE): udp-5-client.o support.o
	$(CC) $(CFLAGS) -o udp-5-client$(EXE) udp-5-client.o support.o
udp-6-client$(EXE): udp-6-client.o support.o
	$(CC) $(CFLAGS) -o udp-6-client$(EXE) udp-6-client.o support.o
udp-7-client$(EXE): udp-7-client.o support.o
	$(CC) $(CFLAGS) -o udp-7-client$(EXE) udp-7-client.o support.o
udp-8-client$(EXE): udp-8-client.o support.o
	$(CC) $(CFLAGS) -o udp-8-client$(EXE) udp-8-client.o support.o


##
## Multicast
##
mcast-recv$(EXE): mcast-recv.o support.o
	$(CC) $(CFLAGS) -o mcast-recv$(EXE) mcast-recv.o support.o
mcast-send$(EXE): mcast-send.o support.o
	$(CC) $(CFLAGS) -o mcast-send$(EXE) mcast-send.o support.o
udp-select-example$(EXE): udp-select-example.o support.o
	$(CC) $(CFLAGS) -o udp-select-example$(EXE) udp-select-example.o support.o
udp-relay$(EXE): udp-relay.o support.o
	$(CC) $(CFLAGS) -o udp-relay$(EXE) udp-relay.o support.o
udp-struct$(EXE): udp-struct.o support.o
	$(CC) $(CFLAGS) -o udp-struct$(EXE) udp-struct.o support.o
udp-server$(EXE): udp-server.o support.o
	$(CC) $(CFLAGS) -o udp-server$(EXE) udp-server.o support.o

clean:
	-rm *.o
	rm -f $(OBJ)
