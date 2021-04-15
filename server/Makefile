CC=g++
OBJS=server.o digraph.o dijkstra.o serialport.o
CFLAGS=-c -Wall -O2
LFLAGS=-static	
PROGRAM= server

$(PROGRAM): $(OBJS)
	$(CC) $(OBJS) -o $(PROGRAM) $(LFLAGS)

server.o: server.cpp wdigraph.h heap.h
	$(CC) server.cpp -o server.o $(CFLAGS)

digraph.o: digraph.cpp digraph.h 
	$(CC) digraph.cpp -o digraph.o $(CFLAGS)

dijkstra.o: dijkstra.cpp
	$(CC) dijkstra.cpp -o dijkstra.o $(CFLAGS)

serialport.o: serialport.cpp serialport.h
	$(CC) serialport.cpp -o serialport.o $(CFLAGS)

clean:
	@rm -f $(OBJS)
	@rm -f $(PROGRAM)