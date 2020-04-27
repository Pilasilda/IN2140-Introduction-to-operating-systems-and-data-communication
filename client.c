#include "header.h"
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
  DIR* directory = opendir("big_set");
  char buffer[1000];
  struct dirent *entry;
  FILE* fp;
  int lines,ch,i;

  if(directory == NULL){
    perror("Unable to read directory.");
    return(1);
  }

    while((entry = readdir(directory)) != NULL){
      lines++;
      //skip current and parent directory
      if(!strcmp(entry->d_name, ".")){
        continue;
      }
      if(!strcmp(entry->d_name,"..")){
        continue;
      }

      //printf("%s\n",entry->d_name);
      if(entry->d_type != DT_REG){
        continue;
      }

      sprintf(buffer,"%s",entry->d_name);
      //printf("%s\n",buffer);
    }
  closedir(directory);
  return(0);
}

void skipComments(FILE *fp)
{
    int ch;
    char line[100];
    while ((ch = fgetc(fp)) != EOF && isspace(ch)) {
        ;
    }

    if (ch == '#') {
        fgets(line, sizeof(line), fp);
        skipComments(fp);
    } else {
        fseek(fp, -1, SEEK_CUR);
    }
}


void read_image(char*file){
 FILE* fp = fopen(file,"rb");

 char type[255];
 int j,i,max_grey,word;

 if(file == NULL){
   fprintf(stderr, "Empty file: %s\n",file);
   exit(1);
 }

  pgm = malloc(sizeof(struct Image));

  printf("Reading PGM file: %s\n",file);
  fscanf(fp,"%s", type);

 if(strcmp(type,"P2") == 0){
  printf("Valid file type\n");
 }

  skipComments(fp);
  //get Weight,height
  fscanf(fp,"%d",&pgm->width);
  skipComments(fp);
  fscanf(fp,"%d", &pgm->height);
  skipComments(fp);
  fscanf(fp, "%d", &max_grey);
  skipComments(fp);

  printf("WIDTH: %d, HEIGHT: %d\n", pgm->width, pgm->height);

  pgm->data = malloc(sizeof(unsigned char*) *pgm->width *pgm->height);

  for(i =0; i < pgm->height*pgm->width; i++){
    fscanf(fp, "%s", &pgm->data[i]);
    printf("%d\n",pgm->data[i]);
  }

  printf("Loaded PGM. Size: %dx%d, Greyscale: %d\n",pgm->width, pgm->height,max_grey+1);
  fclose(fp);
}




int main(int argc, char* argv[]){
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
  int t = count_lines(argv[1],ch);
  read_file(argv[1]);

  int c = read_directory();
  sprintf(buffer,"%d",c);
  int i;

  for(i=0; i<t; i++){
    read_image(&buffer[i]);
  }
  close(udp_socket);
}
