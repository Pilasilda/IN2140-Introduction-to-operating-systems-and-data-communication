#include "header.h"

int main(int argc, char* argv[]){
  //char *file;
  struct sockaddr_in serveraddr;
  int udp_socket, count;
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
  read_file(argv[1]);
  read_directory();
  close(udp_socket);
}

//Number of lines in textfile
int count_lines(char* file,int ch){
  FILE *fp = fopen(file, "r");
  int lines;
  do{
    ch = fgetc(fp);
    if(ch == '\n'){
      lines++;
    }
  }while(ch != EOF);

  rewind(fp);
  return lines;
}

void read_file(char *file){
  FILE *fp = fopen(file, "r");
  int ch,lines,i;
  size_t length = 0;

  if(fp == NULL){
    fprintf(stderr, "Empty file.\n");
    exit(1);
  }

  lines = count_lines(file, ch);
  char *contents[lines];

  for(i = 0; i < lines; i++){
    contents[i] = NULL;
    getline(&contents[i],&length,fp);
  }

  /*for(i =0; i < lines; i++){
    printf("%s\n",contents[i]);
  }*/

  fclose(fp);
}

//read content in directory
int read_directory(){
  DIR* directory = opendir(".");
  struct dirent *entry;
  FILE* fp;

  if(directory == NULL){
    perror("Unable to read directory.");
    return(1);
  }

  while((entry = readdir(directory)) != NULL){
    if(entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0){
      printf("%s\n", entry->d_name);
      fp= fopen(entry->d_name,"r");
    }
  }

  closedir(directory);
  return(0);
}
