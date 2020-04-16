CC=g++
OBJS= server server.o digraph.o serialport.o
CFLAGS=-c -Wall -O2
PROGRAM=prog

PROGRAM: $(OBJS)

server: server.o digraph.o serialport.o
	$(CC) server.o digraph.o serialport.o -o server

server.o: server.cpp
	$(CC) server.cpp -o server.o $(CFLAGS)


digraph.o: digraph.cpp
	$(CC) digraph.cpp -o digraph.o $(CFLAGS)

serialport.o: serialport.cpp
	$(CC) serialport.cpp -o serialport.o $(CFLAGS)

clean:
	@rm -f $(OBJS)
	@rm -f $(PROGRAM)
