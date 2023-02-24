FLAGS = -Wall -std=c99

all: main.c sock.o
	gcc $(FLAGS) main.c sock.o

sock.o: sock.c sock.h
	gcc $(FLAGS) -c -D_POSIX_C_SOURCE=200112 sock.c

clean:
	rm -f *.o a.out
