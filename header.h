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
#include "pgmread.c"

#define MAX 1000

//Pointer to one image
struct Image *pgm;

void read_file(char*);
int count_lines(char*, int);
int read_directory();
void run_server(int);

void readPgmFile(FILE*);
void *img_read_pgm(FILE*);
