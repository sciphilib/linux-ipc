#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

int main(int argc, char** argv) {
  if (argc < 3) {
    printf("Usage: ./program pid signal\n");
    exit(EXIT_FAILURE);
  }

  pid_t pid = atoi(argv[1]);
  int sign = atoi(argv[2]);
  if (kill(pid, sign) == -1) {
    perror("kill() error: ");
    exit(EXIT_FAILURE);
  }
  
  return 0;
}
