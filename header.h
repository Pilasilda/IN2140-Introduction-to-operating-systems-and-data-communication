#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/stat.h>
#include <libgen.h>

#include "pgmread.c"
#include "pgmread.h"

#define TIMEOUT 5
#define MAX 1024

typedef struct payload{
  char* filename;
  char* data;
  unsigned char seq_number;
  unsigned char filelen;
  struct payload *next;
}packet;

struct header{
  int length;
  char*data;
  unsigned char seq_number;
  unsigned char ack;
  unsigned char flag;
  unsigned char unused;
};

int f;
char* data;
struct payload *pay;
char* buffer[1000];
char* buffer1[1000];
unsigned long filelen;
int windowlength;

struct payload** linkedlist;

void read_file(char*);
int count_lines(char*);
void read_directory();
long readPGM(char*);
int filesize(char*);
void addNodeToList(packet**, packet*);
void removeNode(packet **, unsigned char);
void displayList();
int runudp();
struct payload* create_packet(char *,long, int);
struct header* create_header(char*,int,long);
