CC=g++
CPPFLAGS=-Wall -ansi -Os

all: example1 example2
example1: libshs.a
example2: libshs.a
libshs.a: http-server.o socket.o
	ar rc $@ $^
	ranlib $@

example1.o: example1.cc http-server.h socket.h
example2.o: example2.cc http-server.h socket.h
http-server.o: http-server.cc http-server.h socket.h
socket.o: socket.cc socket.h

clean:
	rm -f *.o *.a example1 example2

