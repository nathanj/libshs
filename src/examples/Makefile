CC=g++
CPPFLAGS=-I.. -Wall -ansi -Os
LDLIBS=-L../shs -lshs

all: example1 example2
example1.o: example1.cc ../shs/libshs.a
example2.o: example2.cc ../shs/libshs.a

install:

clean:
	rm -f *.o example1 example2

