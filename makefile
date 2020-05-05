CC = gcc
CFLAGS = -Wall -Wconversion -Wextra -g
BINARIES = server client

# Hva er PHONY? Jo: targets som ikke er filer
.PHONY: all clean

all: $(BINARIES)

server: server.c
	$(CC) $(CFLAGS) $^ -o $@

client: client.c 
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(BINARIES)
