CC = gcc

all: pm

pm: pm.o
	$(CC) pm.o -o pm
pm.o: pm.c
	$(CC) -c pm.c
clean:
	rm *.o pm
