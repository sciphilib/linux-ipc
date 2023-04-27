#include <stdio.h>
#include <stdlib.h> // exit
#include <sys/stat.h> // mkfifo
#include <sys/unistd.h> // sleep
#include <fcntl.h>

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("Usage: ./program fifo_name\n");
    exit(EXIT_FAILURE);
  }

  char* fifo_name = argv[1];
  FILE* rd_stream;
  if ((rd_stream = fopen(fifo_name, "r+")) == NULL) {
    perror("fopen() error ");
    exit(EXIT_FAILURE);
  }  

  /* read random numbers to pipe. */
  int num;
  while (1) {
    if (fread(&num, sizeof(int), 1, rd_stream) > 0) {
      printf("%d ", num);
    }
  }

  if(fclose(rd_stream) == -1) {
    perror("In parent process:\n close() error");
  }

  return 0;
}
