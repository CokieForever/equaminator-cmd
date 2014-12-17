
CC = gcc
CFLAGS = -Wall -O4

eispack: Makefile eispack.o
	ar cr libeispack.a eispack.o
eispack.o: Makefile eispack.c eispack.h
	${CC} ${CFLAGS} -c eispack.c

clean: 
	rm *.o
	rm *.a

