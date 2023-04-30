#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

static int fd_stdout;

void SIGINTQUIT_LST(int sig) {
  char sigint_str[] = "SIGINT received\n";
  char sigquit_str[] = "SIGQUIT received\n";
  if (sig == SIGINT) {
    write(fd_stdout, sigint_str, strlen(sigint_str));
  }
  else if (sig == SIGQUIT) {
    write(fd_stdout, sigquit_str, strlen(sigquit_str));
  }
}

int main(int argc, char** argv) {
  /* catch the SIGINT and SIGQUIT signals with a SIGINTQUIT_LST handler. */
  signal(SIGINT, SIGINTQUIT_LST);
  signal(SIGQUIT, SIGINTQUIT_LST);
  
  int counter = 0;
  int wr_stream;
  if ((wr_stream = open("file.txt", O_CREAT | O_WRONLY, S_IRWXU)) == -1) {
    perror("open() error ");
    exit(EXIT_FAILURE);
  }

  fd_stdout = dup(STDOUT_FILENO);
  if (dup2(wr_stream, STDOUT_FILENO) == -1) {
    perror("dup2() error ");
    exit(EXIT_FAILURE);
  }

  while (1) {
    printf("%d ", counter++);
    sleep(1);
  }
  
  close(wr_stream);
  close(fd_stdout);
  return 0;
}
