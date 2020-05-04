CC = gcc
C= -g -D_GNU_SOURCE
BINARIES = client server

.PHONY: all clean

all: $(BINARIES)

client: client.c pgmread.c
$(CC) $(CFLAGS) $^ -o $@

server: server.c
$(CC) $(CFLAGS) $^ -o $@

run: all

clean:
	rm -rf client
	rm -rf node
