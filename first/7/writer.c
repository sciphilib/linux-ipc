#include <stdio.h>
#include <stdlib.h> // exit
#include <unistd.h> // write

int main(int argc, char** argv) {

  if (argc < 2) {
    printf("Usage: ./program filename\n");
    exit(EXIT_FAILURE);
  }
  
  FILE* file;
  file = fopen(argv[1], "a");
  if (file == NULL) {
    perror("fopen() error ");
    exit(EXIT_FAILURE);
  }

  char curchar;
  while ((curchar = getchar()) != EOF) {
    fwrite(&curchar, 1, 1, file);
  }

  fclose(file);

  return 0;
}
