CC	=g++
CFLAGS	=-O -g -Wall
CXXFLAGS	=-O -g -Wall
DEPS	= support.h support.o
LIBS	= support.o

EXE	=.exe
OBJ	= \
	tcp-1-server$(EXE) tcp-1-client$(EXE) \
	tcp-2-server$(EXE) tcp-2-client$(EXE) \
	tcp-3-server$(EXE) tcp-3-client$(EXE) \
	tcp-4-server$(EXE) tcp-4-client$(EXE) \
	tcp-5-server$(EXE) tcp-5-client$(EXE) \
	tcp-6-server$(EXE) 

default: $(OBJ)

tcp-1-server$(EXE): tcp-1-server.o $(DEPS)
	$(CC) $(CFLAGS) -o tcp-1-server$(EXE) tcp-1-server.o $(LIBS)

tcp-1-client$(EXE): tcp-1-client.o $(DEPS)
	$(CC) $(CFLAGS) -o tcp-1-client$(EXE) tcp-1-client.o $(LIBS)

tcp-2-server$(EXE): tcp-2-server.o $(DEPS)
	$(CC) $(CFLAGS) -o tcp-2-server$(EXE) tcp-2-server.o $(LIBS)

tcp-2-client$(EXE): tcp-2-client.o $(DEPS)
	$(CC) $(CFLAGS) -o tcp-2-client$(EXE) tcp-2-client.o $(LIBS)

tcp-3-server$(EXE): tcp-3-server.o $(DEPS)
	$(CC) $(CFLAGS) -o tcp-3-server$(EXE) tcp-3-server.o $(LIBS)

tcp-3-client$(EXE): tcp-3-client.o $(DEPS)
	$(CC) $(CFLAGS) -o tcp-3-client$(EXE) tcp-3-client.o $(LIBS)

tcp-4-server$(EXE): tcp-4-server.o $(DEPS)
	$(CC) $(CFLAGS) -o tcp-4-server$(EXE) tcp-4-server.o $(LIBS)

tcp-4-client$(EXE): tcp-4-client.o $(DEPS)
	$(CC) $(CFLAGS) -o tcp-4-client$(EXE) tcp-4-client.o $(LIBS)

tcp-5-server$(EXE): tcp-5-server.o $(DEPS)
	$(CC) $(CFLAGS) -o tcp-5-server$(EXE) tcp-5-server.o $(LIBS)

tcp-5-client$(EXE): tcp-5-client.o $(DEPS)
	$(CC) $(CFLAGS) -o tcp-5-client$(EXE) tcp-5-client.o $(LIBS)

tcp-6-server$(EXE): tcp-6-server.o $(DEPS)
	$(CC) $(CFLAGS) -o tcp-6-server$(EXE) tcp-6-server.o $(LIBS)

tcp-6-client$(EXE): tcp-6-client.o $(DEPS<)
	$(CC) $(CFLAGS) -o tcp-6-client$(EXE) tcp-6-client.o $(LIBS)

support.o: support.cc support.h

clean:
	-rm *.o
	rm -f $(OBJ)

