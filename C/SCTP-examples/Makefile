CC	=c++
CFLAGS	=-O -g -Wall -I.
CXXFLAGS	=-O -g -Wall -I.
LIBS	= support.o

OBJ	= \
	sctp-1-server sctp-1-client sctp-1-client-bindx \
	sctp-2-server sctp-2-client \
	sctp-3-server sctp-3-client \
	sctp-4-server sctp-4-client \
	sctp-5-server sctp-5-client \
	sctp-6-server

default: $(OBJ)

sctp-1-server: sctp-1-server.o $(LIBS)
	$(CC) $(CFLAGS) -o sctp-1-server sctp-1-server.o $(LIBS)

sctp-1-client: sctp-1-client.o $(LIBS)
	$(CC) $(CFLAGS) -o sctp-1-client sctp-1-client.o $(LIBS)

sctp-1-client-bindx: sctp-1-client-bindx.o bindx.o $(LIBS)
	$(CC) $(CFLAGS) -o sctp-1-client-bindx sctp-1-client-bindx.o bindx.o $(LIBS)

sctp-2-server: sctp-2-server.o $(LIBS)
	$(CC) $(CFLAGS) -o sctp-2-server sctp-2-server.o $(LIBS)

sctp-2-client: sctp-2-client.o $(LIBS)
	$(CC) $(CFLAGS) -o sctp-2-client sctp-2-client.o $(LIBS)

sctp-3-server: sctp-3-server.o $(LIBS)
	$(CC) $(CFLAGS) -o sctp-3-server sctp-3-server.o $(LIBS)

sctp-3-client: sctp-3-client.o $(LIBS)
	$(CC) $(CFLAGS) -o sctp-3-client sctp-3-client.o $(LIBS)

sctp-4-server: sctp-4-server.o $(LIBS)
	$(CC) $(CFLAGS) -o sctp-4-server sctp-4-server.o $(LIBS)

sctp-4-client: sctp-4-client.o $(LIBS)
	$(CC) $(CFLAGS) -o sctp-4-client sctp-4-client.o $(LIBS)

sctp-5-server: sctp-5-server.o $(LIBS)
	$(CC) $(CFLAGS) -o sctp-5-server sctp-5-server.o $(LIBS)

sctp-5-client: sctp-5-client.o $(LIBS)
	$(CC) $(CFLAGS) -o sctp-5-client sctp-5-client.o $(LIBS)

sctp-6-server: sctp-6-server.o $(LIBS)
	$(CC) $(CFLAGS) -o sctp-6-server sctp-6-server.o $(LIBS)

sctp-6-client: sctp-6-client.o $(LIBS)
	$(CC) $(CFLAGS) -o sctp-6-client sctp-6-client.o $(LIBS)

clean:
	-rm *.o
	rm -f $(OBJ)

