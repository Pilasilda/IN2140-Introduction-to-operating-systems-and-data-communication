#include "header.h"

//method for validating commandline arguments
void validate(int argc,char*argv[]){
  int test;

  if(argc < 3){
    printf("Usage: %s IP port\n",argv[0]);
    exit(EXIT_FAILURE);
  }
  test = inet_pton(AF_INET,argv[1],&address);
  if(test == 0){
    printf("%s is an invalid IPv4 address.\n",argv[1]);
    exit(EXIT_FAILURE);
  }
  size_t i;
  for(i =0; i< strlen(argv[2]); i++){
    if(!isdigit(argv[2][i])){
      printf("Invalid port number %s\n",argv[2]);
      exit(EXIT_FAILURE);
    }
  }

  test = atoi(argv[2]);

  if(test < 1024 || test > 0xffff){
    printf("Port must be between an interval of 1024-65535.\n");
    exit(EXIT_FAILURE);
  }
}

//count line in file used in main
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

struct payload* create_payload(char *data,long byte, int b,int uniq_number){
  pay = malloc(sizeof(struct payload));
  char * base = basename(buffer[b]);
  //buffer[0] = '\0';

  pay->uniqnumber = uniq_number;
  pay->filename = base;
  pay->filelen = strlen(buffer[b]);
  pay->data = malloc(byte*b);
  pay->data = data;
  //printf("Seqnummer:%d Fillengde:%d Filnavn:%s Data:%s \n",pay->uniqnumber,pay->filelen,pay->filename,pay->data);
  return pay;
  free(pay);
  free(pay->data);
}

void convertStructtoBuff(){

}

struct header* create_header(char*data,int i,long c){
  unsigned int payload;
  unsigned int ack;
  unsigned int termination;
  int total;
  int sizeofdata = sizeof(data);
  unsigned char unused = 0x7f;
  head = malloc(sizeof(struct header));
  int size = sizeof(data);
  int sizeHeader = sizeof(struct header);
  int sizePayload = sizeof(struct payload);
  char* b;
  b = buffer[i];

  head->length = (sizeHeader+sizePayload+size);
  head->seq_num = b;

  printf("%s\n", head->seq_num);
  //return head;
  //free(head);
  return head;
}

//adding node to linkedlist
void addNodeToList(struct payload **list, struct payload *newlist){
  if(list == NULL){
    newlist->next = NULL;
    list = &newlist;
  }else{
    newlist->next = *list;
    *list = newlist;
  }
  //printf("%s\n",newlist->data);
}

void removeNode(struct payload **list, int seqnumber){
  if(list == NULL)
    return;

  struct payload *previous = NULL;
  struct payload *temp = *list;

  while(temp != NULL && temp->uniqnumber != seqnumber){
    previous = temp;
    temp = temp->next;
  }

  if(previous == NULL){
    list = &temp->next;
    free(temp->data);
    free(temp);
  }else if(temp != NULL){
    previous->next = temp->next;
    free(temp->data);
    free(temp);
  }
}

//method for displaying linkedlist
void displayList(struct payload* pack){
  while(pack != NULL){
    pack = pack->next;
  }
}

int main(int argc, char* argv[]){
  char ch;
  int i,j,k,udp_socket;
  long b,c;
  int t = count_lines(argv[1]);
  read_file(argv[1]);
  read_directory();
  int uniq = 1;
  //validate(argc,argv);
  struct sockaddr_in clientaddr;
  socklen_t size = sizeof(clientaddr);

  //clientaddr.sin_family = AF_INET;
  //clientaddr.sin_port = htons(atoi(argv[2]));
  //clientaddr.sin_addr = address;

  //create udp socket
  //udp_socket = socket(AF_INET, SOCK_DGRAM, 0);

  //connect socket
  /*if(connect(udp_socket, (struct sockaddr*)&clientaddr,sizeof(clientaddr)) < 0){
    perror("Error while creatng socket");
    exit(EXIT_FAILURE);
  }*/

  //send_packet(udp_socket, packet, sizeof(buffer), int flags, clientaddr, size);
  //recvfrom(udp_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL,NULL);

  linkedlist = malloc(sizeof(struct payload)*t);

  for(i=0; i<t; i++){
    b = readPGM(buffer[i]);
    //calling function struct packet to create packet with payload
    struct payload *pe = create_payload(data,b,i,uniq);
    struct header *he = create_header(data,b,i);
    //addNodeToList(linkedlist,pe);
    //displayList(pe);
    //struct header *header = create_header(data,b,c);
  }

  //Close socket descriptors
  close(udp_socket);
  return 0;
  //displayList();
  //free(data);
}
