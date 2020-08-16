CC=clang
FLAGS=-Wall -Werror -g

main: main.o alloc.o
	$(CC) $(FLAGS) main.o alloc.o -o main

main.o: main.c alloc.h
	$(CC) $(FLAGS) -c main.c

alloc.o: alloc.c alloc.h
	$(CC) $(FLAGS) -c alloc.c

clean:
	rm -rf *.o
	rm -rf main
