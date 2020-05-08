#include "header.h"

void validateargs(int argc,char*argv[]){
  if(argc != 2){
    printf("Usage: %s IP port\n",argv[1]);
    exit(EXIT_FAILURE);
  }
  int test;
  test = atoi(argv[1]);
  if(test < 1024 || test > 0xffff){
    printf("Port must be between an interval of 1024-65535.\n");
    exit(EXIT_FAILURE);
  }

}

//Method to write information to file
void write_tofile(char* buffer){
  FILE* ptr;

  ptr = fopen("output.txt","w");
  if(ptr == NULL){
    printf("Error!\n");
    exit(1);
  }

  fgets(buffer,sizeof(buffer),stdin);
  fprintf(ptr, "%s\n", buffer);
  fclose(ptr);
  return;
}

int main(int argc, char* argv[]){
  validateargs(argc,argv);
  int socketfd,length;
  char* buffer = malloc(sizeof(data));
  char* packet = "Got your packet";
  struct sockaddr_in serveraddr;
  struct in_addr adress;


  if((socketfd = socket(AF_INET, SOCK_DGRAM,0)) < 0){
    perror("Socket creation failed.");
    exit(EXIT_FAILURE);
  }

  memset(&serveraddr,0, sizeof(serveraddr));

  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr = address;
  serveraddr.sin_port = htons(atoi(argv[1]));

  if(bind(socketfd, (const struct sockaddr *)&serveraddr,sizeof(serveraddr)) <0){
    perror("Failed binding\n");
    exit(EXIT_FAILURE);
  }

  length = recvfrom(socketfd, (char*)buffer,sizeof(buffer),0,(struct sockaddr*)&serveraddr,NULL);

  buffer[length] = '\0';
  printf("Client: %s\n", buffer);
  sendto(socketfd, packet,strlen(packet),0,(struct sockaddr*)NULL,sizeof(serveraddr));
  return 0;


  //code stop&wait protocol for server, goes to else and printing not recieved.
  /*while(1){
    int recive = recvfrom(socketfd, (char*)buffer,sizeof(buffer),0,(struct sockaddr*)&serveraddr,NULL);
    if(recive > 0 && sender.frame_ack == 1 && reciever.seqnumber == frame_id){
      printf("Frame Received: %s\n", reciever.packet);

      sender.seqnumber = 0;
      sender.frame_ack = reciever.seqnumber +1;
      sendto(socketfd, packet,strlen(packet),0,(struct sockaddr*)NULL,sizeof(serveraddr));

      buffer[length] = '\0';
      printf("Data: %s\n", buffer);
    }else{
      printf("Not received. \n");
    }
    frame_id++;
  }*/

  /*
  int unpack(unsigned char *buff){
    unsigned int i = ((unsigned int)buff[0]<<) | buff[1];
    int e;

    if(i <= 0x7fffu){
      e=i;
  }else{
    e = -1 - (int)(0xffffu - i);
  }
  return e;
}*/
  close(socketfd);
}
