 #include "header.h"
//Method for freeing buffer
void bufferFree(int t, char* buffer[]){
  int i;
  for(i=0; i <t; i++){
    free(buffer[i]);
  }
}

//method for validating commandline arguments
void validate(int argc,char*argv[]){
  int test;

  if(argc < 4){
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
    //reading it into buffer,allocating buffer
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
  char filename[255];

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

      char * file = buffer[lines];
      char * directoryName = dirname(file);

      strcpy(filename,directoryName);
      pgmFile = entry->d_name;
      strncat(filename,entry->d_name,20);
      buffer1[lines] = malloc(strlen(filename)+1);
      strcpy(buffer1[lines],filename);
      //printf("%s\n",buffer1[lines]);
      lines++;
    }
  closedir(directory);
}

char* readPGM(char* file){
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
  fseek(fp,0,SEEK_SET);
  //alloc for global char*data to store image information
  data = malloc(filelen);

  if(data == NULL){
    exit(EXIT_FAILURE);
  }
  //reading pgm file as a chunk of bytes, storing it to char*data
  f = fread(data,1,filelen,fp);
  //store totalbyte of each file in int* which is global for later use
  totalBytes = malloc(sizeof(int));
  totalBytes = &f;

  fclose(fp);
  return data;
}

//method for creating payload
struct payload* create_payload(char *data,int b,int bytes){
  pay = malloc(sizeof(struct payload));
  char * base = basename(buffer[b+'\0']);

  pay->uniqnumber = b;
  pay->filename = base;
  pay->filelen = strlen(buffer[b]+1);
  pay->data = malloc(b*bytes);
  pay->data = data;
  //printf("Seqnummer:%d Fillengde:%d Filnavn:%s Data:%s \n",pay->uniqnumber,pay->filelen,pay->filename,pay->data);
  return pay;
  free(pay);
  free(pay->data);
}

//method for creating header
char* create_header(struct payload *payload,unsigned char last_number,unsigned char flag ,int totalBytes){
  //allocating space for all variables
  header = malloc(sizeof(int) + sizeof(char)*4 + totalBytes);
  unsigned char unused = 0x7f;
  totalBytes = strlen(pay->data);
  int length = (sizeof(struct packets)+ sizeof(payload) + totalBytes);
  int test = length +'0';
  int uniq = pay->uniqnumber;

  memcpy(header, &test, sizeof(int));
  memcpy(header + sizeof(int), &uniq, 1);
  memcpy(header + sizeof(int) + sizeof(char), &last_number,1);
  memcpy(header + sizeof(int) + sizeof(char) + sizeof(char), &flag ,1);
  memcpy(header + sizeof(int) + sizeof(char) + sizeof(char) + sizeof(char), &unused, 1);
  memcpy(header + sizeof(int) + sizeof(char) + sizeof(char) + sizeof(char) + sizeof(char), pay, 1);

  return header;
}

struct packets* build_struct_packet(char* tmp, struct payload* uid){
  //alloc for struct pointer
  point = malloc(sizeof(struct packets));
  point->seqnumber = uid->uniqnumber;
  point->packet = tmp;

  //printf("%s\n", tmp);
  return point;
}

//method for creating a packet combining header+payload
char* build_packet(char* tmp, struct payload* pay, int totalBytes){
  packet = malloc(sizeof(tmp) + sizeof(struct payload)+totalBytes);

  memcpy(packet + sizeof(tmp), tmp, 1);
  memcpy(packet + sizeof(tmp) + sizeof(pay), pay, 1);

  //printf("%x\n", *(uint32_t *)packet);
  return packet;
}


//adding node to linkedlist
void addNodeToList(struct packets **list, struct packets *newlist){
  if(list == NULL){
    newlist->next = NULL;
    list = &newlist;
  }else{
    newlist->next = *list;
    *list = newlist;
  }
  //printf("%s\n",newlist->data);
}

void removeNode(struct packets **list, int snumber){
  if(list == NULL)
    return;

  struct packets *previous = NULL;
  struct packets *temp = *list;

  while(temp != NULL && temp->seqnumber != snumber){
    previous = temp;
    temp = temp->next;
  }

  if(previous == NULL){
    list = &temp->next;
    free(temp->packet);
    free(temp);
  }else if(temp != NULL){
    previous->next = temp->next;
    free(temp->packet);
    free(temp);
  }
}

//method for displaying linkedlist
void displayList(struct packets* pack){
  while(pack != NULL){
    pack = pack->next;
  }
}


int main(int argc, char* argv[]){
  validate(argc,argv);
  char ch;
  int i,j,k,udp_socket;
  int s;
  long b,c;
  struct sockaddr_in clientaddr;
  struct in_addr address;
  char*buff[255];
  int frame_id;
  Frame sender;
  Frame reciever;
  int ack = 1;

  //create udp socket file descriptor
  if((udp_socket = socket(AF_INET,SOCK_DGRAM,0))<0){
    perror("Socket creation failed\n");
  }

  memset(&clientaddr,0,sizeof(clientaddr));
  //connection information
  clientaddr.sin_family = AF_INET;
  clientaddr.sin_port = htons(atoi(argv[2]));
  clientaddr.sin_addr = address;

  //printf("Connected!\n");
  int t = count_lines(argv[3]);
  read_file(argv[3]);
  read_directory();
  int uniq = 1;
  linkedlist = malloc(sizeof(struct payload)*t);

  //how i would have done it
  /*if(recv_ack ==1){
    packets->seqnr = frame_id;
    packets->ack = 0;
    packets->flagg = '0x1';
    create_header(payload,0,flag);
    send_packet(socketfd,&packet->pack,strlen(&packet->pack),0 (struct sockaddr*)&clientaddr, sizeof(clientaddr));
    printf("Packet have been sent\n");
 }
    int addr = sizeof(clientaddr);
    int n = recvfrom(udp_socket,buff, sizeof(data), 0, (struct sockaddr*)NULL,NULL);
    if(n > 0 && reciever.seqnumber ==0 && reciever.ack == frame_id+1){
      printf("ACK recieved\n");
      ack = 0;
    }
    frame_id++;
  */
  for(i=0; i<t; i++){
    char* image = readPGM(buffer[i]);
    //calling function struct packet to create packet with payload
    struct payload *pe = create_payload(image,i,*totalBytes);
    char * test = create_header(pe,2,1,*totalBytes);
    char * test1 = build_packet(test,pe,*totalBytes);
    struct packets* pack = build_struct_packet(test1,pe);

  //Stop&wait
  while(1){
    if(ack==1){
      sender.seqnumber = frame_id;
      sender.frame_ack = 1;
      sender.ack = 0;
      printf("Message read%s\n",data);
      printf("---------Data sending---------\n");
      sendto(udp_socket,data,strlen(data),0,(struct sockaddr*)&clientaddr,sizeof(clientaddr));
      printf("Data buffer sent\n");
    }

    int addr = sizeof(clientaddr);
    int n = recvfrom(udp_socket,buff, sizeof(data), 0, (struct sockaddr*)NULL,NULL);
    if(n > 0 && reciever.seqnumber ==0 && reciever.ack == frame_id+1){
      printf("ACK recieved\n");
      ack = 0;
    }
    frame_id++;
  }
    //addNodeToList(linkedlist,pack);
    //displayList(pe);
    free(pack);
    free(test1);
    free(test);
    //removeNode(linkedlist,pe->uniqnumber);
    free(pe->data);
    free(pe);
    free(totalBytes);
    free(data);
  }
  free(linkedlist);
  bufferFree(t,buffer1);
  bufferFree(t, buffer);
  close(udp_socket);
  return EXIT_SUCCESS;
  //displayList();
  //free(data);
}
