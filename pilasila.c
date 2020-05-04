#include "header.h"

int count_lines(char* file){
  FILE* fp = fopen(file,"r");
  int lines=0;
  char ch;
  do{
    ch = fgetc(fp);
    if(ch == '\n'){
      lines++;
    }
  }while(ch != EOF);
  rewind(fp);
  return lines;
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
    buffer[teller] = malloc(sizeof(str) + 1);
    strcpy(buffer[teller], str);

    teller++;
  }
}

fclose(fp);
}

//read content in directory
void read_directory(){
  DIR* directory = opendir("big_set/");
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

long readPGM(char* file){
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

  fclose(fp);
  return filelen;
  free(data);
}


struct payload* create_packet(char *data,long byte, int b){
  pay = malloc(sizeof(struct payload));
  char * base = basename(buffer[b+'\0']);

  pay->seq_number = b;
  pay->filename = base;
  pay->filelen = strlen(buffer[b+'\0']);
  pay->data = malloc(byte*b);
  pay->data = data;
  //printf("Seqnummer:%d Fillengde:%d Filnavn:%s Data:%s \n",pay->seq_number,pay->filelen,pay->filename,pay->data);
  return pay;
  free(pay);
  free(pay->data);
}

struct header* create_header(char*data,int i,long c){
  unsigned int payload;
  unsigned int ack;
  unsigned int termination;
  int total;
  int sizeofdata = sizeof(data);

  struct header*head = malloc(sizeof(struct header));
  total = ((sizeof(struct payload))+(sizeof(struct header)+c));
  head->length = total;

  //printf("%d\n",head->length);
  return head;
  free(head);
}



int main(int argc, char* argv[]){
  char ch;
  int i,j;
  long b,c;
  int t = count_lines(argv[1]);
  read_file(argv[1]);
  read_directory();

  for(i=0; i<t; i++){
    b = readPGM(buffer[i]);
    //calling function struct packet to create packet with payload
    struct payload *pay = create_packet(data,b,i);
    struct header *header = create_header(data,b,c);
    free(pay);
  }
  //free(data);
}
