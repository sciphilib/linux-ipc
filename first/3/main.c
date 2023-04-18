#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h> // fork
#include <unistd.h>    // fork, pid_t, getpid, getppid
#include <sys/wait.h>  // wait

int main(int argc, char** argv)
{
  // divide argv array into pieces
  int pieces = (argc / 2);
  int exitval;
  // square's side
  float side;
  
  switch (fork()) {
  case -1:
    perror("fork() failed!\n");
    exit(EXIT_FAILURE);
  case 0:
    printf("Child calculation:\n");
    for (int i = 1; i <= pieces; i++) {
      side = atof(argv[i]);
      printf("side = %f, square = %f\n", side, side * side);
    }
    exit(EXIT_SUCCESS);
  default:
    printf("Parent calculation:\n");
    for (int i = pieces + 1; i < argc; i++) {
      side = atof(argv[i]);
      printf("side = %f, square = %f\n", side, side * side);
    }
    wait(&exitval);
  }
  exit(EXIT_SUCCESS);
}
