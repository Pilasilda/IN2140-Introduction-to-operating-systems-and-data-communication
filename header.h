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

typedef struct payload{
  char* filename;
  char* data;
  int uniqnumber;
  unsigned char filelen;
}pack;

typedef struct packets{
  int frame_ack;
  unsigned char seqnumber;
  char* packet;
  unsigned char ack;
  unsigned char flagg;
  struct packets *next;
}Frame;

int f;
int* totalBytes;
char* data;
char* header;
char *packet;
char* ack[1000];
char* buffer[1000];
char* buffer1[1000];
int udp_socket;

struct packets** linkedlist;
struct packets *point;
struct in_addr address;
struct payload *pakke = NULL;
struct payload *pay;

int runudp();
void displayList();
void bufferFree(int t, char*[]);
void validateargs();
char* readPGM(char*);
void read_file(char*);
void read_directory();
int count_lines(char*);
void display(struct payload*);
void removeNode(struct packets **, int);
struct payload* create_payload(char*,int,int);
void addNodeToList(struct packets**, struct packets*);
struct packets* build_struct_packet(char*, struct payload*);
char* create_header(struct payload*,unsigned char,unsigned char,int);
