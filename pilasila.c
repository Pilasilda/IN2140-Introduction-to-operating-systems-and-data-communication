#include "header.h"


void freeBuffer(int t, char *buffer[]) {
  int i;
  for(i = 0; i < t; ++i) {
    free(buffer[i]);
  }
}

//reading txt file
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
      buffer[teller] = malloc(strlen(contents) + 1);
      strcpy(buffer[teller], contents);
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
      buffer1[lines] = malloc(strlen(pgmFile));
      strcpy(buffer1[lines],pgmFile);
      //printf("%s\n",buffer1[lines]);
      lines++;
    }
  closedir(directory);
}

static void skip_whites_and_comments(FILE * f){
    int c;
    do{
        while(isspace(c=getc(f))); /* skip spaces */
        if(c=='#') /* skip comments */
            while( c!='\n' && c!='\r' && c!=EOF )
                c=getc(f);
    }
    while( c == '#' || isspace(c) );
    if( c != EOF && ungetc(c,f) == EOF )
      printf("Error: unable to 'ungetc' while reading PGM file.\n");
}


void read_image(char* file){
 FILE* fp = fopen(file,"r");
 char type[255];
 int j,i,max_grey,word;

 if(fp == NULL){
   printf("Empty file.\n");
   exit(1);
 }

 if(feof(fp)){
   printf("END OF FILE\n");
 }

 pgm = malloc(sizeof(struct Image));
 printf("Reading PGM file: %s\n",file);

 fgets(type,sizeof(type), fp);
 //fscanf(fp,"%254s",type);
 if(strcmp(type,"P5") == 0){
  fprintf(stderr, "Wrong file type\n");
  exit(EXIT_FAILURE);
 }

  //get Weight,height
  skip_whites_and_comments(fp);
  fscanf(fp,"%d",&pgm->width);
  skip_whites_and_comments(fp);
  fscanf(fp,"%d", &pgm->height);
  skip_whites_and_comments(fp);
  fscanf(fp, "%d", &max_grey);
  fgetc(fp);

  printf("WIDTH: %d, HEIGHT: %d\n", pgm->width, pgm->height);
  printf("Max_grey value: %d\n",max_grey);
  printf("\n");

  fgetc(fp);
  for(i =0; i < pgm->height; i++){
    pgm->data = malloc(pgm->width * pgm->height* sizeof(unsigned char*));
    fread(pgm->data, sizeof(unsigned char*),pgm->width,fp);
    //printf("%s",pgm->data);
  }
  //printf("Loaded PGM. Size: %dx%d, Greyscale: %d\n",pgm->width, pgm->height,max_grey+1);
  fclose(fp);
  free(pgm);
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

/*void addEntry(char* buffer){
  int w, h;
  unsigned char *d;
  struct Image* tempNode,*iterator;
  tempNode->width = w;
  tempNode->height = h;
  tempNode->data = d;
  tempNode = (struct Image*) malloc(sizeof(struct Image));
}*/


int main(int argc, char* argv[]){
  char ch;
  int i,j;
  int t = count_lines(argv[1]);
  pgm = malloc(sizeof(struct Image));

  read_file(argv[1]);
  /*for(i=0;i<t;i++){
    printf("%s\n",buffer[i]);
  }*/

  read_directory();
  /*for(i=0;i<t;i++){
    printf("%s\n",buffer1[i]);
  }*/

  for(j=0; j<t; j++){
    read_image(buffer[j]);
    pgm = Image_create(buffer[j]);
  }

  freeBuffer(t, buffer1);
  freeBuffer(t,buffer);
  free(pgm);
}
