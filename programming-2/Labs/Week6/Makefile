CC = gcc
CFLAGS = -Wall -Wextra -Wconversion -g -fanalyzer

all: program

program: main.o FileHandaling.o printing.o Logger.o
	$(CC) $(CFLAGS) main.o FileHandaling.o printing.o Logger.o -o program

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

FileHandaling.o: FileHandaling.c
	$(CC) $(CFLAGS) -c FileHandaling.c

printing.o: printing.c
	$(CC) $(CFLAGS) -c printing.c

Logger.o: Logger.c
	$(CC) $(CFLAGS) -c Logger.c

clean:
	rm -f *.o program
