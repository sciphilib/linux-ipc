#include <stdio.h>
#include <stdlib.h> // exit
#include <sys/stat.h> // mkfifo
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("Usage: ./program fifo_name\n");
    exit(EXIT_FAILURE);
  }

  char* fifo_name = argv[1];
  unlink(fifo_name);
  /* create pipe. */
  if (mkfifo(fifo_name, S_IRWXU) == -1) {
    perror("mkfifo() error ");
    exit(EXIT_FAILURE);
  }

  FILE* wr_stream;
  if ((wr_stream = fopen(fifo_name, "a+")) == NULL) {
    perror("fopen() error ");
    exit(EXIT_FAILURE);
  }  

  /* write random numbers to the pipe. */
  while(1) {
    int num = rand() % 100;
    if (fwrite(&num, sizeof(int), 1, wr_stream) < 1) {
      perror("fwrite() error ");
      exit(EXIT_FAILURE);
    }
  }

  if(fclose(wr_stream) == -1) {
    perror("In parent process:\n close() error");
  }

  return 0;
}
