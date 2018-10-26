CC = gcc

all: shell

shell: shell.o
	$(CC) shell.o -o shell
shell.o: shell.c
	$(CC) -c shell.c

clean:
	rm *.o shell
