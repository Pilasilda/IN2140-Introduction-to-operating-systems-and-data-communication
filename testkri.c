#include "header.h"
char* star[1000];
char* buffer1[1000];

typedef struct pcknode{
  int length;
  unsigned char id;
  unsigned char ack;
  unsigned char flags;
  unsigned char unused;
  unsigned char *buff; //buffer is data we want to send, need to make a function to send it, read one into buffer and data stream, sending buffer, server takes a buffer as a bytes
  struct pcknode *next;
}packetNode;

void addNode(packetNode **list, packetNode *new){
  if(list == NULL){
    new->next = NULL;
    list = &new;
  } else {
    new->next = *list;
    *list = new;
  }
}

void removeNode(packetNode **list, unsigned char seqNum){
  if(list == NULL)
    return;

  packetNode *prev = NULL;
  packetNode *temp = *list;
  while(temp != NULL && temp->id != seqNum){
    prev = temp;
    temp = temp->next;
  }

  if(prev == NULL){
    *list = temp->next;
    free(temp->buff);
    free(temp);
  } else if(temp != NULL){
    prev->next = temp->next;
    free(temp->buff);
    free(temp);
  }


}

//global stuff;
packetNode **packetList;
int id = 0;
char* start;


void freeBuffer(int t, char *buffer[]) {
  int i;
  for(i = 0; i < t; ++i) {
    free(buffer[i]);
  }
}

int count_lines(char* file) {
  char c;
  int count = 0;
  FILE* fp = fopen(file, "r");
  if (fp == NULL) {
    perror("countLines: Error opening textfile");
    return(-1);
  }
  c = fgetc(fp);
  while(c != EOF) {
    if(c == '\n') {
      count++;
    }
    c = fgetc(fp);
  }
  fclose(fp);
  return count;
}

//reading txt file
void read_file(char *file){
  char str[count_lines(file)];
  int teller = 0;
  FILE* fp = fopen(file, "r");

  if(fp == NULL) {
    perror("readFileNames: Error opening textfile");
    exit(-1);
  }

  if (file != NULL) {
    while(fscanf(fp, "%s", str) != EOF) {
      star[teller] = malloc(sizeof(str) + 1);
      strcpy(star[teller], str);

      teller++;
    }
  }

  fclose(fp);
}

//read content in directory
void read_directory(){
  DIR* directory = opendir("big_set");
  struct dirent *entry;
  char* pgmFile;
  int lines = 0;

  if(directory == NULL){
    perror("Unable to read directory.");
    exit(1);
  }

    while((entry = readdir(directory)) != NULL){
      //skip current and parent directory
      if(!strcmp(entry->d_name, ".")){
        continue;
      }
      if(!strcmp(entry->d_name,"..")){
        continue;
      }

      pgmFile = entry->d_name;
      buffer1[lines] = malloc(sizeof(pgmFile));
      strcpy(buffer1[lines],pgmFile);
      //printf("%s\n",buffer1[lines]);
      lines++;
    }
  closedir(directory);
}

void readthis(char* file){
  FILE* fp = fopen(file,"r");
  unsigned long filelen;
  int f;

  if(file == NULL ) {
    fprintf(stderr, "Bad file%s\n", file);
    exit(1);
  }

  if(fp == NULL){
    perror("file point at null\n");

    exit(1);
  }

  //get file length
  fseek(fp,0,SEEK_END);
  filelen = ftell(fp);
  data = malloc(filelen+1);
  fseek(fp,0,SEEK_SET);

  f = fread(data,1,filelen,fp);
  packetNode *new = malloc(sizeof(packetNode));
  new->id = f;
  //i++;
  new->flags = 0x1;
  new->length = f; //++ need to find a way to add the length of the whole file
  new->buff = data;
  addNode(packetList, new);

  //Image_create(data);
  printf("%d bytes read.\n",f);
  fclose(fp);
}

//void create_payload(){
  //struct header pack;
  //int i;
  //for(i = 0; i < sizeof(data); i++){
    //printf("%d\n", f);
  //}
//)}


int main(int argc, char* argv[]){
  char ch;
  int i;
  int t = count_lines(argv[1]);
  read_file(argv[1]);
  read_directory();

  for(i=0; i<t; i++){
    readthis(buffer1[i]);
  }

  free(data);
  freeBuffer(t, &start);
  freeBuffer(t, buffer1);
  //printf("%s\n", data);
  //printf("%s",data);
}
