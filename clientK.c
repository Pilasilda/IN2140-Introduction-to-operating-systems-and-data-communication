#include "header.h"
int runudp(){
  //char *file;
  struct sockaddr_in serveraddr;
  int udp_socket, count,ch;
  char buffer[1000];
  char *msg = "Hello server";

  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serveraddr.sin_port = htons(5000);
  serveraddr.sin_family = AF_INET;

  //create udp socket
  udp_socket = socket(AF_INET, SOCK_DGRAM, 0);

  //connect socket
  if(connect(udp_socket, (struct sockaddr*)&serveraddr,sizeof(serveraddr)) < 0){
    perror("Error while creatng socket");
    exit(EXIT_FAILURE);
  }

  sendto(udp_socket, msg, MAX,0, (struct sockaddr*) NULL, sizeof(serveraddr));
  recvfrom(udp_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL,NULL);
  //puts(buffer);
  //printf("Message sent from server: %s\n", buffer);

  //Close socket descriptor
  close(udp_socket);
  return 0;
}

//countlines
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

/*struct payload* create_packet(char *data,long byte, int b){
  pay = malloc(sizeof(struct payload));
  char * base = basename(buffer[b]);

  pay->seq_number = b;
  pay->filename = base;
  pay->filelen = strlen(buffer[b]);
  pay->data = malloc(byte*b);
  pay->data = data;
  //printf("Seqnummer:%d Fillengde:%d Filnavn:%s Data:%s \n",pay->seq_number,pay->filelen,pay->filename,pay->data);
  return pay;
  free(pay);
  free(pay->data);
}*/

char* create_header(int length, unsigned char seq_num, unsigned char seq_num_last, unsigned char flags){
  //rekkefølge er fra assignment er
  // <<fil lengde, seq_num, seq_num_last, flags, unused = 0x7f(alltid);
  // så lengde for char in bytes er, int+char+char+char+char
  packet = malloc(sizeof(int) + sizeof(char) * 4);
  int len;
  len = length;
  unsigned char unused = 0x7f;
  //memcpy, shifter plass og setter griene inni riktig rekkefølge
  //samme som header.len(som har size int) = len
  memcpy(packet, &len, sizeof(int));
  //header må shift plass til char, så samme som indexing inni en array, hvis det gir mening
  //måter for å gjøre der er nede
  memcpy(packet + sizeof(int), &seq_num, 1);
  memcpy(packet + sizeof(int) + sizeof(char), &seq_num_last, 1);
  memcpy(packet + sizeof(int) + sizeof(char) + sizeof(char), &flags, 1);
  memcpy(packet + sizeof(int) + sizeof(char) + sizeof(char) + sizeof(char), &unused, 1);
  return packet;
}

//payload
char* create_payload(int uid, char* data, long byte,int b){
  char* base = basename(buffer[b]);
  char *payload = malloc(sizeof(int) * 2 + sizeof(char) + sizeof(int));
  char* bytesofimages;
  char* filename;
  int i;
  i=uid;

  int length = strlen(buffer[b]);
  bytesofimages = malloc(byte*b);
  bytesofimages = data;
  //printf("%d\n",i);
  //memcpy(packet,(char*)&i, sizeof(int));
  //printf("%s\n",header);
  //memcpy(packet + sizeof(int) * 2,(char*)&length, sizeof(int));
  //printf("%s\n",header);
  memcpy(packet,base,sizeof(char));
  puts(packet);
  //printf("%s\n", packet);

  //memcpy(packet + sizeof(int) * 2 + sizeof(char), &base, 1);//usikker siden det er navn av fil
  //memcpy(header + sizeof(int) *2 + sizeof(char) + sizeof(int), &bytesofimages, sizeof(int));
  return packet;
}

/*void addNodeToList(packet **list, packet*new){
  if(list == NULL){
    new->next = NULL;
    list = &new;
  }else{
    new->next = *list;
    *list = new;
  }
}

void removeNode(packet **list, unsigned char seqnumber){
  if(list == NULL)
    return;

  packet *previous = NULL;
  packet *temp = *list;

  while(temp != NULL && temp->seq_number != seqnumber){
    previous = temp;
    temp = temp->next;
  }

  if(previous == NULL){
    *list = temp->next;
    free(temp->data);
    free(temp);
  }else if(temp != NULL){
    previous->next = temp->next;
    free(temp->data);
    free(temp);
  }
}*/

/*void displayList(){
  packet* pack;
  while(pack != NULL){
    printf("heheheheh");
    //pack = pack->next;
  }
}*/

int main(int argc, char* argv[]){
  char ch;
  int i,j;
  long b,c;
  int t = count_lines(argv[1]);
  read_file(argv[1]);
  read_directory();
  int p=1;

  //linkedlist = malloc(sizeof(struct payload)*t);
  for(i=0; i<t; i++){
    b = readPGM(buffer[i]);
    create_payload(p,data,b,i);
    //calling function struct packet to create packet with payload
    //struct payload *pe = create_packet(data,b,i);
    //addNodeToList(linkedlist,pe);
    //displayList();
    //struct header *header = create_header(data,b,c);
  }

  //displayList();
  //free(data);
}
