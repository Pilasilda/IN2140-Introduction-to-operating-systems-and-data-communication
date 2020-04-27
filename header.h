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
#include "pgmread.c"
#include "pgmread.h"

//Pointer to one image
struct Image *pgm;
char* buffer[1000];
char* buffer1[1000];

void read_file(char*);
int count_lines(char*);
void read_directory();
void run_server(int);

void read_image(char*);
void skipComments(FILE*);
//void readPgmFile();
