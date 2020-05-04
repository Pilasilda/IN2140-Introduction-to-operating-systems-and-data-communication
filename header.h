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

struct payload{
  char* filename;
  char* data;
  int seq_number;
  unsigned char filelen;
  struct payload *next;
};struct payload *pay;

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
char *header;
char* buffer[1000];
char* buffer1[1000];
unsigned long filelen;

struct payload** linkedlist;

void read_file(char*);
int count_lines(char*);
void read_directory();
long readPGM(char*);
int filesize(char*);
void addNodeToList(struct payload**, struct payload*);
void removeNode(struct payload **, int);
void displayList();
int runudp();
//struct payload* create_packet(char *,long, int);
//struct header* create_header(char*,int,long);
void display(struct payload*);
char* create_payload(int,long, int,char*);
//char* create_header(int, unsigned char, unsigned char, unsigned char);
