CC = gcc
CFLAGS = -Wall
OUTPUT1 = tanque.o
OUTPUT2 = pc.o
DEBUG = -g
SOURCES1 = tanque.c
SOURCES2 = pc.c
 
all:
	$(CC) $(SOURCES1) $(CFLAGS) -o $(OUTPUT1) -lncurses -pthread
	$(CC) $(SOURCES2) $(CFLAGS) -o $(OUTPUT2) -lncurses -pthread
debug:
	$(CC) $(SOURCES1) $(CFLAGS) -o $(OUTPUT1)
	$(CC) $(SOURCES2) $(CFLAGS) -o $(OUTPUT2)
clean:
	rm -f $(OUTPUT1)
	rm -f $(OUTPUT2)
fresh:
	make clean
	make all 
