#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

int main(int argc, char** argv) {
  /* ignore SIGING signal. */
  signal(SIGINT, SIG_IGN);
  
  int counter = 0;
  int wr_stream;
  
  if ((wr_stream = open("file.txt", O_CREAT | O_WRONLY, S_IRWXU)) == -1) {
    perror("open() error ");
    exit(EXIT_FAILURE);
  }
  
  if (dup2(wr_stream, 1) == -1) {
    perror("dup2() error ");
    exit(EXIT_FAILURE);
  }

  while (1) {
    printf("%d ", counter++);
    sleep(1);
  }
  
  close(wr_stream);
  return 0;
}
