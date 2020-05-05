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
#include <ctype.h>

#include "pgmread.c"
#include "pgmread.h"
#include "send_packet.c"

#define TIMEOUT 5
#define MAX 1024

struct payload{
  char* filename;
  char* data;
  int uniqnumber;
  unsigned char filelen;
};struct payload *pay;

struct packets{
  unsigned char seqnumber;
  char* packet;
  struct packets *next;
};

int f;
int* totalBytes;
char* data;
char* header;
char *packet;
char* buffer[1000];
char* buffer1[1000];
unsigned long filelen;

struct packets** linkedlist;
struct packets *point;
struct in_addr address;

int runudp();
void displayList();
void validateargs();
int filesize(char*);
char* readPGM(char*);
void read_file(char*);
void read_directory();
int count_lines(char*);
void display(struct payload*);
void removeNode(struct packets **, int);
struct payload* create_payload(char*,int,int);
void addNodeToList(struct packets**, struct packets*);
struct packets* build_struct_packet(char*, struct payload*);
char* create_header(struct payload*, unsigned char,unsigned char,int);
