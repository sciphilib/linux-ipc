#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

static int is_block;

void SIGINTQUIT_LST(int sig) {
  if (!is_block) {
    if (sig == SIGINT) {
      printf("SIGINT received\n");
    }
    else if (sig == SIGQUIT) {
      printf("SIGQUIT received\n");
    }
  } else {
    printf("Signal ignored\n");
  }
}

int main(int argc, char** argv) {
  /* catch the SIGINT and SIGQUIT signals with a SIGINTQUIT_LST handler. */
  signal(SIGINT, SIGINTQUIT_LST);
  signal(SIGQUIT, SIGINTQUIT_LST);

  int counter = 0;
  FILE* wr_stream;
  if ((wr_stream = fopen("file.txt", "a")) == NULL) {
    perror("open() error ");
    exit(EXIT_FAILURE);
  }

  while (1) {
    /* block if writing to the file and unblock if not. */
    is_block = 1;
    fprintf(wr_stream, "%d ", counter++);
    is_block = 0;
    sleep(1);
  }
  
  fclose(wr_stream);
  return 0;
}
