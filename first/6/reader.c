#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h> // exit, EXIT_FAILURE
#include <unistd.h> // write

int main(int argc, char** argv) {

  if (argc < 2) {
    printf("Usage: ./program filename\n");
    exit(EXIT_FAILURE);
  }

  int file;
  if ((file = open(argv[1], O_RDONLY)) == -1) {
    perror("open() failed ");
    exit(EXIT_FAILURE);
  }
  
  char curchar;
  while (read(file, &curchar, 1)) {
    putchar(curchar);
  }

  return 0;
}
