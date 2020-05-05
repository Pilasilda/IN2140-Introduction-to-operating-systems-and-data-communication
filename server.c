#include "header.h"
void run_server(int udp_socket){
  struct sockaddr_in serveraddr;
  socklen_t count = sizeof(serveraddr);
  char buffer[1000];
  char *msg = "Got your packet";

  count = sizeof(serveraddr);
  recvfrom(udp_socket, buffer, sizeof(buffer),0, (struct sockaddr*)&serveraddr,&count);
  sendto(udp_socket, msg, MAX, 0, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
  printf("Packet sent from client: %s\n", buffer);
}

void validateargs(int argc,char*argv[]){
  if(argc != 2){
    printf("Usage: %s IP port\n",argv[0]);
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char* argv[]){
  struct sockaddr_in serveraddr;
  int udp_socket,yes,ret;
  socklen_t count = sizeof(serveraddr);

  validateargs(argc, argv);

  printf("Creating socket\n");
  //creating udp socket
  udp_socket = socket(AF_INET,SOCK_DGRAM,0);

  if(udp_socket == -1){
    perror("Error while creating socket.\n");
    exit(EXIT_FAILURE);
  }

  printf("Socket created.\n");
  //setting serveraddr to null
  memset(&serveraddr, '\0', sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons(5000);
  setsockopt(udp_socket, SOL_SOCKET, SO_REUSEADDR,&yes, sizeof(int));

  //bind adress to socket descriptor
  printf("Binding\n");
  ret = bind(udp_socket, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
  if(ret== -1){
    perror("Error while binding.\n");
    exit(EXIT_FAILURE);
  }

  printf("Server socket bound.\n");
  run_server(udp_socket);
  //close socket descriptor
  close(udp_socket);
}


//sett innn all alle pakkene i lenkeliste, sende 7 pakker,
//vente paa ack, fjern pakker fra liste
