CC=g++
CPPFLAGS=-Wall -ansi -Os

all: example1 example2
example1: socket.o http-server.o example1.o
example2: socket.o http-server.o example2.o

example1.o: example1.cc http-server.h socket.h
example2.o: example2.cc http-server.h socket.h
http-server.o: http-server.cc http-server.h socket.h
socket.o: socket.cc socket.h

clean:
	rm -f *.o example1 example2

