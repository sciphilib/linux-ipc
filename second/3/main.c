#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

static int SIGINTCNT = 3;
static int wait = 0;

void SIGINT_LST(int sig) {
  if ((--SIGINTCNT) == 0) wait = 1;
}

int main(int argc, char** argv) {
  /* catch the SIGINT signal with a SIGINT_LST handler. */
  signal(SIGINT, SIGINT_LST);
  
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

  while (!wait) {
    printf("%d ", counter++);
    sleep(1);
  }
  
  close(wr_stream);
  return 0;
}
