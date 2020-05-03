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
#include "protocol.c"
#define MAGICNUM "P2"

struct header{
  int length;
  unsigned char seq_number;
  unsigned char seq_ack;
  unsigned char flagg;
  unsigned char unused;
  unsigned char* data;
};


int f;
unsigned char* data;
char* buffer[1000];
char* buffer1[1000];
unsigned long filelen;


void read_file(char*);
int count_lines(char*);
void read_directory();
void run_server(int);
void readthis(char*);
void readPGM(char*);
int filesize(char*);
char* serialize_int(char*,int);
char* serialize_char(char*, char);
char* serialize_header();
void create_packet(char*);
