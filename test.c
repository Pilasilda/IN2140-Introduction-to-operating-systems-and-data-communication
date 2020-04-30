#include "header.h"

void freeBuffer(int t, char *buffer[]) {
  int i;
  for(i = 0; i < t; ++i) {
    free(buffer[i]);
  }
}

void read_file(char *file){
  FILE *fp = fopen(file, "r");
  int ch,lines,i;
  size_t length = 0;

  if(fp == NULL){
    fprintf(stderr, "Empty file.\n");
    exit(1);
  }
  lines = count_lines(file);
  char contents[lines];
  int teller = 0;

  if (file != NULL) {
    while(fscanf(fp, "%s", contents) != EOF) {
      buffer1[teller] = malloc(strlen(contents) + 1);
      strcpy(buffer1[teller], contents);
      teller++;
    }
  }
  fclose(fp);
}

//read content in directory
void read_directory(){
  DIR* directory = opendir("big_set");
  //buffer = malloc(sizeof(buffer));
  struct dirent *entry;
  char* pgmFile;
  FILE* fp;
  int i;
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
      if(entry->d_type != DT_REG){
        continue;
      }

      pgmFile = entry->d_name;
      buffer[lines] = malloc(strlen(pgmFile));
      strcpy(buffer[lines],pgmFile);
      //printf("%s\n",buffer[lines]);
      lines++;
    }
  closedir(directory);
}


int filesize(char*file){
  struct stat buf;
  int r;

  r = stat(file,&buf);
  if(r == -1){
    perror("stat");
    return -1;
  }
  printf("Size of file: %lld bytes.s\n",buf.st_size);
  return 0;
}

void read_image(char* file){
 FILE* fp = fopen(file,"r");
 //struct header *h;
 head = malloc(sizeof(struct header));
 pgm = malloc(sizeof(struct Image));
 printf("Reading PGM file: %s\n",file);

 fscanf(fp,"%s",head->magic_number);
 //get Weight,height
 fscanf(fp,"%d",&head->width);
 //printf("%d\n", head->width);
 fscanf(fp,"%d",&head->height);
 fscanf(fp, "%d", &head->max_value);
 fscanf(fp,"%d", &head->rest);
 //printf("Data: %d\n",head->data);
 serialize_header(head);

 int i;
 for(i =0; i < pgm->height; i++){
   pgm->data = malloc(pgm->width * pgm->height* sizeof(unsigned char*));
   fread(pgm->data, sizeof(unsigned char*),pgm->width,fp);
 }
 free(head);
 free(pgm);
 fclose(fp);
}

char* serialize_int(char *buffer, int value){
  buffer[0] = value >> 24;
  buffer[1] = value >> 16;
  buffer[2] = value >> 8;
  return buffer+4;

}

char* serialize_char(char* buffer, char v){
  buffer[0] = v;
  return buffer+1;
}

char* serialize_header(struct header *temp){
  int i;
  //data = serialize_char(data,temp->magic_number[i]);
  data = serialize_int(data,temp->width);
  data= serialize_int(data,temp->height);
  data = serialize_int(data, temp->max_value);
  data = serialize_int(data,temp->rest);

  return data;
}

int count_lines(char* file) {
  FILE *fp = fopen(file, "r");
  int lines = 0;
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


int main(int argc, char* argv[]){
  char ch;
  int i;
  int t = count_lines(argv[1]);
  //printf("%d\n", t);
  read_file(argv[1]);

  read_directory();
  filesize("bark-1.pgm");

  //printf("%s\n", pgmfiles[i]);
  read_image("bark-1.pgm");
  Image_create(data);

  //printf("%s\n",data);


  /*for(i=0;i<sizeof(pgmfiles);i++){
    //printf("hehehe\n");
    printf("Array content: %s\n",pgmfiles[i]);
  }*/
  //printf("%2s\n",pgmfiles[i]);
  //printf("%x\n",pgmfiles[i]);
  /*for(i=0;i<sizeof(pgmfiles);i++){
    //printf("%s\n",pgmfiles[i]);
  }*/

  //Image_create(&pgmfiles[i]);
  //printf("Information: %\n",pgmfiles[i]);

  //read_image(buffer1[i]);

  /*for(i=0; i<t; i++){
    printf("%s\n", buffer[i]);
    //read_image(buffer[i]);
    //Image_create(buffer1[i]);
  }*/

  freeBuffer(t, buffer1);
}
