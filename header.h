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
#define MAGICNUM "P2"

struct header{
  char magic_number[2];
  int width;
  int height;
  int max_value;
  int rest;
};

struct node{
  char magic_number[2];
  int width;
  int height;
  int max_value;
  int rest;
  unsigned char* data;
  struct node* next;
};

char* data;

//Pointer to one image
struct Image *pgm;

char* buffer[1000];
char* buffer1[1000];

void read_file(char*);
int count_lines(char*);
void read_directory();
void run_server(int);
void readthis(char*);
char read_image(char*);
int filesize(char*);
char* serialize_int(char*,int);
char* serialize_char(char*, char);
char* serialize_header();
