CC=clang
FLAGS=-Wall -Werror -g

main: main.o alloc.o
	$(CC) $(FLAGS) main.o alloc.o -o main

main.o: main.c alloc.h
	$(CC) $(FLAGS) -c main.c

alloc.o: alloc.c alloc.h
	$(CC) $(FLAGS) -c alloc.c

heap_test: heap_test.c
	$(CC) $(FLAGS) heap_test.c -o heap_test

clean:
	rm -rf *.o
	rm -rf main
	rm -rf heap_test
