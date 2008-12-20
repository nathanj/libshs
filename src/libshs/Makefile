CC=g++
CPPFLAGS=-Wall -ansi -Os

PREFIX ?= /usr

all: libshs.a
libshs.a: http-server.o socket.o
	ar rc $@ $^
	ranlib $@

install: libshs.a http-server.h socket.h
	install -Dm644 libshs.a $(PREFIX)/lib/libshs.a
	install -Dm644 http-server.h $(PREFIX)/include/shs/http-server.h
	install -Dm644 socket.h $(PREFIX)/include/shs/socket.h

http-server.o: http-server.cc http-server.h socket.h
socket.o: socket.cc socket.h

clean:
	rm -f *.o *.a

