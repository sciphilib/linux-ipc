#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h> // exit, EXIT_FAILURE
#include <unistd.h> // write, getenv
#include <string.h> // strcat

int main(int argc, char** argv) {

  if (argc < 2) {
    printf("Usage: ./program filename\n");
    exit(EXIT_FAILURE);
  }

  int file;
  
  char *filepath = getenv("PWD");
  filepath = strcat(filepath, "/\0");
  filepath = strcat(filepath, argv[1]);

  if ((file = open(filepath, O_RDWR | O_CREAT | O_APPEND, S_IRWXU)) == -1) {
    perror("open() failed!\n");
    exit(EXIT_FAILURE);
  }
  
  char curchar;
  while ((curchar = getchar())) {
    write(file, &curchar, 1);
  }
  curchar = '\0';
  write(file, &curchar, 1);

  return 0;
}
