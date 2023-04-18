#include <stdio.h>
#include <stdlib.h> // exit
#include <unistd.h> // read

int main(int argc, char** argv) {

  if (argc < 2) {
    printf("Usage: ./program filename\n");
    exit(EXIT_FAILURE);
  }

  FILE* file;
  file = fopen(argv[1], "r");
  if (file == NULL) {
    perror("fopen() error ");
    exit(EXIT_FAILURE);
  }

  char curchar;
  while (fread(&curchar, 1, 1, file)) {
    putchar(curchar);
  }
  
  return 0;
}
