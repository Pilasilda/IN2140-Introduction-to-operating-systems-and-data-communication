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
      lines++;

    }
  closedir(directory);
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


void read_image(char* file){
 FILE* fp = fopen(file,"rb");
 pgm = malloc(sizeof(struct Image));

 char type[255];
 int j,i,max_grey,word;

 if(file == NULL){
   fprintf(stderr, "Empty file: %s\n",file);
   exit(1);
 }

 if(feof(fp)){
   printf("End of file reached.\n");
   exit(1);
 }

 if(fp == NULL){
   fprintf(stderr, "Bad file %s\n", file);
 }

 printf("Reading PGM file: %s\n",file);

 fscanf(fp,"%s", type);
 if(strcmp(type,"P2") == 0){
  printf("Valid file type: %s\n", type);
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
  printf("Max_grey value: %d\n",max_grey);

  fgetc(fp);
  for(i =0; i < pgm->height; i++){
    pgm->data = malloc(pgm->width * pgm->height* sizeof(unsigned char*));
    fread(pgm->data, sizeof(unsigned char*),pgm->width,fp);
    printf("%s\n",pgm->data);
  }
  //printf("Loaded PGM. Size: %dx%d, Greyscale: %d\n",pgm->width, pgm->height,max_grey+1);
  fclose(fp);
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
  //char buffer[1000];
  char ch;
  int i;
  int t = count_lines(argv[1]);
  read_file(argv[1]);
  read_directory();

  for(i=0; i<sizeof(buffer1); i++){
    printf("%s\n", buffer1[i]);
    read_image(buffer1[i]);
    //Image_create(buffer[i]);
  }

  freeBuffer(t, buffer1);
}
